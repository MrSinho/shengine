#include <shvulkan/shVkMemoryInfo.h>
#include <shvulkan/shVkPipelineData.h>
#include <shvulkan/shVkDrawLoop.h>

#include "engine/shEngine.h"
#include "engine/shInput.h"

#include "linear_algebra/shProjection.h"
#include "linear_algebra/shView.h"
#include "linear_algebra/shEuler.h"
#include "linear_algebra/shView.h"
#include "linear_algebra/shProjection.h"

#include "ecs/shTransform.h"
#include "ecs/shCamera.h"
#include "ecs/shMaterial.h"
#include "ecs/shMesh.h"

#include "scene/shScene.h"

#include <string.h>
#include <cglm/cglm.h>

#ifdef _MSC_VER
#pragma warning (disable: 6385)
#endif//_MSC_VER

void shSceneInit(ShEngine* p_engine, const uint32_t scene_idx) {

	for (uint32_t entity = 0; entity < p_engine->scenes[scene_idx].entity_count; entity++) {
		ShMesh* p_mesh = shGetShMesh(&p_engine->scenes[scene_idx], entity);
		ShTransform* p_transform = shGetShTransform(&p_engine->scenes[scene_idx], entity);

		if (p_mesh != NULL) {
			if (p_mesh->mesh_info.vertex_count > 0 && p_mesh->mesh_info.p_vertices != NULL) {
				shCreateVertexBuffer(&p_engine->core, p_mesh->mesh_info.vertex_count * 4, &p_mesh->vertex_buffer);
				shAllocateVertexBuffer(&p_engine->core, p_mesh->vertex_buffer, &p_mesh->vertex_buffer_memory);
				if (p_mesh->mesh_info.flags & SH_MESH_SETUP_STATIC_MESH) {
					shWriteVertexBufferMemory(&p_engine->core, p_mesh->vertex_buffer_memory, p_mesh->mesh_info.vertex_count * 4, p_mesh->mesh_info.p_vertices);
				}
			}
			if (p_mesh->mesh_info.index_count > 0 && p_mesh->mesh_info.p_indices != NULL) {
				shCreateIndexBuffer(&p_engine->core, p_mesh->mesh_info.index_count * 4, &p_mesh->index_buffer);
				shAllocateIndexBuffer(&p_engine->core, p_mesh->index_buffer, &p_mesh->index_buffer_memory);
				if (p_mesh->mesh_info.flags & SH_MESH_SETUP_STATIC_MESH) {
					shWriteIndexBufferMemory(&p_engine->core, p_mesh->index_buffer_memory, p_mesh->mesh_info.index_count * 4, p_mesh->mesh_info.p_indices);
				}
			}
		}

		if (p_transform != NULL) {
			p_transform->position[1] *= -1.0f;
			shUpdateShTransform(p_transform);
		}
	}

}

void shUpdateShTransform(ShTransform* p_transform) {
	assert(p_transform != NULL);

	glm_mat4_identity(p_transform->model);
	glm_translate(p_transform->model, p_transform->position);
	glm_scale(p_transform->model, p_transform->scale);
	glm_rotate_x(p_transform->model, SH_DEGREES_TO_RADIANS(p_transform->rotation[0]), p_transform->model);
	glm_rotate_y(p_transform->model, SH_DEGREES_TO_RADIANS(p_transform->rotation[1]), p_transform->model);
	glm_rotate_z(p_transform->model, SH_DEGREES_TO_RADIANS(p_transform->rotation[2]), p_transform->model);
	
	shEulerToVector(p_transform->rotation, p_transform->front);
	
	glm_vec3_cross((vec3) { 0.0f, 1.0f, 0.0f }, p_transform->front, p_transform->left);
	glm_vec3_normalize(p_transform->left);
	
	glm_vec3_cross(p_transform->front, p_transform->left, p_transform->up);
	glm_vec3_normalize(p_transform->up);
}

