
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

		}

		if (ezecsHasFggMaterial(scene, entity)) {
			FggMaterial* m = ezecsGetFggMaterial(scene, entity);
			fggSetupGraphicsPipeline(core, fixedStates, m->pushConstantRange, &m->pipelineData);
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
			if (ezecsHasFggMaterial(scene, entity)) {
				FggMaterial* mat = ezecsGetFggMaterial(scene, entity);
				fggBindPipeline(core.pCmdBuffers[0], mat->pipelineData);
				fggPushConstants(core.pCmdBuffers[0], mat->pipelineData, mat->pushConstantRange, mat->ppPushConstantsData);
				fggDraw(core.pCmdBuffers[0], mat->pipelineData, *mesh);
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