#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shvulkan/shVkMemoryInfo.h>
#include <shvulkan/shVkPipelineData.h>
#include <shvulkan/shVkDrawLoop.h>

#include "shengine/shEngine.h"
#include "shengine/shInput.h"

#include "shlinear-algebra/shProjection.h"
#include "shlinear-algebra/shView.h"
#include "shlinear-algebra/shEuler.h"
#include "shlinear-algebra/shView.h"
#include "shlinear-algebra/shProjection.h"

#include "shecs/shComponents.h"

#include "shscene/shScene.h"

#include <string.h>
#include <cglm/cglm.h>

#ifdef _MSC_VER
#pragma warning (disable: 6385)
#endif//_MSC_VER



void shSceneInit(ShEngine* p_engine, ShScene* p_scene) {
		
	for (uint32_t entity = 0; entity < p_scene->entity_count; entity++) {
		ShMesh* p_mesh = shGetShMesh(&p_engine->scene, entity);
		ShTransform* p_transform = shGetShTransform(&p_engine->scene, entity);

		if (p_mesh != NULL) {
			if (p_mesh->mesh_info.vertex_count > 0 && p_mesh->mesh_info.p_vertices != NULL) {
				shCreateVertexBuffer(p_engine->core.device, p_mesh->mesh_info.vertex_count * 4, 0, &p_mesh->vertex_buffer);
				shAllocateVertexBufferMemory(p_engine->core.device, p_engine->core.physical_device, p_mesh->vertex_buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &p_mesh->vertex_buffer_memory);
				if (p_mesh->mesh_info.flags & SH_MESH_SETUP_STATIC_MESH) {
					shWriteVertexBufferMemory(p_engine->core.device, p_mesh->vertex_buffer_memory, 0, p_mesh->mesh_info.vertex_count * 4, p_mesh->mesh_info.p_vertices);
				}
				shBindVertexBufferMemory(p_engine->core.device, p_mesh->vertex_buffer, 0, p_mesh->vertex_buffer_memory);
			}
			if (p_mesh->mesh_info.index_count > 0 && p_mesh->mesh_info.p_indices != NULL) {
				shCreateIndexBuffer(p_engine->core.device, p_mesh->mesh_info.index_count * 4, 0, &p_mesh->index_buffer);
				shAllocateIndexBufferMemory(p_engine->core.device, p_engine->core.physical_device, p_mesh->index_buffer, &p_mesh->index_buffer_memory);
				if (p_mesh->mesh_info.flags & SH_MESH_SETUP_STATIC_MESH) {
					shWriteIndexBufferMemory(p_engine->core.device, p_mesh->index_buffer_memory, 0, p_mesh->mesh_info.index_count * 4, p_mesh->mesh_info.p_indices);
				}
				shBindIndexBufferMemory(p_engine->core.device, p_mesh->index_buffer, 0, p_mesh->index_buffer_memory);
			}
		}

		//INIT PER ENTITY EXTENSIONS
		if (p_transform != NULL) {
			p_transform->position[1] *= -1.0f;
			shUpdateShTransform(p_transform);
		}
	}

}

void shUpdateShTransform(ShTransform* p_transform) {
	shEngineError(p_transform == NULL, "shUpdateShTransform: invalid transform memory", exit(-1));

	glm_mat4_identity(p_transform->model);
	glm_translate(p_transform->model, p_transform->position);
	glm_scale(p_transform->model, p_transform->scale);
	glm_rotate_x(p_transform->model, p_transform->rotation[0], p_transform->model);
	glm_rotate_y(p_transform->model, p_transform->rotation[1], p_transform->model);
	glm_rotate_z(p_transform->model, p_transform->rotation[2], p_transform->model);
	
	shEulerToVector(p_transform->rotation, p_transform->front);
	
	glm_vec3_cross((vec3) { 0.0f, 1.0f, 0.0f }, p_transform->front, p_transform->left);
	glm_vec3_normalize(p_transform->left);
	
	glm_vec3_cross(p_transform->front, p_transform->left, p_transform->up);
	glm_vec3_normalize(p_transform->up);

	for (uint32_t i = 0; i < sizeof(p_transform->rotation) / sizeof(p_transform->rotation[0]); i++) {
		p_transform->rotation[i] -= (float)((int)p_transform->rotation[i] / (int)(SH_DEGREES_TO_RADIANS(360.0f))) * SH_DEGREES_TO_RADIANS(360.0f);
	}
	p_transform->euler[0] = SH_RADIANS_TO_DEGREES(p_transform->rotation[0]);
	p_transform->euler[1] = SH_RADIANS_TO_DEGREES(p_transform->rotation[1]);
	p_transform->euler[2] = SH_RADIANS_TO_DEGREES(p_transform->rotation[2]);
}

