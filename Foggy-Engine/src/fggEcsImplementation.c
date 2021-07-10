#include "fggEcsImplementation.h"
#include "fggComponents.h"

#include <cglm/cglm.h>

void fggSceneUpdate(const ezecsScene scene) {

	for (uint32_t entity = 0; entity < EZ_ECS_MAX_ENTITIES; entity++) {
		
		if (ezecsHasFggTransformComponent(scene, entity)) {
			FggTransform* t = ezecsGetFggTransformComponent(scene, entity);
			glm_mat4_identity(t->model);
			glm_scale(t->model, t->scale);
			glm_rotate(t->model, t->rotation[0] * GLM_PI / 180.0f, (vec3) { 1.0f, 0.0f, 0.0f });
			glm_rotate(t->model, t->rotation[1] * GLM_PI / 180.0f, (vec3) { 0.0f, 1.0f, 0.0f });
			glm_rotate(t->model, t->rotation[2] * GLM_PI / 180.0f, (vec3) { 0.0f, 0.0f, 1.0f });
			glm_translate(t->model, t->position);
		}
	}

}