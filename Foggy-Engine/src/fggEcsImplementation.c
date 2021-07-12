
#include "fggVkMemoryInfo.h"
#include "fggVkPipelineData.h"

#include "fggEcsImplementation.h"
#include "fggComponents.h"
#include "fggProjection.h"
#include "fggView.h"

#include <cglm/cglm.h>



void fggSceneInit(const FggVkCore core, const FggVkFixedStates fixedStates, ezecsScene scene) {

	for (uint32_t entity = 0; entity < EZ_ECS_MAX_ENTITIES; entity++) {
		if (ezecsHasFggMesh(scene, entity)) {

		}

		if (ezecsHasFggMaterial(scene, entity)) {
			FggMaterial* m = ezecsGetFggMaterial(scene, entity);
			fggSetupGraphicsPipeline(core, fixedStates, &m->pipelineData);
		}

		if (ezecsHasFggTransform(scene, entity)) {

		}

		if (ezecsHasFggCamera(scene, entity)) {

		}
	}

}

void fggSceneUpdate(const FggVkCore core, const mat4 projection, const ezecsScene scene) {

	FggCamera camera = { 0 };

	for (uint32_t entity = 0; entity < EZ_ECS_MAX_ENTITIES; entity++) {
		
		if (ezecsHasFggMesh(scene, entity)) {
			FggMesh* mesh = ezecsGetFggMesh(scene, entity);
			fggBindVertexBuffers(core, *mesh);
			if (ezecsHasFggMaterial(scene, entity)) {
				FggMaterial* mat = ezecsGetFggMaterial(scene, entity);
				fggBindPipeline(core, mat->pipelineData);
				void* pushConstants[2] = { projection, camera.view };
				fggPushConstants(core, mat->pipelineData, VK_SHADER_STAGE_VERTEX_BIT, sizeof(mat4) * 2, pushConstants[0]);
				fggDraw(core, mat->pipelineData, camera.view, projection, *mesh);
			}
		}

		if (ezecsHasFggTransform(scene, entity)) {
			FggTransform* t = ezecsGetFggTransform(scene, entity);
			glm_mat4_identity(t->model);
			glm_scale(t->model, t->scale);
			glm_rotate(t->model, t->rotation[0] * GLM_PI / 180.0f, (vec3) { 1.0f, 0.0f, 0.0f });
			glm_rotate(t->model, t->rotation[1] * GLM_PI / 180.0f, (vec3) { 0.0f, 1.0f, 0.0f });
			glm_rotate(t->model, t->rotation[2] * GLM_PI / 180.0f, (vec3) { 0.0f, 0.0f, 1.0f });
			glm_translate(t->model, t->position);

		}

		if (ezecsHasFggCamera(scene, entity)) {
			camera = *ezecsGetFggCamera(scene, entity);
			fggSetView(camera.view);
		}

	}

}