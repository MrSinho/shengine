#include "shVkMemoryInfo.h"
#include "shVkPipelineData.h"

#include "shSceneHandle.h"
#include "shComponentEmbedded.h"
#include "shComponents.h"
#include "shProjection.h"
#include "shView.h"
#include "shDrawLoop.h"
#include "shEuler.h"
#include "shInput.h"
#include "shPhysicsInfo.h"

#include <string.h>


void shSceneInit(const ShVkCore core, ShScene* scene) {

	for (uint32_t entity = 0; entity < scene->entity_count; entity++) {
		if (shHasShMeshInfo(scene, entity)) {
			ShMeshInfo* mesh_info = shGetShMeshInfo(scene, entity);
			ShMesh* mesh = shAddShMesh(scene, entity);

			//Allocate memory
			if (!(mesh_info->flags & SH_MESH_SETUP_RUNTIME_MESH)) {
				if (mesh_info->vertex_count > 0 && mesh_info->p_vertices != NULL) {
					shCreateVertexBuffer(core, mesh_info, mesh);
					shAllocateVertexBuffer(core, mesh);
				}
				if (mesh_info->index_count > 0 && mesh_info->p_indices != NULL) {
					shCreateIndexBuffer(core, mesh_info, mesh);
					shAllocateIndexBuffer(core, mesh);
				}
			}

			//Map memory
			if (mesh_info->flags & SH_MESH_SETUP_STATIC_MESH) {
				if (mesh_info->vertex_count > 0 && mesh_info->p_vertices != NULL) {
					shMapVertexBufferMemory(core, mesh_info, mesh);
				}
				if (mesh_info->index_count > 0 && mesh_info->p_indices != NULL) {
					shMapIndexBufferMemory(core, mesh_info, mesh);
				}
			}
		}

		if (shHasShMaterialInfo(scene, entity)) {
			shSetupMaterial(core, *shGetShMaterialInfo(scene, entity), shAddShMaterial(scene, entity));
		}

		if (shHasShTransform(scene, entity)) {
			ShTransform* t = shGetShTransform(scene, entity);
			t->position[1] *= -1.0f;
			if (shHasShRigidBody(scene, entity)) {
				ShRigidBody* p_rb = shGetShRigidBody(scene, entity);
				p_rb->position = t->position;
			}
		}

	}

}

