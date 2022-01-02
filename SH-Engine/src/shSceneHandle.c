#include "shVkMemoryInfo.h"
#include "shVkPipelineData.h"

#include "shSceneHandle.h"
#include "shComponentEmbedded.h"
#include "shComponents.h"

#include "shProjection.h"
#include "shView.h"
#include "shDrawLoop.h"
#include "shInput.h"
#include "shEuler.h"

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

void shSceneInit(const ShVkCore core, ShScene* scene) {

	for (uint32_t entity = 0; entity < scene->entity_count; entity++) {
		ShMeshInfo* mesh_info = shGetShMeshInfo(scene, entity);
		ShTransform* transform = shGetShTransform(scene, entity);

		if (mesh_info != NULL) {
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

		if (transform != NULL) {
			transform->position[1] *= -1.0f;
			shUpdateTransform(transform);
		}
	}

}

void shSceneUpdate(const ShVkCore core, const ShTime time, ShPhysicsHost* physics, ShScene* scene) {

	ShCamera camera = { 0 };

	for (uint32_t entity = 0; entity < scene->entity_count; entity++) {

		if (shHasShTransform(scene, entity)) {
			ShTransform* t = shGetShTransform(scene, entity);
				shUpdateTransform(t);
			
				shEulerToVector(t->rotation, t->front);
			
			glm_vec3_cross((vec3) { 0.0f, 1.0f, 0.0f }, t->front, t->left);
			glm_vec3_normalize(t->left);

			glm_vec3_cross(t->front, t->left, t->up);
			glm_vec3_normalize(t->up);

			if (shHasShCamera(scene, entity)) {
				camera = *shGetShCamera(scene, entity);
				if (camera.flags & SH_CAMERA_SETUP_FREE_FLIGHT_BIT) {
					float displacement[3] = { 0.0f, 0.0f, 0.0f };
					if (shIsKeyPressed(core.window, SH_KEY_W)) {
						memcpy(displacement, t->front, 12);
						displacement[0] *= camera.speed;
						displacement[1] *= camera.speed;
						displacement[2] *= camera.speed;
					}
					if (shIsKeyPressed(core.window, SH_KEY_A)) {
						memcpy(displacement, t->left, 12);
						displacement[0] *= camera.speed;
						displacement[1] *= camera.speed;
						displacement[2] *= camera.speed;
					}
					if (shIsKeyPressed(core.window, SH_KEY_S)) {
						memcpy(displacement, t->front, 12);
						displacement[0] *= -1.0f * camera.speed;
						displacement[1] *= -1.0f * camera.speed;
						displacement[2] *= -1.0f * camera.speed;
					}
					if (shIsKeyPressed(core.window, SH_KEY_D)) {
						memcpy(displacement, t->left, 12);
						displacement[0] *= -1.0f * camera.speed;
						displacement[1] *= -1.0f * camera.speed;
						displacement[2] *= -1.0f * camera.speed;
					}
					if (shIsKeyPressed(core.window, SH_KEY_E)) {
						memcpy(displacement, t->up, 12);
						displacement[0] *= -1.0f * camera.speed;
						displacement[1] *= -1.0f * camera.speed;
						displacement[2] *= -1.0f * camera.speed;
					}
					if (shIsKeyPressed(core.window, SH_KEY_Q)) {
						memcpy(displacement, t->up, 12);
						displacement[0] *= camera.speed;
						displacement[1] *= camera.speed;
						displacement[2] *= camera.speed;
					}
					displacement[0] *= 5.0f * (float)time.delta_time;
					displacement[1] *= 5.0f * (float)time.delta_time;
					displacement[2] *= 5.0f * (float)time.delta_time;
					glm_vec3_add(t->position, displacement, t->position);
					if (shIsMouseButtonPressed(core.window, SH_MOUSE_BUTTON_RIGHT)) {
						glfwSetInputMode(core.window.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
						double dx, dy = 0.0;
						shMouseOffset(core.window, &dx, &dy);
						t->rotation[0] -= 0.6f * (float)dy * (float)time.delta_time;
						t->rotation[1] -= 0.6f * (float)dx * (float)time.delta_time;
						if (t->rotation[0] >= SH_DEGREES_TO_RADIANS(89.99999f)) {
							t->rotation[0] = SH_DEGREES_TO_RADIANS(89.99999f);
						}
						if (t->rotation[0] <= SH_DEGREES_TO_RADIANS(-89.99999f)) {
							t->rotation[0] = SH_DEGREES_TO_RADIANS(-89.99999f);
						}
					}
					else if (shIsMouseButtonReleased(core.window, SH_MOUSE_BUTTON_RIGHT)) {
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

			void* p_push_constants[128];
			uint32_t push_constants_size = 0;
			if (material->pipeline_data.pushConstantRange.size != 0) {
				if (camera.flags != 0) {
					vec4* p_cam_const[2] = { camera.projection, camera.view };
					memcpy((void*)&((char*)p_push_constants)[push_constants_size], &p_cam_const[0][0], sizeof(mat4) * 2);
					push_constants_size += sizeof(mat4) * 2;
				}
			}
			
			void* p_uniform_buffers[64000];
			uint32_t uniform_buffers_size = 0;
			if (material->pipeline_data.uniform_buffer_count != 0) {
				ShTransform* transform = shGetShTransform(scene, entity);
				ShPhysicsClient* client = shGetShPhysicsClient(scene, entity);
				if (transform != NULL) {
					memcpy(&((char*)p_uniform_buffers)[uniform_buffers_size], transform->model, 64);
					uniform_buffers_size += 64;
				}
				if (client != NULL) {
					if (*client & SH_PHYSICS_CLIENT_ELECTROSTATICS) {
						memcpy(&((char*)&p_uniform_buffers)[uniform_buffers_size], 
							physics->electrostaticWorld.charges, 16 * 32);
						uniform_buffers_size += 16 * 32;
						float dtime[1] = { (float)time.delta_time };
						memcpy(&((char*)&p_uniform_buffers)[uniform_buffers_size], dtime, 16);
						uniform_buffers_size += 16;
					}
				}
			}

			shRenderMesh(core, material->pipeline_data, 
				push_constants_size, p_push_constants, 
				uniform_buffers_size, p_uniform_buffers,
				mesh_info, mesh);
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
