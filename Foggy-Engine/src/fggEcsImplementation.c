#include "fggVkMemoryInfo.h"
#include "fggVkPipelineData.h"

#include "fggEcsImplementation.h"
#include "fggComponents.h"
#include "fggProjection.h"
#include "fggView.h"
#include "fggDrawLoop.h"
#include "FggEuler.h"
#include "fggInput.h"

#include <string.h>


void fggSceneInit(const FggVkCore core, const FggScene scene) {

	for (uint32_t entity = 0; entity < FGG_ECS_MAX_ENTITIES; entity++) {
		if (fggHasFggMesh(scene, entity)) {
			FggMesh* m = fggGetFggMesh(scene, entity);

			//Allocate memory
			if (!(m->flags & FGG_MESH_SETUP_RUNTIME_MESH)) {
				if (m->vertex_count > 0 && m->p_vertices != NULL) {
					fggAllocateMeshVertexData(core, m);
				}
				if (m->index_count > 0 && m->p_indices != NULL) {
					fggAllocateMeshIndexData(core, m);
				}
			}

			//Map memory
			if (m->flags & FGG_MESH_SETUP_STATIC_MESH) {
				if (m->vertex_count > 0 && m->p_vertices != NULL) {
					fggMapVertexBufferMemory(core, m);
				}
				if (m->index_count > 0 && m->p_indices != NULL) {
					fggMapIndexBufferMemory(core, m);
				}
			}
		}

		if (fggHasFggMaterial(scene, entity)) {
			FggMaterial* m = fggGetFggMaterial(scene, entity);
		}

		if (fggHasFggTransform(scene, entity)) {
			FggTransform* t = fggGetFggTransform(scene, entity);
		}
	}

}

