#include <shVkMemoryInfo.h>
#include <shVkPipelineData.h>
#include <shVkDrawLoop.h>

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
#include "ecs/shMesh.h"

#include "scene/shScene.h"

#include <string.h>
#include <cglm/cglm.h>

#ifdef _MSC_VER
#pragma warning (disable: 6385)
#endif//_MSC_VER

void shUpdateTransform(ShTransform* t) {
	glm_mat4_identity(t->model);
	glm_translate(t->model, t->position);
	glm_scale(t->model, t->scale);
	glm_rotate_x(t->model, SH_DEGREES_TO_RADIANS(t->rotation[0]), t->model);
	glm_rotate_y(t->model, SH_DEGREES_TO_RADIANS(t->rotation[1]), t->model);
	glm_rotate_z(t->model, SH_DEGREES_TO_RADIANS(t->rotation[2]), t->model);
}

void shSceneInit(ShEngine* p_engine, const uint32_t scene_idx) {

	for (uint32_t entity = 0; entity < p_engine->scenes[scene_idx].entity_count; entity++) {
		ShMeshInfo* mesh_info = shGetShMeshInfo(&p_engine->scenes[scene_idx], entity);
		ShTransform* transform = shGetShTransform(&p_engine->scenes[scene_idx], entity);

		if (mesh_info != NULL) {
			ShMesh* mesh = shAddShMesh(&p_engine->scenes[scene_idx], entity);

			//Allocate memory
			if (!(mesh_info->flags & SH_MESH_SETUP_RUNTIME_MESH)) {
				if (mesh_info->vertex_count > 0 && mesh_info->p_vertices != NULL) {
					shCreateVertexBuffer(p_engine->core, mesh_info, mesh);
					shAllocateVertexBuffer(p_engine->core, mesh);
				}
				if (mesh_info->index_count > 0 && mesh_info->p_indices != NULL) {
					shCreateIndexBuffer(p_engine->core, mesh_info, mesh);
					shAllocateIndexBuffer(p_engine->core, mesh);
				}
			}

			//Map memory
			if (mesh_info->flags & SH_MESH_SETUP_STATIC_MESH) {
				if (mesh_info->vertex_count > 0 && mesh_info->p_vertices != NULL) {
					shMapVertexBufferMemory(p_engine->core, mesh_info, mesh);
				}
				if (mesh_info->index_count > 0 && mesh_info->p_indices != NULL) {
					shMapIndexBufferMemory(p_engine->core, mesh_info, mesh);
				}
			}
		}

		if (shHasShMaterialInfo(&p_engine->scenes[scene_idx], entity)) {
			shSetupMaterial(p_engine->core, *shGetShMaterialInfo(&p_engine->scenes[scene_idx], entity), shAddShMaterial(&p_engine->scenes[scene_idx], entity));
		}

		if (transform != NULL) {
			transform->position[1] *= -1.0f;
			shUpdateTransform(transform);
		}
	}

}

