
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
			//free(m->pVertices);
			//free(m->pIndices);
		}

		if (ezecsHasFggMaterial(scene, entity)) {
			FggMaterial* m = ezecsGetFggMaterial(scene, entity);
		}

		if (ezecsHasFggTransform(scene, entity)) {

		}
	}

}

void fggSceneUpdate(const FggVkCore core, const ezecsScene scene) {

	for (uint32_t entity = 0; entity < EZ_ECS_MAX_ENTITIES; entity++) {
		
		if (ezecsHasFggMesh(scene, entity)) {
			FggMesh* mesh = ezecsGetFggMesh(scene, entity);
			fggBindVertexBuffers(core, *mesh);
			if (mesh->indexCount > 0 && mesh->indexBuffer != NULL) {
				fggBindIndexBuffers(core, *mesh);
			}
			if (ezecsHasFggMaterial(scene, entity)) {
				FggMaterial* mat = ezecsGetFggMaterial(scene, entity);
				fggBindPipeline(core.pCmdBuffers[0], mat->pipelineData);
				fggPushConstants(core.pCmdBuffers[0], mat->pipelineData, mat->pushConstantRange, mat->ppPushConstantsData);
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
				vkDestroyPipelineLayout(core.device, mat->pipelineData.mainPipelineLayout, NULL);
				vkDestroyPipeline(core.device, mat->pipelineData.pipeline, NULL);
				vkDestroyShaderModule(core.device, mat->pipelineData.pShaderModules[0], NULL);
				vkDestroyShaderModule(core.device, mat->pipelineData.pShaderModules[1], NULL);
				free(mat->pipelineData.pShaderStages);
				
				mat->pipelineData.shaderStageCount = 0;
				mat->pipelineData.shaderModuleCount = 0;
				mat = NULL;
			}
		}

	}

}
