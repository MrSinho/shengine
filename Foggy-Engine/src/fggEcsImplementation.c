#include "fggVkMemoryInfo.h"
#include "fggVkPipelineData.h"

#include "fggEcsImplementation.h"
#include "fggComponents.h"
#include "fggProjection.h"
#include "fggView.h"
#include "fggDrawLoop.h"
#include "FggEuler.h"
#include "fggInput.h"

#include "fggCglmImplementation.h"


void fggSceneInit(const FggVkCore core, const ezecsScene scene) {

	for (uint32_t entity = 0; entity < EZ_ECS_MAX_ENTITIES; entity++) {
		if (ezecsHasFggMesh(scene, entity)) {
			FggMesh* m = ezecsGetFggMesh(scene, entity);

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

		if (ezecsHasFggMaterial(scene, entity)) {
			FggMaterial* m = ezecsGetFggMaterial(scene, entity);
		}

		if (ezecsHasFggTransform(scene, entity)) {
			FggTransform* t = ezecsGetFggTransform(scene, entity);
		}
	}

}

void fggSceneUpdate(const FggVkCore core, const FggTime time, const ezecsScene scene) {

	FggCamera camera = { 0 };

	for (uint32_t entity = 0; entity < EZ_ECS_MAX_ENTITIES; entity++) {

		if (ezecsHasFggTransform(scene, entity)) {
			FggTransform* t = ezecsGetFggTransform(scene, entity);
			glm_mat4_identity(t->model);
			glm_translate(t->model, t->position);
			glm_scale(t->model, t->scale);
			glm_rotate(t->model, glm_rad(t->rotation[0]), (vec3) { 1.0f, 0.0f, 0.0f });
			glm_rotate(t->model, glm_rad(t->rotation[1]), (vec3) { 0.0f, 1.0f, 0.0f });
			glm_rotate(t->model, glm_rad(t->rotation[2]), (vec3) { 0.0f, 0.0f, 1.0f });
			if (ezecsHasFggMaterial(scene, entity)) {
				FggMaterial* m = ezecsGetFggMaterial(scene, entity);
				fggMapMemory(core.device, m->pipeline_data.uniformBufferMemory, sizeof(mat4), t->model);
			}
			fggDegreesToVector(t->rotation, t->front);
			
			glm_cross((vec3) { 0.0f, 1.0f, 0.0f }, t->front, t->left);
			glm_normalize(t->left);
			
			glm_cross(t->front, t->left, t->up);
			glm_normalize(t->up);

			if (ezecsHasFggCamera(scene, entity)) {
				camera = *ezecsGetFggCamera(scene, entity);

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

		if (ezecsHasFggMesh(scene, entity)) {
			FggMesh* mesh = ezecsGetFggMesh(scene, entity);

			//Map memory
			if (mesh->flags & FGG_MESH_SETUP_DYNAMIC_MESH) {
				if (mesh->flags & FGG_MESH_SETUP_RUNTIME_MESH) {
					if (mesh->vertex_count >= 0 && mesh->p_vertices != NULL) {
						fggAllocateMeshVertexData(core, mesh);
					}
					if (mesh->index_count >= 0 && mesh->p_indices != NULL) {
						fggAllocateMeshIndexData(core, mesh);
					}
				}
				fggMapVertexBufferMemory(core, mesh);
				if (mesh->index_count > 0) {
					fggMapIndexBufferMemory(core, mesh);
				}
			}

			
			//Bind vertex and index buffers
			if (mesh->vertex_count > 0) {
				fggBindVertexBuffers(core, *mesh);
			}
			if (mesh->index_count > 0) {
				fggBindIndexBuffers(core, *mesh);
			}

			if (ezecsHasFggMaterial(scene, entity)) {
				FggMaterial* mat = ezecsGetFggMaterial(scene, entity);
				fggBindPipeline(core.p_cmd_buffers[0], mat->pipeline_data);

				//push constants
				vec4* pConst[2] = { camera.projection, camera.view };
				fggPushConstants(core.p_cmd_buffers[0], mat->pipeline_data, &pConst[0][0]);

				//Bind descriptor sets
				if (mat->pipeline_data.setupFlags & FGG_PIPELINE_SETUP_UNIFORM_BUFFER_BIT) {
					fggBindDescriptorSets(core, mat->pipeline_data);
				}
				fggDraw(core.p_cmd_buffers[0], mat->pipeline_data.vertexStride / 4, *mesh);
			}

			if (mesh->flags & FGG_MESH_SETUP_DYNAMIC_MESH & FGG_MESH_SETUP_RUNTIME_MESH) {
				if (mesh->vertex_count >= 0 && mesh->p_vertices != NULL) {
					fggClearBufferMemory(core.device, mesh->vertex_buffer, mesh->vertex_buffer_memory);
				}
				if (mesh->index_count >= 0 && mesh->p_indices != NULL) {
					fggClearBufferMemory(core.device, mesh->index_buffer, mesh->index_buffer_memory);
				}
			}
		}

	}

}

void fggSceneRelease(const FggVkCore core, const ezecsScene scene) {

	vkDeviceWaitIdle(core.device);

	for (uint32_t entity = 0; entity < EZ_ECS_MAX_ENTITIES; entity++) {

		if (ezecsHasFggMesh(scene, entity)) {
			FggMesh* mesh = ezecsGetFggMesh(scene, entity);
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
			if (ezecsHasFggMaterial(scene, entity)) {
				FggMaterial* mat = ezecsGetFggMaterial(scene, entity);
				fggDestroyPipeline(core, &mat->pipeline_data);
				mat = NULL;
			}
		}

	}

}