void shSceneUpdate(ShEngine* p_engine, const uint32_t scene_idx) {

	ShCamera camera = { 0 };

	for (uint32_t entity = 0; entity < p_engine->scenes[scene_idx].entity_count; entity++) {

		if (shHasShTransform(&p_engine->scenes[scene_idx], entity)) {
			ShTransform* t = shGetShTransform(&p_engine->scenes[scene_idx], entity);
				shUpdateTransform(t);
			
				shEulerToVector(t->rotation, t->front);
			
			glm_vec3_cross((vec3) { 0.0f, 1.0f, 0.0f }, t->front, t->left);
			glm_vec3_normalize(t->left);

			glm_vec3_cross(t->front, t->left, t->up);
			glm_vec3_normalize(t->up);

			if (shHasShCamera(&p_engine->scenes[scene_idx], entity)) {
				camera = *shGetShCamera(&p_engine->scenes[scene_idx], entity);
				if (camera.flags & SH_CAMERA_SETUP_FREE_FLIGHT) {
					float displacement[3] = { 0.0f, 0.0f, 0.0f };
					if (shIsKeyPressed(p_engine->window, SH_KEY_W)) {
						memcpy(displacement, t->front, 12);
						displacement[0] *= camera.speed;
						displacement[1] *= camera.speed;
						displacement[2] *= camera.speed;
					}
					if (shIsKeyPressed(p_engine->window, SH_KEY_A)) {
						memcpy(displacement, t->left, 12);
						displacement[0] *= camera.speed;
						displacement[1] *= camera.speed;
						displacement[2] *= camera.speed;
					}
					if (shIsKeyPressed(p_engine->window, SH_KEY_S)) {
						memcpy(displacement, t->front, 12);
						displacement[0] *= -1.0f * camera.speed;
						displacement[1] *= -1.0f * camera.speed;
						displacement[2] *= -1.0f * camera.speed;
					}
					if (shIsKeyPressed(p_engine->window, SH_KEY_D)) {
						memcpy(displacement, t->left, 12);
						displacement[0] *= -1.0f * camera.speed;
						displacement[1] *= -1.0f * camera.speed;
						displacement[2] *= -1.0f * camera.speed;
					}
					if (shIsKeyPressed(p_engine->window, SH_KEY_E)) {
						memcpy(displacement, t->up, 12);
						displacement[0] *= -1.0f * camera.speed;
						displacement[1] *= -1.0f * camera.speed;
						displacement[2] *= -1.0f * camera.speed;
					}
					if (shIsKeyPressed(p_engine->window, SH_KEY_Q)) {
						memcpy(displacement, t->up, 12);
						displacement[0] *= camera.speed;
						displacement[1] *= camera.speed;
						displacement[2] *= camera.speed;
					}
					displacement[0] *= 5.0f * (float)p_engine->time.delta_time;
					displacement[1] *= 5.0f * (float)p_engine->time.delta_time;
					displacement[2] *= 5.0f * (float)p_engine->time.delta_time;
					glm_vec3_add(t->position, displacement, t->position);
					if (shIsMouseButtonPressed(p_engine->window, SH_MOUSE_BUTTON_RIGHT)) {
						glfwSetInputMode(p_engine->window.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
						double dx, dy = 0.0;
						shMouseOffset(p_engine->window, &dx, &dy);
						t->rotation[0] -= 0.6f * (float)dy * (float)p_engine->time.delta_time;
						t->rotation[1] -= 0.6f * (float)dx * (float)p_engine->time.delta_time;
						if (t->rotation[0] >= SH_DEGREES_TO_RADIANS(89.99999f)) {
							t->rotation[0] = SH_DEGREES_TO_RADIANS(89.99999f);
						}
						if (t->rotation[0] <= SH_DEGREES_TO_RADIANS(-89.99999f)) {
							t->rotation[0] = SH_DEGREES_TO_RADIANS(-89.99999f);
						}
					}
					else if (shIsMouseButtonReleased(p_engine->window, SH_MOUSE_BUTTON_RIGHT)) {
						glfwSetInputMode(p_engine->window.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
					}
				}
				shSetProjection(p_engine->window, camera.fov, camera.nc, camera.fc, camera.projection);
				shSetView(t->position, t->front, t->up, camera.view);
			}
		}

		if (shHasShMeshInfo(&p_engine->scenes[scene_idx], entity) && shHasShMaterial(&p_engine->scenes[scene_idx], entity)) {
			ShMaterial* material = shGetShMaterial(&p_engine->scenes[scene_idx], entity);
			ShMeshInfo* mesh_info = shGetShMeshInfo(&p_engine->scenes[scene_idx], entity);
			ShMesh* mesh = shGetShMesh(&p_engine->scenes[scene_idx], entity);

			void* p_push_constants[128];
			uint32_t push_constants_size = 0;
			if (material->pipeline_data.push_constant_range.size != 0) {
				if (camera.flags != 0) {
					vec4* p_cam_const[2] = { camera.projection, camera.view };
					memcpy((void*)&((char*)p_push_constants)[push_constants_size], &p_cam_const[0][0], sizeof(mat4) * 2);
					push_constants_size += sizeof(mat4) * 2;
				}
			}
			
			void* p_uniform_buffers[64000];
			uint32_t uniform_buffers_size = 0;
			if (material->pipeline_data.uniform_buffer_count != 0) {
				ShTransform* transform = shGetShTransform(&p_engine->scenes[scene_idx], entity);
				ShPhysicsClient* client = shGetShPhysicsClient(&p_engine->scenes[scene_idx], entity);
				if (transform != NULL) {
					memcpy(&((char*)p_uniform_buffers)[uniform_buffers_size], transform->model, 64);
					uniform_buffers_size += 64;
				}
				if (client != NULL) {
					if (*client & SH_PHYSICS_CLIENT_ELECTROSTATICS) {
						memcpy(&((char*)&p_uniform_buffers)[uniform_buffers_size], 
							p_engine->physics_host.electrostatic_world.charges, 16 * 32);
						uniform_buffers_size += 16 * 32;
						float dtime[1] = { (float)p_engine->time.delta_time };
						memcpy(&((char*)&p_uniform_buffers)[uniform_buffers_size], dtime, 16);
						uniform_buffers_size += 16;
					}
				}
			}

			shRenderMesh(p_engine->core, material->pipeline_data, 
				push_constants_size, p_push_constants, 
				uniform_buffers_size, p_uniform_buffers,
				mesh_info, mesh);
		}
	}
}

void shRenderMesh(const ShVkCore core, const ShVkPipelineData pipe_data, const uint32_t push_const_size, void* p_push_const, const uint32_t uniforms_size, void* p_uniforms, ShMeshInfo* p_mesh_info, ShMesh* mesh) {

	//Map mesh buffers
	if (p_mesh_info->flags & SH_MESH_SETUP_DYNAMIC_MESH) {
		shMapVertexBufferMemory(core, p_mesh_info, mesh);
		if ((size_t)mesh->index_buffer_memory != 0) {
			shMapIndexBufferMemory(core, p_mesh_info, mesh);
		}
	}

	//Bind vertex and index buffers
	if ((size_t)mesh->vertex_buffer_memory != 0) {
		shBindVertexBuffer(core, &mesh->vertex_buffer);
	}
	if ((size_t)mesh->index_buffer_memory != 0) {
		shBindIndexBuffer(core, &mesh->index_buffer);
	}

	shBindPipeline(core.cmd_buffers[0], pipe_data);

	//push constants
	if (push_const_size != 0 && p_push_const != NULL) {
		shPushConstants(core.cmd_buffers[0], pipe_data, p_push_const);
	}

	// bind uniform memory
	uint32_t uniform_offset = 0;
	for (uint32_t i = 0; i < pipe_data.uniform_buffer_count; i++) {
		shMapMemory(core.device,
			pipe_data.p_uniform_buffers[i].uniform_buffer_memory,
			pipe_data.p_uniform_buffers[i].uniform_buffer_size,
			&((char*)p_uniforms)[uniform_offset]);
		shBindDescriptorSets(core, pipe_data);
		uniform_offset += pipe_data.p_uniform_buffers[i].uniform_buffer_size;
	}

	if ((size_t)mesh->vertex_buffer_memory != 0) {
		if ((size_t)mesh->index_buffer_memory != 0) {
			shDraw(core.cmd_buffers[0], p_mesh_info->index_count, 1);
		}
		else {
			shDraw(core.cmd_buffers[0], p_mesh_info->vertex_count / pipe_data.vertexStride / 4, 0);
		}
	}
}

void shSceneRelease(ShEngine* p_engine, const uint32_t scene_idx) {

	vkDeviceWaitIdle(p_engine->core.device);

	for (uint32_t entity = 0; entity < p_engine->scenes[scene_idx].entity_count; entity++) {

		if (shHasShMesh(&p_engine->scenes[scene_idx], entity)) {
			ShMesh* mesh = shGetShMesh(&p_engine->scenes[scene_idx], entity);
			if ((size_t)mesh->vertex_buffer_memory != 0) {
				shClearBufferMemory(p_engine->core.device, mesh->vertex_buffer, mesh->vertex_buffer_memory);
			}
			if ((size_t)mesh->index_buffer_memory != 0) {
				shClearBufferMemory(p_engine->core.device, mesh->index_buffer, mesh->index_buffer_memory);
			}
			mesh = NULL;
		}

		if (shHasShMaterial(&p_engine->scenes[scene_idx], entity)) {
			ShMaterial* mat = shGetShMaterial(&p_engine->scenes[scene_idx], entity);
			shDestroyPipeline(p_engine->core, &mat->pipeline_data);
			mat = NULL;
		}

	}
	shClearScene(&p_engine->scenes[scene_idx]);
}
