#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "shengine/shEngine.h"
#include "shengine/shInput.h"

#include "shengine/shLinearAlgebra.h"
#include "shenvironment/shEnvironment.h"

#include <string.h>
#include <cglm/cglm.h>



uint8_t shSceneInit(
	ShEngine* p_engine
) {
	shEngineError(p_engine == NULL, "shSceneInit: invalid engine memory", return 0);

	for (uint32_t entity = 0; entity < p_engine->scene_properties.entity_count; entity++) {

		ShIdentity*         p_identity           = &p_engine->scene_properties.p_identities          [entity];
		ShCamera*           p_camera             = &p_engine->scene_properties.p_cameras             [entity];
		ShTransform*        p_transform          = &p_engine->scene_properties.p_transforms          [entity];
		//ShHostMemoryLinker* p_host_memory_linker = &p_engine->scene_properties.p_host_memory_linkers [entity];
									       
		shEngineError(p_identity           == NULL, "shSceneInit: invalid identity memory",           return 0);
		shEngineError(p_camera             == NULL, "shSceneInit: invalid camera memory",             return 0);
		shEngineError(p_transform          == NULL, "shSceneInit: invalid transform memory",          return 0);
		//shEngineError(p_host_memory_linker == NULL, "shSceneInit: invalid host memory linker memory", return 0);

		p_transform->position[1] *= -1.0f;
		shUpdateTransform(p_transform);
		
	}

	return 1;
}

uint8_t shUpdateTransform(
	ShTransform* p_transform
) {
	shEngineError(p_transform == NULL, "shUpdateTransform: invalid transform memory", return 0);
    
    shmat4 model = { 0 };
    memcpy(model, &p_transform->model[0], sizeof(shmat4));

	glm_mat4_identity(model);
    glm_translate(model, &p_transform->position[0]);
    glm_scale(model, &p_transform->scale[0]);
	glm_rotate_x(model, p_transform->rotation[0], model);
	glm_rotate_y(model, p_transform->rotation[1], model);
	glm_rotate_z(model, p_transform->rotation[2], model);
	
    memcpy(&p_transform->model[0], model, sizeof(shmat4));

	shEulerToVector(&p_transform->rotation[0], &p_transform->front[0]);
	
    shvec3 p = { 0.0f, 1.0f, 0.0f };
	glm_vec3_cross(p, &p_transform->front[0], &p_transform->left[0]);
	glm_vec3_normalize(&p_transform->left[0]);
	
	glm_vec3_cross(&p_transform->front[0], &p_transform->left[0], &p_transform->up[0]);
	glm_vec3_normalize(&p_transform->up[0]);

	for (uint32_t i = 0; i < 4; i++) {
		p_transform->rotation[i] -= (float)((int)p_transform->rotation[i] / (int)(SH_DEGREES_TO_RADIANS(360.0f))) * SH_DEGREES_TO_RADIANS(360.0f);
	}
	p_transform->euler[0] = SH_RADIANS_TO_DEGREES(p_transform->rotation[0]);
	p_transform->euler[1] = SH_RADIANS_TO_DEGREES(p_transform->rotation[1]);
	p_transform->euler[2] = SH_RADIANS_TO_DEGREES(p_transform->rotation[2]);

	return 1;
}

