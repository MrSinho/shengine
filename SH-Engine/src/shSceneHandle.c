#include "shVkMemoryInfo.h"
#include "shVkPipelineData.h"

#include "shSceneHandle.h"
#include "shComponentEmbedded.h"
#include "shComponents.h"

#include "shProjection.h"
#include "shView.h"
#include "shDrawLoop.h"
#include "shInput.h"

#include "shLinearAlgebra.h"
#include "shMatrix.h"
#include "shEuler.h"
#include "shVector.h"
#include "shPhysicsInfo.h"

#include <string.h>

void shUpdateTransform(ShTransform* t) {
	glm_mat4_identity(t->model);
	shMat4Translate(t->model, t->position, t->model);
	shMat4Scale(t->model, t->scale, t->model);
	shMat4Rotate(t->model, SH_DEGREES_TO_RADIANS(t->rotation[0]), (shvec3) { 1.0f, 0.0f, 0.0f }, t->model);
	shMat4Rotate(t->model, SH_DEGREES_TO_RADIANS(t->rotation[1]), (shvec3) { 0.0f, 1.0f, 0.0f }, t->model);
	shMat4Rotate(t->model, SH_DEGREES_TO_RADIANS(t->rotation[2]), (shvec3) { 0.0f, 0.0f, 1.0f }, t->model);
}

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
			shUpdateTransform(t);
			if (shHasShRigidBody(scene, entity)) {
				ShRigidBody* p_rb = shGetShRigidBody(scene, entity);
				memcpy(p_rb->transform, t->model, sizeof(shmat4));
			}
		}

	}

}

void shSceneUpdate(const ShVkCore core, const ShTime time, ShScene* scene) {

	ShCamera camera = { 0 };

	for (uint32_t entity = 0; entity < scene->entity_count; entity++) {

		if (shHasShTransform(scene, entity)) {
			ShTransform* t = shGetShTransform(scene, entity);
			if (shHasShRigidBody(scene, entity)) {
				ShRigidBody* p_rb = shGetShRigidBody(scene, entity);
				//printf("pos %f\n", p_rb->transform[3][1]);
				memcpy(t->model, p_rb->transform, sizeof(shmat4));
			}
			else {
				shUpdateTransform(t);
			}
			
			shEulerToVector(t->rotation, t->front);
			
			shVec3Cross((shvec3) { 0.0f, 1.0f, 0.0f }, t->front, t->left);
			shVec3Normalize(t->left, t->left);

			shVec3Cross(t->front, t->left, t->up);
			shVec3Normalize(t->up, t->up);

			if (shHasShCamera(scene, entity)) {
				camera = *shGetShCamera(scene, entity);
				if (camera.flags & SH_CAMERA_SETUP_FREE_FLIGHT_BIT) {
					shvec3 displacement = { 0.0f, 0.0f, 0.0f };
					if (shIsKeyPressed(core.window, SH_KEY_W)) {
						memcpy(displacement, t->front, sizeof(shvec3));
						displacement[0] *= camera.speed;
						displacement[1] *= camera.speed;
						displacement[2] *= camera.speed;
					}
					if (shIsKeyPressed(core.window, SH_KEY_A)) {
						memcpy(displacement, t->left, sizeof(shvec3));
						displacement[0] *= camera.speed;
						displacement[1] *= camera.speed;
						displacement[2] *= camera.speed;
					}
					if (shIsKeyPressed(core.window, SH_KEY_S)) {
						memcpy(displacement, t->front, sizeof(shvec3));
						displacement[0] *= -1.0f * camera.speed;
						displacement[1] *= -1.0f * camera.speed;
						displacement[2] *= -1.0f * camera.speed;
					}
					if (shIsKeyPressed(core.window, SH_KEY_D)) {
						memcpy(displacement, t->left, sizeof(shvec3));
						displacement[0] *= -1.0f * camera.speed;
						displacement[1] *= -1.0f * camera.speed;
						displacement[2] *= -1.0f * camera.speed;
					}
					if (shIsKeyPressed(core.window, SH_KEY_E)) {
						memcpy(displacement, t->up, sizeof(shvec3));
						displacement[0] *= -1.0f * camera.speed;
						displacement[1] *= -1.0f * camera.speed;
						displacement[2] *= -1.0f * camera.speed;
					}
					if (shIsKeyPressed(core.window, SH_KEY_Q)) {
						memcpy(displacement, t->up, sizeof(shvec3));
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

			uint32_t push_constants_index = 0;
			void* p_push_constants = NULL;

			// push constants check
			if (material->pipeline_data.pushConstantRange.size != 0) {
				p_push_constants = calloc(core.physical_device_properties.limits.maxPushConstantsSize, 1);
				if (p_push_constants == NULL) { break; }
				if (camera.flags != 0) {
					vec4* p_cam_const[2] = { camera.projection, camera.view };
					memcpy((void*)&((char*)p_push_constants)[push_constants_index], &p_cam_const[0][0], sizeof(mat4) * 2);
					push_constants_index += sizeof(mat4) * 2;
				}
			}
			// uniform buffers check
			void* p_uniform_buffers = NULL;
			uint32_t uniform_buffers_size = 0;
			uint32_t uniform_idx = 0;
			if (shHasShTransform(scene, entity)) {
				uniform_buffers_size += sizeof(shmat4);
				uniform_idx++;
			}
			for (; uniform_idx < material->pipeline_data.uniform_buffer_count; uniform_idx++) {
				uniform_buffers_size += material->pipeline_data.p_uniform_buffers[uniform_idx].uniform_buffer_size;
			}
			p_uniform_buffers = calloc(uniform_buffers_size, 1);
			if (p_uniform_buffers != NULL) {
				for (uint32_t i = 0; i < material->pipeline_data.uniform_buffer_count; i++) {
					if (shHasShTransform(scene, entity)) {
						memcpy(p_uniform_buffers, shGetShTransform(scene, entity)->model, sizeof(shmat4));
					}
				}
			}

			shRenderMesh(core, material->pipeline_data, 
				push_constants_index, p_push_constants, 
				uniform_buffers_size, p_uniform_buffers, 
				mesh_info, mesh);


			if (p_uniform_buffers != NULL) { free(p_uniform_buffers); }
			if (p_push_constants != NULL) { free(p_push_constants); }
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
