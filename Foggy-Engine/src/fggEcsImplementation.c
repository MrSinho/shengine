
#include "fggVkMemoryInfo.h"
#include "fggVkPipelineData.h"

#include "fggEcsImplementation.h"
#include "fggComponents.h"
#include "fggProjection.h"
#include "fggView.h"
#include "fggDrawLoop.h"

#include "fggCglmImplementation.h"


void fggSceneInit(const FggVkCore core, const FggVkFixedStates fixedStates, const ezecsScene scene) {

	for (uint32_t entity = 0; entity < EZ_ECS_MAX_ENTITIES; entity++) {
		if (ezecsHasFggMesh(scene, entity)) {
			FggMesh* m = ezecsGetFggMesh(scene, entity);
			fggAllocateMeshVertexData(core, m);
			fggAllocateMeshIndexData(core, m);
			if (m->flags & FGG_MESH_SETUP_STATIC_MESH) {
				fggMapVertexBufferMemory(core, m);
				fggMapIndexBufferMemory(core, m);
			}
		}

		if (ezecsHasFggMaterial(scene, entity)) {
			FggMaterial* m = ezecsGetFggMaterial(scene, entity);
		}

		if (ezecsHasFggTransform(scene, entity)) {
			FggTransform* t = ezecsGetFggTransform(scene, entity);
			t->position[1] *= -1.0f;
		}
	}

}

void fggSceneUpdate(const FggVkCore core, const ezecsScene scene) {

	for (uint32_t entity = 0; entity < EZ_ECS_MAX_ENTITIES; entity++) {
		
		if (ezecsHasFggMesh(scene, entity)) {
			FggMesh* mesh = ezecsGetFggMesh(scene, entity);
			fggBindVertexBuffers(core, *mesh);
			if (mesh->flags & FGG_MESH_SETUP_DYNAMIC_MESH) {
				fggMapVertexBufferMemory(core, mesh);
				fggMapIndexBufferMemory(core, mesh);
			}
			if (mesh->indexCount > 0 && mesh->indexBuffer != NULL) {
				fggBindIndexBuffers(core, *mesh);
			}
			if (ezecsHasFggMaterial(scene, entity)) {
				FggMaterial* mat = ezecsGetFggMaterial(scene, entity);
				fggBindPipeline(core.pCmdBuffers[0], mat->pipelineData);
				if (mat->pipelineData.setupFlags & FGG_PIPELINE_SETUP_PUSH_CONSTANTS_BIT) {
					fggPushConstants(core.pCmdBuffers[0], mat->pipelineData);
				}
				if (mat->pipelineData.setupFlags & FGG_PIPELINE_SETUP_UNIFORM_BUFFER_BIT) {
					fggBindDescriptorSets(core, mat->pipelineData);
				}
				fggDraw(core.pCmdBuffers[0], *mesh);
			}
		}

		if (ezecsHasFggTransform(scene, entity)) {
			FggTransform* t = ezecsGetFggTransform(scene, entity);
			glm_mat4_identity(t->model);
			glm_scale(t->model, t->scale);
			glm_rotate(t->model, t->rotation[0] * (float)GLM_PI / 180.0f, (vec3) { 1.0f, 0.0f, 0.0f });
			glm_rotate(t->model, t->rotation[1] * (float)GLM_PI / 180.0f, (vec3) { 0.0f, 1.0f, 0.0f });
			glm_rotate(t->model, t->rotation[2] * (float)GLM_PI / 180.0f, (vec3) { 0.0f, 0.0f, 1.0f });
			glm_translate(t->model, t->position);
			if (ezecsHasFggMaterial(scene, entity)) {
				FggMaterial* m = ezecsGetFggMaterial(scene, entity);
				fggMapMemory(core.device, m->pipelineData.uniformBufferMemory, sizeof(mat4), t->model);
			}

		}

	}

}

void fggSceneRelease(const FggVkCore core, const ezecsScene scene) {

	vkDeviceWaitIdle(core.device);

	for (uint32_t entity = 0; entity < EZ_ECS_MAX_ENTITIES; entity++) {

		if (ezecsHasFggMesh(scene, entity)) {
			FggMesh* mesh = ezecsGetFggMesh(scene, entity);
			fggClearBufferMemory(core.device, mesh->vertexBuffer, mesh->vertexBufferMemory);
			if (mesh->pVertices != NULL) { free(mesh->pVertices); }
			mesh->vertexCount = 0;
			if (mesh->indexCount > 0 && mesh->indexBuffer != NULL) {
				fggClearBufferMemory(core.device, mesh->indexBuffer, mesh->indexBufferMemory);
				free(mesh->pIndices);
				mesh->indexCount = 0;
			}
			mesh = NULL;
			if (ezecsHasFggMaterial(scene, entity)) {
				FggMaterial* mat = ezecsGetFggMaterial(scene, entity);
				fggDestroyPipeline(core, &mat->pipelineData);
				mat = NULL;
			}
		}

	}

}