uint8_t shUpdateCamera(
	ShEngine*    p_engine,
	ShTransform* p_transform,
	ShCamera*    p_camera
) {
	shEngineError(p_transform         == NULL, "shUpdateCamera: invalid transform memory", return 0);
	shEngineError(p_camera            == NULL, "shUpdateCamera: invalid camera memory",    return 0);

	if (p_camera->free_flight) {
		float displacement[3] = { 0.0f, 0.0f, 0.0f };
		if (shIsKeyDown(p_engine->window, SH_KEY_W)) {
			memcpy(displacement, p_transform->front, 12);
			glm_vec3_mul(displacement, (vec3){ p_camera->speed, p_camera->speed, p_camera->speed }, displacement);
		}
		if (shIsKeyDown(p_engine->window, SH_KEY_A)) {
			memcpy(displacement, p_transform->left, 12);
			glm_vec3_mul(displacement, (vec3) { p_camera->speed, p_camera->speed, p_camera->speed }, displacement);
		}
		if (shIsKeyDown(p_engine->window, SH_KEY_S)) {
			memcpy(displacement, p_transform->front, 12);
			glm_vec3_mul(displacement, (vec3) { -p_camera->speed, -p_camera->speed, -p_camera->speed }, displacement);
		}
		if (shIsKeyDown(p_engine->window, SH_KEY_D)) {
			memcpy(displacement, p_transform->left, 12);
			glm_vec3_mul(displacement, (vec3) { -p_camera->speed, -p_camera->speed, -p_camera->speed }, displacement);
		}
		if (shIsKeyDown(p_engine->window, SH_KEY_E)) {
			memcpy(displacement, p_transform->up, 12);
			glm_vec3_mul(displacement, (vec3) { -p_camera->speed, -p_camera->speed, -p_camera->speed }, displacement);
		}
		if (shIsKeyDown(p_engine->window, SH_KEY_Q)) {
			memcpy(displacement, p_transform->up, 12);
			glm_vec3_mul(displacement, (vec3) { p_camera->speed, p_camera->speed, p_camera->speed }, displacement);
		}
		glm_vec3_mul(displacement, (vec3) { (float)p_engine->time.delta_time, (float)p_engine->time.delta_time, (float)p_engine->time.delta_time }, displacement);
		glm_vec3_add(p_transform->position, displacement, p_transform->position);
		uint8_t gui_condition = 0;
		//if (p_engine->p_gui != NULL) {
		//	gui_condition = p_engine->p_gui->region_infos.cursor_on_any_region;
		//}
		if (shIsMouseButtonDown(p_engine->window, SH_MOUSE_BUTTON_RIGHT) && gui_condition == 0) {
			glfwSetInputMode(p_engine->window.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			p_transform->rotation[0] -= p_camera->mouse_speed / 100.0f * (float)p_engine->window.input.d_cursor_pos_y * (float)p_engine->time.delta_time;
			p_transform->rotation[1] -= p_camera->mouse_speed / 100.0f * (float)p_engine->window.input.d_cursor_pos_x * (float)p_engine->time.delta_time;
			if (p_transform->rotation[0] >= SH_DEGREES_TO_RADIANS(89.99999f)) {
				p_transform->rotation[0] = SH_DEGREES_TO_RADIANS(89.99999f);
			}
			if (p_transform->rotation[0] <= SH_DEGREES_TO_RADIANS(-89.99999f)) {
				p_transform->rotation[0] = SH_DEGREES_TO_RADIANS(-89.99999f);
			}
		}
		else if (shIsMouseButtonReleased(p_engine->window, SH_MOUSE_BUTTON_RIGHT)) {
			glfwSetInputMode(p_engine->window.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
    
    shmat4 projection = { 0 };
    shmat4 view       = { 0 };

	shSetProjection(p_engine->window, p_camera->fov, p_camera->nc, p_camera->fc, projection);
	shSetView(&p_transform->position[0], &p_transform->front[0], &p_transform->up[0], view);

    memcpy(p_camera->projection, projection, sizeof(shmat4));
    memcpy(p_camera->view      , view,       sizeof(shmat4));

	return 1;
}

uint8_t shSceneUpdate(
	ShEngine* p_engine
) {
	shEngineError(p_engine == NULL, "shSceneUpdate: invalid engine memory", return 0);

	for (uint32_t entity = 0; entity < p_engine->scene_properties.entity_count; entity++) {

		ShIdentity*         p_identity           = &p_engine->scene_properties.p_identities          [entity];
		ShCamera*           p_camera             = &p_engine->scene_properties.p_cameras             [entity];
		ShTransform*        p_transform          = &p_engine->scene_properties.p_transforms          [entity];
		//ShHostMemoryLinker* p_host_memory_linker = &p_engine->scene_properties.p_host_memory_linkers [entity];
									       
		shEngineError(p_identity           == NULL, "shSceneInit: invalid identity memory",           return 0);
		shEngineError(p_camera             == NULL, "shSceneInit: invalid camera memory",             return 0);
		shEngineError(p_transform          == NULL, "shSceneInit: invalid transform memory",          return 0);
		//shEngineError(p_host_memory_linker == NULL, "shSceneInit: invalid host memory linker memory", return 0);

		shUpdateCamera(p_engine, p_transform, p_camera);

		shUpdateTransform(p_transform);

	}

	return 1;
}

uint8_t shEndScene(
	ShEngine* p_engine
) {
	shEngineError(p_engine == NULL, "shEndScene: invalid engine memory", return 0);
	
	if (p_engine->scene_properties.p_identities          != NULL) { free(p_engine->scene_properties.p_identities         ); }
	if (p_engine->scene_properties.p_cameras             != NULL) { free(p_engine->scene_properties.p_cameras            ); }
	if (p_engine->scene_properties.p_transforms          != NULL) { free(p_engine->scene_properties.p_transforms         ); }
	//if (p_engine->scene_properties.p_host_memory_linkers != NULL) { free(p_engine->scene_properties.p_host_memory_linkers); }

	memset(&p_engine->scene_properties, 0, sizeof(ShSceneProperties));

	return 1;
}



#ifdef __cplusplus
}
#endif//__cplusplus
