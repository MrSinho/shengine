#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shengine/shEngine.h>
#include <shengine/shExport.h>

#include <shecs/shIdentity.h>
#include <shecs/shTransform.h>

#include <stdio.h>

void SH_ENGINE_EXPORT_FUNCTION simulation_start(ShEngine* p_engine, const uint32_t entity) {
    if (entity == 0) {
        printf("SIMULATION IS RUNNING...\n");
        //ShTransform* p_transform = shGetShTransform(&p_engine->scene, entity);
        //if (p_transform != NULL) {
        //    p_transform->rotation[0] += 50.0f * (float)p_engine->time.delta_time;
        //}   
    }
}


#include <string.h>

void SH_ENGINE_EXPORT_FUNCTION simulation_update(ShEngine* p_engine, const uint32_t entity) {
    ShIdentity* p_identity = shGetShIdentity(&p_engine->scene, entity);
    if (p_identity != NULL) {
        if (strcmp(p_identity->name, "rotator") == 0) {
            ShTransform* p_transform = shGetShTransform(&p_engine->scene, entity);
            p_transform->rotation[1] += 50.0f * (float)p_engine->time.delta_time;
        }
    }
    if (p_engine->window.input.key_actions[SH_KEY_1] == SH_PRESS) {
        shGetShTransform(&p_engine->scene, 2)->position[1] += 5.0f * (float)p_engine->time.delta_time;
    }
    if (p_engine->window.input.key_actions[SH_KEY_2] == SH_PRESS) {
        shGetShTransform(&p_engine->scene, 2)->position[1] -= 5.0f * (float)p_engine->time.delta_time;
    }
}

#ifdef __cplusplus
}
#endif//__cplusplus