void fggSceneUpdate(const FggVkCore core, const FggTime time, const FggScene scene) {

	FggCamera camera = { 0 };

	for (uint32_t entity = 0; entity < FGG_ECS_MAX_ENTITIES; entity++) {

		if (fggHasFggTransform(scene, entity)) {
			FggTransform* t = fggGetFggTransform(scene, entity);
			glm_mat4_identity(t->model);
			glm_translate(t->model, t->position);
			glm_scale(t->model, t->scale);
			glm_rotate(t->model, glm_rad(t->rotation[0]), (vec3) { 1.0f, 0.0f, 0.0f });
			glm_rotate(t->model, glm_rad(t->rotation[1]), (vec3) { 0.0f, 1.0f, 0.0f });
			glm_rotate(t->model, glm_rad(t->rotation[2]), (vec3) { 0.0f, 0.0f, 1.0f });
			fggDegreesToVector(t->rotation, t->front);
			
			glm_cross((vec3) { 0.0f, 1.0f, 0.0f }, t->front, t->left);
			glm_normalize(t->left);
			
			glm_cross(t->front, t->left, t->up);
			glm_normalize(t->up);

			if (fggHasFggCamera(scene, entity)) {
				camera = *fggGetFggCamera(scene, entity);

				if (camera.flags & FGG_CAMERA_SETUP_FREE_FLIGHT_BIT) {
					vec3 displacement = { 0.0f, 0.0f, 0.0f };
					if (fggIsKeyPressed(core.window, KEY_W)) {
						glm_vec3_copy(t->front, displacement);
					}
					if (fggIsKeyPressed(core.window, KEY_A)) {
						glm_vec3_copy(t->left, displacement);
					}
					if (fggIsKeyPressed(core.window, KEY_S)) {
						glm_vec3_copy(t->front, displacement);
						displacement[0] *= -1.0f;
						displacement[1] *= -1.0f;
						displacement[2] *= -1.0f;
					}
					if (fggIsKeyPressed(core.window, KEY_D)) {
						glm_vec3_copy(t->left, displacement);
						displacement[0] *= -1.0f;
						displacement[1] *= -1.0f;
						displacement[2] *= -1.0f;
					}
					if (fggIsKeyPressed(core.window, KEY_E)) {
						glm_vec3_copy(t->up, displacement);
						displacement[0] *= -1.0f;
						displacement[1] *= -1.0f;
						displacement[2] *= -1.0f;
					}
					if (fggIsKeyPressed(core.window, KEY_Q)) {
						glm_vec3_copy(t->up, displacement);
					}
					displacement[0] *= 5.0f * time.delta_time;
					displacement[1] *= 5.0f * time.delta_time;
					displacement[2] *= 5.0f * time.delta_time;
					glm_vec3_add(t->position, displacement, t->position);
				}
				if (fggIsMouseButtonPressed(core.window, MOUSE_BUTTON_RIGHT)) {
					glfwSetInputMode(core.window.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
					double dx, dy = 0.0;
					fggMouseOffset(core.window, &dx, &dy);
					t->rotation[0] -= 0.6f * (float)dy * time.delta_time;
					t->rotation[1] -= 0.6f * (float)dx * time.delta_time;
					if (t->rotation[0] >= glm_rad(89.99999f)) {
						t->rotation[0] = glm_rad(89.99999f);
					}
					if (t->rotation[0] <= glm_rad(-89.99999f)) {
						t->rotation[0] = glm_rad(-89.99999f);
					}
				}
				else if (fggIsMouseButtonReleased(core.window, MOUSE_BUTTON_RIGHT)) {
					glfwSetInputMode(core.window.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				}
				fggSetProjection(core.window, camera.fov, camera.nc, camera.fc, camera.projection);
				fggSetView(t->position, t->front, t->up, camera.view);
			}
		}

		if (fggHasFggMesh(scene, entity) && fggHasFggMaterial(scene, entity)) {
			FggMaterial* material = fggGetFggMaterial(scene, entity);
			FggMesh* mesh = fggGetFggMesh(scene, entity);

			uint32_t uniform_buffer_index = 0;
			void* p_uniform_buffer;
			uint32_t push_constants_index = 0;
			void* p_push_constants;

			// push constants check
			if (material->pipeline_data.setupFlags & FGG_PIPELINE_SETUP_UNIFORM_BUFFER_BIT) {
					p_push_constants = calloc(core.physical_device_properties.limits.maxPushConstantsSize, 1); if (camera.flags != 0) {
					if (p_push_constants == NULL) { break; }
					vec4* p_cam_const[2] = { camera.projection, camera.view };
					memcpy((void*)&((char*)p_push_constants)[push_constants_index], &p_cam_const[0][0], sizeof(mat4)*2);
					push_constants_index += sizeof(mat4)*2;
				}
			}
			// uniform buffer check
			if (material->pipeline_data.setupFlags & FGG_PIPELINE_SETUP_UNIFORM_BUFFER_BIT) {
				if (fggHasFggTransform(scene, entity)) {
					p_uniform_buffer = calloc(material->pipeline_data.uniformBufferSize, 1);
					if (p_uniform_buffer == NULL) { break; }
					FggTransform* transform = fggGetFggTransform(scene, entity);
					memcpy((void*)&((char*)p_uniform_buffer)[uniform_buffer_index], transform->model, sizeof(mat4));
					uniform_buffer_index += sizeof(mat4);
				}
			}
			//int uniform_updated = 0;
			//if (material->p_main_material != NULL) {
			//	FggMaterial* p_main_material = material->p_main_material;
			//	uniform_updated = p_main_material->pipeline_data.uniform_updated;
			//	p_main_material->pipeline_data.uniform_updated = 1;
			//}
			if (!p_uniform_buffer) { break; }
			if (!p_push_constants) { break; }

			fggRenderMesh(core, material->pipeline_data, 
				push_constants_index, p_push_constants,
				uniform_buffer_index, p_uniform_buffer,
				mesh);


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

void fggSceneRelease(const FggVkCore core, const FggScene scene) {

	vkDeviceWaitIdle(core.device);

	for (uint32_t entity = 0; entity < FGG_ECS_MAX_ENTITIES; entity++) {

		if (fggHasFggMesh(scene, entity)) {
			FggMesh* mesh = fggGetFggMesh(scene, entity);
			if (mesh->vertex_count > 0 && mesh->p_vertices != NULL) {
				fggClearBufferMemory(core.device, mesh->vertex_buffer, mesh->vertex_buffer_memory);
				free(mesh->p_vertices);
				mesh->p_vertices = NULL;
				mesh->vertex_count = 0;
			}
			if (mesh->index_count > 0) {
				fggClearBufferMemory(core.device, mesh->index_buffer, mesh->index_buffer_memory);
				free(mesh->p_indices);
				mesh->p_indices = NULL;
				mesh->index_count = 0;
			}
			mesh = NULL;
		}
		if (fggHasFggMaterial(scene, entity)) {
			FggMaterial* mat = fggGetFggMaterial(scene, entity);
			fggDestroyPipeline(core, &mat->pipeline_data);
			mat = NULL;
		}

	}

}