void shUpdateShCamera(ShEngine* p_engine, ShTransform* p_transform, ShCamera* p_camera) {
	assert(p_transform != NULL && p_camera != NULL);

	if (p_camera->flags & SH_CAMERA_SETUP_FREE_FLIGHT) {
		float displacement[3] = { 0.0f, 0.0f, 0.0f };
		if (shIsKeyPressed(p_engine->window, SH_KEY_W)) {
			memcpy(displacement, p_transform->front, 12);
			glm_vec3_mul(displacement, (vec3){ p_camera->speed, p_camera->speed, p_camera->speed }, displacement);
		}
		if (shIsKeyPressed(p_engine->window, SH_KEY_A)) {
			memcpy(displacement, p_transform->left, 12);
			glm_vec3_mul(displacement, (vec3) { p_camera->speed, p_camera->speed, p_camera->speed }, displacement);
		}
		if (shIsKeyPressed(p_engine->window, SH_KEY_S)) {
			memcpy(displacement, p_transform->front, 12);
			glm_vec3_mul(displacement, (vec3) { -p_camera->speed, -p_camera->speed, -p_camera->speed }, displacement);
		}
		if (shIsKeyPressed(p_engine->window, SH_KEY_D)) {
			memcpy(displacement, p_transform->left, 12);
			glm_vec3_mul(displacement, (vec3) { -p_camera->speed, -p_camera->speed, -p_camera->speed }, displacement);
		}
		if (shIsKeyPressed(p_engine->window, SH_KEY_E)) {
			memcpy(displacement, p_transform->up, 12);
			glm_vec3_mul(displacement, (vec3) { -p_camera->speed, -p_camera->speed, -p_camera->speed }, displacement);
		}
		if (shIsKeyPressed(p_engine->window, SH_KEY_Q)) {
			memcpy(displacement, p_transform->up, 12);
			glm_vec3_mul(displacement, (vec3) { p_camera->speed, p_camera->speed, p_camera->speed }, displacement);
		}
		glm_vec3_mul(displacement, (vec3) { (float)p_engine->time.delta_time, (float)p_engine->time.delta_time, (float)p_engine->time.delta_time }, displacement);
		glm_vec3_add(p_transform->position, displacement, p_transform->position);
		if (shIsMouseButtonPressed(p_engine->window, SH_MOUSE_BUTTON_RIGHT)) {
			glfwSetInputMode(p_engine->window.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			p_transform->rotation[0] -= 3.0f * (float)p_engine->window.d_cursor_pos_y * (float)p_engine->time.delta_time;
			p_transform->rotation[1] -= 3.0f * (float)p_engine->window.d_cursor_pos_x * (float)p_engine->time.delta_time;
			if (p_transform->rotation[0] >= SH_DEGREES_TO_RADIANS(89.99999f)) {
				p_transform->rotation[0] = SH_DEGREES_TO_RADIANS(89.99999f);
			}
			if (p_transform->rotation[0] <= SH_DEGREES_TO_RADIANS(-89.99999f)) {
				p_transform->rotation[0] = SH_DEGREES_TO_RADIANS(-89.99999f);
			}
		}
		else if (shIsMouseButtonReleased(p_engine->window, SH_MOUSE_BUTTON_RIGHT)) {
			glfwSetInputMode(p_engine->window.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
	shUpdateShTransform(p_transform);
	shSetProjection(p_engine->window, p_camera->fov, p_camera->nc, p_camera->fc, p_camera->projection);
	shSetView(p_transform->position, p_transform->front, p_transform->up, p_camera->view);
}

void shSceneUpdate(ShEngine* p_engine, const uint32_t scene_idx) {

	ShCamera* p_camera = NULL;
	void* p_push_constant[128/8];
	uint32_t uniform_buffer_offset = 0;
	ShScene* p_scene = &p_engine->scenes[scene_idx];

	for (uint32_t entity = 0; entity < p_scene->entity_count; entity++) {
		if (shHasShCamera(p_scene, entity) && shHasShTransform(p_scene, entity)) {
			p_camera = shGetShCamera(p_scene, entity);
			ShTransform* p_transform = shGetShTransform(p_scene, entity);
			shUpdateShCamera(p_engine, p_transform, p_camera);
			break;
		}
	}

	for (uint32_t i = 0; i < p_engine->material_count; i++) {
		ShMaterialHost* p_material = &p_engine->p_materials[i];
		shBindPipeline(&p_engine->core, &p_material->pipeline);

		if (p_material->pipeline.push_constant_range.size != 0) {
			if (p_camera != NULL) {
				memcpy(p_push_constant, p_camera->projection, 64);
				memcpy(&p_push_constant[64 / 8], p_camera->view, 64);
			}
			shPushConstants(&p_engine->core, p_push_constant, &p_material->pipeline);
		}

		if (p_material->pipeline.uniform_count) {
			shUpdateUniformBuffers(&p_engine->core, &p_material->pipeline);
			for (uint32_t j = 0; j < p_material->pipeline.uniform_count; j++) {
				void* p_uniform_buffer[64000 / 8];
				if (!p_material->pipeline.dynamic_uniforms[j]) {
					shWriteUniformBufferMemory(&p_engine->core, j, p_uniform_buffer, &p_material->pipeline);
					shBindUniformBuffer(&p_engine->core, j, &p_material->pipeline);
				}
			}
		}

		for (uint32_t i = 0; i < p_material->entity_count; i++) {
			const uint32_t entity = p_material->entities[i];

			for (uint32_t j = 0; j < p_material->pipeline.uniform_count; j++) {
				void* p_uniform_buffer[64000 / 8];
				if (p_material->pipeline.dynamic_uniforms[j]) {
					if (shHasShTransform(p_scene, entity)) {
						memcpy(p_uniform_buffer, shGetShTransform(p_scene, entity)->model, 64);
						uniform_buffer_offset += 64;
					}
					shWriteDynamicUniformBufferMemory(&p_engine->core, j, p_uniform_buffer, &p_material->pipeline);
					shBindDynamicUniformBuffer(&p_engine->core, j, &p_material->pipeline);
				}
			}

			if (shHasShMesh(p_scene, entity)) {
				ShMesh* p_mesh = shGetShMesh(p_scene, entity);
				if (p_mesh->mesh_info.vertex_count != 0) {
					if (p_mesh->mesh_info.flags & SH_MESH_SETUP_DYNAMIC_MESH) {
						shWriteVertexBufferMemory(
							&p_engine->core,
							p_mesh->vertex_buffer_memory,
							p_mesh->mesh_info.vertex_count * sizeof(p_mesh->mesh_info.p_vertices[0]),
							p_mesh->mesh_info.p_vertices
						);
					}
					shBindVertexBuffer(&p_engine->core, &p_mesh->vertex_buffer);
				}
				if (p_mesh->mesh_info.index_count != 0) {
					if (p_mesh->mesh_info.flags & SH_MESH_SETUP_DYNAMIC_MESH) {
						shWriteIndexBufferMemory(
							&p_engine->core,
							p_mesh->index_buffer_memory,
							p_mesh->mesh_info.index_count * sizeof(p_mesh->mesh_info.p_indices[0]),
							p_mesh->mesh_info.p_indices
						);
					}
					shBindIndexBuffer(&p_engine->core, &p_mesh->index_buffer);
				}
				if (p_mesh->mesh_info.index_count != 0) {
					shDrawIndexed(&p_engine->core, p_mesh->mesh_info.index_count);
				}
				else if (p_mesh->mesh_info.index_count == 0 && p_mesh->mesh_info.vertex_count != 0) {
					shDraw(&p_engine->core, p_mesh->mesh_info.vertex_count / p_mesh->mesh_info.vertex_stride);
				}
			}
		}

		shEndPipeline(&p_material->pipeline);
	}

	for (uint32_t entity = 0; entity < p_engine->scenes[scene_idx].entity_count; entity++) {
		//
		//if (shHasShMesh(&p_engine->scenes[scene_idx], entity) && shHasShMaterial(&p_engine->scenes[scene_idx], entity)) {
		//	ShMaterial* p_material = shGetShMaterial(&p_engine->scenes[scene_idx], entity);
		//	ShMesh* p_mesh = shGetShMesh(&p_engine->scenes[scene_idx], entity);
		//	
		//	shBindPipeline(&p_engine->core, &p_material->pipeline);
		//
		//	uint32_t push_constants_size = 0;
		//	if (p_material->pipeline.push_constant_range.size != 0) {
		//		if (camera.flags != 0) {
		//			vec4* p_cam_const[2] = { camera.projection, camera.view };
		//			memcpy((void*)&((char*)p_push_constants)[push_constants_size], &p_cam_const[0][0], sizeof(mat4) * 2);
		//			push_constants_size += sizeof(mat4) * 2;
		//		}
		//	}
		//	shPushConstants(&p_engine->core, p_push_constants, &p_material->pipeline);
		//
		//	uint32_t uniform_buffers_size = 0;
		//	if (p_material->pipeline.uniform_count != 0) {
		//		shUpdateUniformBuffers(&p_engine->core, &p_material->pipeline);
		//		ShTransform* transform = shGetShTransform(&p_engine->scenes[scene_idx], entity);
		//		if (transform != NULL) {
		//			memcpy(&((char*)p_uniform_buffers)[uniform_buffers_size], transform->model, 64);
		//			uniform_buffers_size += 64;
		//			shWriteUniformBufferMemory(&p_engine->core, 0, p_uniform_buffers, &p_material->pipeline);
		//			shBindDynamicUniformBuffer(&p_engine->core, 0, &p_material->pipeline);
		//		}
		//		//ShPhysicsClient* client = shGetShPhysicsClient(&p_engine->scenes[scene_idx], entity);
		//		//if (client != NULL) {
		//		//	if (*client & SH_PHYSICS_CLIENT_ELECTROSTATICS) {
		//		//		memcpy(&((char*)&p_uniform_buffers)[uniform_buffers_size], 
		//		//			p_engine->physics_host.electrostatic_world.charges, 16 * 32);
		//		//		uniform_buffers_size += 16 * 32;
		//		//		float dtime[1] = { (float)p_engine->time.delta_time };
		//		//		memcpy(&((char*)&p_uniform_buffers)[uniform_buffers_size], dtime, 16);
		//		//		uniform_buffers_size += 16;
		//		//	}
		//		//}
		//	}
		//	
		//	if (p_mesh->vertex_buffer != NULL) {
		//		shBindVertexBuffer(&p_engine->core, &p_mesh->vertex_buffer);
		//	}
		//	if (p_mesh->index_buffer != NULL) {
		//		shBindIndexBuffer(&p_engine->core, &p_mesh->index_buffer);
		//		shDrawIndexed(&p_engine->core, p_mesh->mesh_info.index_count);
		//	}
		//	if (p_mesh->index_buffer == NULL) {
		//		shDraw(&p_engine->core, p_mesh->mesh_info.vertex_count / p_mesh->mesh_info.vertex_stride * 4);
		//	}
		//
		//	shEndPipeline(&p_material->pipeline);
		//}
	}	//
}

//void shRenderMesh(ShVkCore* p_core, ShVkGraphicsPipeline* p_pipeline, const uint32_t push_const_size, void* p_push_const, const uint32_t uniforms_size, void* p_uniforms, ShMesh* p_mesh) {
//
//	////Map mesh buffers
//	//if (p_mesh->mesh_info.flags & SH_MESH_SETUP_DYNAMIC_MESH) {
//	//	shWriteVertexBufferMemory(p_core, p_mesh->vertex_buffer_memory, p_mesh->mesh_info.vertex_count * 4, p_mesh->mesh_info.p_vertices);
//	//	if ((size_t)p_mesh->index_buffer_memory != 0) {
//	//		shMapVertexBufferMemory(p_core, p_mesh->index_buffer_memory, p_mesh->mesh_info.index_count * 4, p_mesh->mesh_info.p_indices);
//	//	}
//	//}
//	//
//	////Bind vertex and index buffers
//	//if ((size_t)p_mesh->vertex_buffer_memory != 0) {
//	//	shBindVertexBuffer(p_core, &p_mesh->vertex_buffer);
//	//}
//	//if ((size_t)p_mesh->index_buffer_memory != 0) {
//	//	shBindIndexBuffer(p_core, &p_mesh->index_buffer);
//	//}
//	//
//	//shBindPipeline(p_core, p_pipeline);
//	//
//	////push constants
//	//if (push_const_size != 0 && p_push_const != NULL) {
//	//	shPushConstants(p_core, p_pipeline, p_push_const);
//	//}
//	//
//	//// bind uniform memory
//	//uint32_t uniform_offset = 0;
//	//for (uint32_t i = 0; i < p_pipeline->uniform_count; i++) {
//	//	shMapMemory(p_core->device,
//	//		p_pipeline->uniform_buffers_memory[i],
//	//		p_pipeline->uniform_buffers_size[i],
//	//		&((char*)p_uniforms)[uniform_offset]);
//	//	shBindUniformBuffer(p_core, i, p_pipeline);
//	//	uniform_offset += p_pipeline->uniform_buffers_size[i];
//	//}
//	//
//	//if ((size_t)p_mesh->vertex_buffer_memory != 0) {
//	//	if ((size_t)p_mesh->index_buffer_memory != 0) {
//	//		shDraw(p_core, p_mesh->mesh_info.index_count, 1);
//	//	}
//	//	else {
//	//		shDraw(p_core, p_mesh->mesh_info.vertex_count / p_mesh->mesh_info.vertex_stride * 4, 0);
//	//	}
//	//}
//}

void shSceneRelease(ShEngine* p_engine, const uint32_t scene_idx) {

	vkDeviceWaitIdle(p_engine->core.device);

	for (uint32_t entity = 0; entity < p_engine->scenes[scene_idx].entity_count; entity++) {

		if (shHasShMesh(&p_engine->scenes[scene_idx], entity)) {
			ShMesh* p_mesh = shGetShMesh(&p_engine->scenes[scene_idx], entity);
			if ((size_t)p_mesh->vertex_buffer_memory != 0) {
				shClearBufferMemory(p_engine->core.device, p_mesh->vertex_buffer, p_mesh->vertex_buffer_memory);
			}
			if ((size_t)p_mesh->index_buffer_memory != 0) {
				shClearBufferMemory(p_engine->core.device, p_mesh->index_buffer, p_mesh->index_buffer_memory);
			}
			p_mesh = NULL;
			shRemoveShMesh(&p_engine->scenes[scene_idx], entity);
		}
	}
	shClearScene(&p_engine->scenes[scene_idx]);
}