void shSceneUpdate(const ShVkCore core, const ShTime time, ShScene* scene) {

	ShCamera camera = { 0 };

	for (uint32_t entity = 0; entity < scene->entity_count; entity++) {

		if (shHasShTransform(scene, entity)) {
			ShTransform* t = shGetShTransform(scene, entity);
			glm_mat4_identity(t->model);
			glm_translate(t->model, t->position);
			glm_scale(t->model, t->scale);
			glm_rotate(t->model, glm_rad(t->rotation[0]), (vec3) { 1.0f, 0.0f, 0.0f });
			glm_rotate(t->model, glm_rad(t->rotation[1]), (vec3) { 0.0f, 1.0f, 0.0f });
			glm_rotate(t->model, glm_rad(t->rotation[2]), (vec3) { 0.0f, 0.0f, 1.0f });
			shDegreesToVector(t->rotation, t->front);

			glm_cross((vec3) { 0.0f, 1.0f, 0.0f }, t->front, t->left);
			glm_normalize(t->left);

			glm_cross(t->front, t->left, t->up);
			glm_normalize(t->up);

			if (shHasShCamera(scene, entity)) {
				camera = *shGetShCamera(scene, entity);
				if (camera.flags & SH_CAMERA_SETUP_FREE_FLIGHT_BIT) {
					vec3 displacement = { 0.0f, 0.0f, 0.0f };
					if (shIsKeyPressed(core.window, KEY_W)) {
						glm_vec3_copy(t->front, displacement);
						displacement[0] *= camera.speed;
						displacement[1] *= camera.speed;
						displacement[2] *= camera.speed;
					}
					if (shIsKeyPressed(core.window, KEY_A)) {
						glm_vec3_copy(t->left, displacement);
						displacement[0] *= camera.speed;
						displacement[1] *= camera.speed;
						displacement[2] *= camera.speed;
					}
					if (shIsKeyPressed(core.window, KEY_S)) {
						glm_vec3_copy(t->front, displacement);
						displacement[0] *= -1.0f * camera.speed;
						displacement[1] *= -1.0f * camera.speed;
						displacement[2] *= -1.0f * camera.speed;
					}
					if (shIsKeyPressed(core.window, KEY_D)) {
						glm_vec3_copy(t->left, displacement);
						displacement[0] *= -1.0f * camera.speed;
						displacement[1] *= -1.0f * camera.speed;
						displacement[2] *= -1.0f * camera.speed;
					}
					if (shIsKeyPressed(core.window, KEY_E)) {
						glm_vec3_copy(t->up, displacement);
						displacement[0] *= -1.0f * camera.speed;
						displacement[1] *= -1.0f * camera.speed;
						displacement[2] *= -1.0f * camera.speed;
					}
					if (shIsKeyPressed(core.window, KEY_Q)) {
						glm_vec3_copy(t->up, displacement);
						displacement[0] *= camera.speed;
						displacement[1] *= camera.speed;
						displacement[2] *= camera.speed;
					}
					displacement[0] *= 5.0f * (float)time.delta_time;
					displacement[1] *= 5.0f * (float)time.delta_time;
					displacement[2] *= 5.0f * (float)time.delta_time;
					glm_vec3_add(t->position, displacement, t->position);
					if (shIsMouseButtonPressed(core.window, MOUSE_BUTTON_RIGHT)) {
						glfwSetInputMode(core.window.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
						double dx, dy = 0.0;
						shMouseOffset(core.window, &dx, &dy);
						t->rotation[0] -= 0.6f * (float)dy * (float)time.delta_time;
						t->rotation[1] -= 0.6f * (float)dx * (float)time.delta_time;
						if (t->rotation[0] >= glm_rad(89.99999f)) {
							t->rotation[0] = glm_rad(89.99999f);
						}
						if (t->rotation[0] <= glm_rad(-89.99999f)) {
							t->rotation[0] = glm_rad(-89.99999f);
						}
					}
					else if (shIsMouseButtonReleased(core.window, MOUSE_BUTTON_RIGHT)) {
						glfwSetInputMode(core.window.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
					}
				}
				shSetProjection(core.window, camera.fov, camera.nc, camera.fc, camera.projection);
				shSetView(t->position, t->front, t->up, camera.view);
			}
		}

		if (shHasShMeshInfo(scene, entity) && shHasShMaterial(scene, entity)) {
			ShMaterial* material = shGetShMaterial(scene, entity);
			ShMeshInfo* mesh_info = shGetShMeshInfo(scene, entity);
			ShMesh* mesh = shGetShMesh(scene, entity);

			uint32_t uniform_buffer_index = 0;
			void* p_uniform_buffer = NULL;
			uint32_t push_constants_index = 0;
			void* p_push_constants = NULL;

			// push constants check
			if (material->pipeline_data.setupFlags & SH_PIPELINE_SETUP_PUSH_CONSTANTS_BIT) {
				p_push_constants = calloc(core.physical_device_properties.limits.maxPushConstantsSize, 1);
				if (p_push_constants == NULL) { break; }
				if (camera.flags != 0) {
					vec4* p_cam_const[2] = { camera.projection, camera.view };
					memcpy((void*)&((char*)p_push_constants)[push_constants_index], &p_cam_const[0][0], sizeof(mat4) * 2);
					push_constants_index += sizeof(mat4) * 2;
				}
			}
			// uniform buffer check
			if (material->pipeline_data.setupFlags & SH_PIPELINE_SETUP_UNIFORM_BUFFER_BIT) {
				if (shHasShTransform(scene, entity)) {
					p_uniform_buffer = calloc(material->pipeline_data.uniformBufferSize, 1);
					if (p_uniform_buffer == NULL) { break; }
					ShTransform* transform = shGetShTransform(scene, entity);
					memcpy((void*)&((char*)p_uniform_buffer)[uniform_buffer_index], transform->model, sizeof(mat4));
					uniform_buffer_index += sizeof(mat4);
				}
			}
			//int uniform_updated = 0;
			//if (material->p_main_material != NULL) {
			//	ShMaterial* p_main_material = material->p_main_material;
			//	uniform_updated = p_main_material->pipeline_data.uniform_updated;
			//	p_main_material->pipeline_data.uniform_updated = 1;
			//}
			if (p_uniform_buffer == NULL) { break; }
			if (p_push_constants == NULL) { break; }

			shRenderMesh(core, material->pipeline_data,
				push_constants_index, p_push_constants,
				uniform_buffer_index, p_uniform_buffer,
				mesh_info, mesh);


			if (p_uniform_buffer != NULL) {
				free(p_uniform_buffer);
			}
			if (p_push_constants != NULL) {
				free(p_push_constants);
			}
			push_constants_index = 0;
			uniform_buffer_index = 0;
		}



	}


}

void shSceneRelease(const ShVkCore core, ShScene* scene) {

	vkDeviceWaitIdle(core.device);

	for (uint32_t entity = 0; entity < scene->entity_count; entity++) {

		if (shHasShMesh(scene, entity)) {
			ShMesh* mesh = shGetShMesh(scene, entity);
			if (mesh->vertex_buffer_memory != NULL) {
				shClearBufferMemory(core.device, mesh->vertex_buffer, mesh->vertex_buffer_memory);
			}
			if (mesh->index_buffer_memory != NULL) {
				shClearBufferMemory(core.device, mesh->index_buffer, mesh->index_buffer_memory);
			}
			mesh = NULL;
		}

		if (shHasShMaterial(scene, entity)) {
			ShMaterial* mat = shGetShMaterial(scene, entity);
			shDestroyPipeline(core, &mat->pipeline_data);
			mat = NULL;
		}

	}
	shClearScene(scene);
}