void shUpdateShCamera(ShEngine* p_engine, ShTransform* p_transform, ShCamera* p_camera) {
	shEngineError(p_transform == NULL || p_camera == NULL, "shUpdateShCamera: invalid transform memory", exit(-1));

	if (p_camera->flags & SH_CAMERA_SETUP_FREE_FLIGHT) {
		float displacement[3] = { 0.0f, 0.0f, 0.0f };
		if (shIsKeyDown(p_engine->window, SH_KEY_W)) {
			memcpy(displacement, p_transform->front, 12);
			glm_vec3_mul(displacement, (vec3){ p_camera->speed, p_camera->speed, p_camera->speed }, displacement);
		}
		if (shIsKeyDown(p_engine->window, SH_KEY_A)) {
			memcpy(displacement, p_transform->left, 12);
			glm_vec3_mul(displacement, (vec3) { p_camera->speed, p_camera->speed, p_camera->speed }, displacement);
		}
		if (shIsKeyDown(p_engine->window, SH_KEY_S)) {
			memcpy(displacement, p_transform->front, 12);
			glm_vec3_mul(displacement, (vec3) { -p_camera->speed, -p_camera->speed, -p_camera->speed }, displacement);
		}
		if (shIsKeyDown(p_engine->window, SH_KEY_D)) {
			memcpy(displacement, p_transform->left, 12);
			glm_vec3_mul(displacement, (vec3) { -p_camera->speed, -p_camera->speed, -p_camera->speed }, displacement);
		}
		if (shIsKeyDown(p_engine->window, SH_KEY_E)) {
			memcpy(displacement, p_transform->up, 12);
			glm_vec3_mul(displacement, (vec3) { -p_camera->speed, -p_camera->speed, -p_camera->speed }, displacement);
		}
		if (shIsKeyDown(p_engine->window, SH_KEY_Q)) {
			memcpy(displacement, p_transform->up, 12);
			glm_vec3_mul(displacement, (vec3) { p_camera->speed, p_camera->speed, p_camera->speed }, displacement);
		}
		glm_vec3_mul(displacement, (vec3) { (float)p_engine->time.delta_time, (float)p_engine->time.delta_time, (float)p_engine->time.delta_time }, displacement);
		glm_vec3_add(p_transform->position, displacement, p_transform->position);
		uint8_t gui_condition = 0;
		if (p_engine->p_gui != NULL) {
			gui_condition = p_engine->p_gui->region_infos.cursor_on_regions;
		}
		if (shIsMouseButtonDown(p_engine->window, SH_MOUSE_BUTTON_RIGHT) && gui_condition == 0) {
			glfwSetInputMode(p_engine->window.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			p_transform->rotation[0] -= p_camera->mouse_speed / 100.0f * (float)p_engine->window.input.d_cursor_pos_y * (float)p_engine->time.delta_time;
			p_transform->rotation[1] -= p_camera->mouse_speed / 100.0f * (float)p_engine->window.input.d_cursor_pos_x * (float)p_engine->time.delta_time;
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
	//shUpdateShTransform(p_transform);

	shSetProjection(p_engine->window, p_camera->fov, p_camera->nc, p_camera->fc, p_camera->projection);
	shSetView(p_transform->position, p_transform->front, p_transform->up, p_camera->view);
}

void shSceneUpdate(ShEngine* p_engine) {
	shEngineError(p_engine == NULL, "shSceneUpdate: invalid engine memory", exit(-1));

	ShScene* p_scene = &p_engine->scene;

	ShCamera* p_camera = NULL;
	{//FIND CAMERA FOR PROJECTION AND VIEW MATRICES (CURRENTLY RESERVED FOR PUSH CONSTANTS)
		ShTransform* p_camera_transform = NULL;
		for (uint32_t entity = 0; entity < p_scene->entity_count; entity++) {
			p_camera = shGetShCamera(p_scene, entity);
			p_camera_transform = shGetShTransform(p_scene, entity);
			if (p_camera != NULL && p_camera_transform != NULL) {
				shUpdateShCamera(p_engine, p_camera_transform, p_camera);
				break;
			}
		}
	}

	for (uint32_t entity = 0; entity < p_scene->entity_count; entity++) {//UPDATE NON MATERIAL RELATED STUFF
		ShTransform* p_transform = shGetShTransform(p_scene, entity);
		if (p_transform != NULL) {
			shUpdateShTransform(p_transform);
		}
	}

	for (uint32_t material_idx = 0; material_idx < p_engine->material_count; material_idx++) {
		
		ShMaterialHost* p_material = &p_engine->p_materials[material_idx];

		if (!p_material->bind_on_loop) {
			break;
		}

		shBindPipeline(p_engine->core.p_graphics_commands[0].cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, &p_material->pipeline);

		if (p_material->pipeline.push_constant_range.size) {//WRITE PUSH CONSTANT DATA (CURRENTLY ONLY FOR PROJECTION AND VIEW MATRIX)
			if (p_camera != NULL) {
				memcpy(p_material->push_constant, p_camera->projection, 64);
				memcpy(&((char*)p_material->push_constant)[64], p_camera->view, 64);
			}
			shPipelinePushConstants(p_engine->core.p_graphics_commands[0].cmd_buffer, p_material->push_constant, &p_material->pipeline);
		}

		if (p_material->pipeline.descriptor_count) {//ALL UNIFORMS CHECK
			shPipelineUpdateDescriptorSets(p_engine->core.device, &p_material->pipeline);//UPDATE ALL UNIFORMS
			
			for (uint32_t descriptor_idx = 0; descriptor_idx < p_material->pipeline.descriptor_count; descriptor_idx++) {//WRITE AND BIND STATIC UNIFORMS

				if (p_material->pipeline.descriptor_set_layout_bindings[descriptor_idx].descriptorType != VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC) {
					uint32_t descriptor_offset = shGetUniformOffset(p_material, descriptor_idx);

					for (uint32_t entity = 0; entity < p_scene->entity_count; entity++) {
						if (shEntityInMaterial(entity, p_material)) {
							memcpy(
								&((char*)p_material->uniform_buffers)[descriptor_offset],
								&((char*)p_material->p_material_clients[entity].p_uniform_parameters)[descriptor_offset],
								p_material->pipeline.descriptor_buffer_infos[descriptor_idx].range
							);
						}
					}

					shPipelineWriteDescriptorBufferMemory(
						p_engine->core.device, descriptor_idx, 
						&((char*)p_material->uniform_buffers)[descriptor_offset], 
						&p_material->pipeline
					);
					shPipelineBindDescriptorSet(
						p_engine->core.p_graphics_commands[0].cmd_buffer, 
						descriptor_idx, VK_PIPELINE_BIND_POINT_GRAPHICS, 
						&p_material->pipeline
					);
				}
			}
		}

		//FOR DYNAMIC UNIFORMS AND MESH RELATED DATA
		for (uint32_t entity = 0; entity < p_scene->entity_count; entity++) {
			ShMesh* p_mesh = shGetShMesh(p_scene, entity);
			ShTransform* p_transform = shGetShTransform(p_scene, entity);

			if (shEntityInMaterial(entity, p_material)) {	
				
				//WRITE AND BIND DYNAMIC UNIFORMS
				for (uint32_t descriptor_idx = 0; descriptor_idx < p_material->pipeline.descriptor_count; descriptor_idx++) {//WRITE AND BIND DYNAMIC UNIFORMS 
					
					uint32_t descriptor_offset = shGetUniformOffset(p_material, descriptor_idx);
					
					if (p_material->pipeline.descriptor_set_layout_bindings[descriptor_idx].descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC) {
						
						//DYNAMIC EXTENSION STRUCTURES
						if (descriptor_offset == p_material->extensions.transform_uniform_offset && p_transform != NULL) {//TRANSFORM
							memcpy(
								&((char*)p_material->uniform_buffers)[p_material->extensions.transform_uniform_offset], 
								p_transform->model, 
								64
							);
						}
						else {//OTHER DYNAMIC STRUCTURES WHICH ARE NOT WRITTEN AS ENGINE EXTENSIONS
							memcpy(
								&((char*)p_material->uniform_buffers)[descriptor_offset],
								&((char*)p_material->p_material_clients[entity].p_uniform_parameters)[descriptor_offset],
								p_material->pipeline.descriptor_buffer_infos[descriptor_idx].range
							);
						}

						shPipelineWriteDynamicDescriptorBufferMemory(
							p_engine->core.device, 
							descriptor_idx, 
							&((char*)p_material->uniform_buffers)[descriptor_offset], 
							&p_material->pipeline
						);
						shPipelineBindDynamicDescriptorSet(
							p_engine->core.p_graphics_commands[0].cmd_buffer, 
							descriptor_idx, 
							VK_PIPELINE_BIND_POINT_GRAPHICS, 
							&p_material->pipeline
						);
					}
				}

				if (p_mesh != NULL) {
					if (p_mesh->mesh_info.vertex_count != 0) {
						if (p_mesh->mesh_info.flags & SH_MESH_SETUP_DYNAMIC_MESH) {
							shWriteVertexBufferMemory(
								p_engine->core.device,
								p_mesh->vertex_buffer_memory,
								0,
								p_mesh->mesh_info.vertex_count * sizeof(p_mesh->mesh_info.p_vertices[0]),
								p_mesh->mesh_info.p_vertices
							);
						}
						shBindVertexBuffer(p_engine->core.p_graphics_commands[0].cmd_buffer, 0, 0, &p_mesh->vertex_buffer);
					}
					if (p_mesh->mesh_info.index_count != 0) {
						if (p_mesh->mesh_info.flags & SH_MESH_SETUP_DYNAMIC_MESH) {
							shWriteIndexBufferMemory(
								p_engine->core.device,
								p_mesh->index_buffer_memory,
								0,
								p_mesh->mesh_info.index_count * sizeof(p_mesh->mesh_info.p_indices[0]),
								p_mesh->mesh_info.p_indices
							);
						}
						shBindIndexBuffer(p_engine->core.p_graphics_commands[0].cmd_buffer, 0, &p_mesh->index_buffer);
					}

					if (p_mesh->mesh_info.index_count != 0) {
						shDrawIndexed(p_engine->core.p_graphics_commands[0].cmd_buffer, p_mesh->mesh_info.index_count);
					}
					else if (p_mesh->mesh_info.index_count == 0 && p_mesh->mesh_info.vertex_count != 0) {
						shDraw(p_engine->core.p_graphics_commands[0].cmd_buffer, p_mesh->mesh_info.vertex_count / (p_mesh->mesh_info.vertex_stride / sizeof(float)));
					}
				}
			}
		}

		shEndPipeline(&p_material->pipeline);
	}
}

void shEndScene(ShEngine* p_engine) {

	vkDeviceWaitIdle(p_engine->core.device);

	for (uint32_t entity = 0; entity < p_engine->scene.entity_count; entity++) {

		ShIdentity* p_identity		= shGetShIdentity(&p_engine->scene, entity);
		ShMesh* p_mesh				= shGetShMesh(&p_engine->scene, entity);
		ShCamera* p_camera			= shGetShCamera(&p_engine->scene, entity);
		ShTransform* p_transform	= shGetShTransform(&p_engine->scene, entity);

		if (p_identity) {
		}

		if (p_mesh) {
			if ((size_t)p_mesh->vertex_buffer_memory != 0) {
				shClearBufferMemory(p_engine->core.device, p_mesh->vertex_buffer, p_mesh->vertex_buffer_memory);
			}
			if ((size_t)p_mesh->index_buffer_memory != 0) {
				shClearBufferMemory(p_engine->core.device, p_mesh->index_buffer, p_mesh->index_buffer_memory);
			}
			if (p_mesh->mesh_info.p_vertices != NULL) {
				free(p_mesh->mesh_info.p_vertices);
			}
			if (p_mesh->mesh_info.p_indices != NULL) {
				free(p_mesh->mesh_info.p_indices);
			}
		}

		if (p_camera) {
		}

		if (p_transform) {
		}
	}
	shSceneRelease(&p_engine->scene);
}

#ifdef __cplusplus
}
#endif//__cplusplus