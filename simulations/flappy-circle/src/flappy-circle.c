#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shengine/shEngine.h>
#include <shengine/shExport.h>

#include <shsharedhost/shSharedHost.h>

#include <shecs/shEcsImplementation.h>
#include <shecs/shComponents.h>

#define CAMERA_ENTITY 0
#define LOOP_OBJECT_ENTITY 1
#define BIRD_ENTITY 2

#include <stdlib.h>
#include <assert.h>

#define GRAVITY_FORCE -100.0f
#define FORCE_UNIT -800.0f //fat bird requires big force

typedef struct Bird {
	float y_force;
} Bird;
SH_ECS_MAKE_COMPONENT_DEFINITIONS(Bird, 10);

void bird_input(ShEngine* p_engine, const uint32_t bird_entity) {
	ShScene* p_scene = &p_engine->scene;
	ShTransform* p_transform = shGetShTransform(p_scene, bird_entity);
	Bird* p_bird = shGetBird(p_scene, bird_entity);
	
	p_transform->position[1] += p_bird->y_force * (float)p_engine->time.delta_time;

	if (shIsKeyPressed(p_engine->window, SH_KEY_SPACE)) {
		p_bird->y_force += FORCE_UNIT * (float)p_engine->time.delta_time;
	}

	p_bird->y_force -= GRAVITY_FORCE * (float)p_engine->time.delta_time;

	if (p_transform->position[1] >= 40 || p_transform->position[1] <= -40.0) { //nice collision detection dude
		shResetEngineState(p_engine, 0);
	}
}

void SH_ENGINE_EXPORT_FUNCTION flappy_circle_start(ShEngine* p_engine) {
	ShScene* p_scene = &p_engine->scene;
	for (uint32_t entity = 0; entity < (p_scene)->entity_count; entity++) {
		if (entity == BIRD_ENTITY) {
			shAddBird(p_scene, entity);
			break;
		}
	}
	
}

void SH_ENGINE_EXPORT_FUNCTION flappy_circle_update(ShEngine* p_engine) {
	ShScene* p_scene = &p_engine->scene;

	for (uint32_t entity = 0; entity < (p_scene)->entity_count; entity++) {
		ShTransform* p_transform = shGetShTransform(p_scene, entity);

		switch (entity) {
		case CAMERA_ENTITY:
			break;

		case LOOP_OBJECT_ENTITY:
			p_transform->rotation[2] += 10.0f * (float)p_engine->time.delta_time;
			break;

		case BIRD_ENTITY:
			bird_input(p_engine, entity);
			break;

		default: //for components linked to the main bird body
			memcpy(p_transform, shGetShTransform(p_scene, BIRD_ENTITY), sizeof(ShTransform));
		}
	}

}

void SH_ENGINE_EXPORT_FUNCTION flappy_circle_close(ShEngine* p_engine) {
	return;
}

#ifdef __cplusplus
}
#endif//__cplusplus
