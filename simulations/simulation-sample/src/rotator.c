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
    }
}


#include <string.h>

void SH_ENGINE_EXPORT_FUNCTION simulation_update(ShEngine* p_engine, const uint32_t entity) {
    ShIdentity* p_identity = shGetShIdentity(&p_engine->scene, entity);
    if (p_identity != NULL) {
        if (strcmp(p_identity->name, "rotator") == 0) {
            ShTransform* p_transform = shGetShTransform(&p_engine->scene, entity);
            p_transform->rotation[1] += 50.0f * (float)p_engine->time.delta_time;
            if (shIsKeyPressed(p_engine->window, SH_KEY_1)) {
                shGetShTransform(&p_engine->scene, entity)->position[1] += 5.0f * (float)p_engine->time.delta_time;
            }
            if (shIsKeyPressed(p_engine->window, SH_KEY_2)) {
                shGetShTransform(&p_engine->scene, entity)->position[1] -= 5.0f * (float)p_engine->time.delta_time;
            }
        }
        if (strcmp(p_identity->name, "circle") == 0) {
            ShMaterialHost* p_material = &p_engine->p_materials[2];
            //If the uniform parameter is not recognized by the engine as an official extension, you must directly change the material host uniform parameters
            //UNIFORM PARAMETERS OFFSET = sizeof(mat4) + sizeof(vec4) = 80
            float* p_red = (float*)&((char*)p_material->material_clients[entity].p_uniform_parameters)[80];
            if (shIsKeyPressed(p_engine->window, SH_KEY_Z)) {
                *p_red += 1.0f * (float)p_engine->time.delta_time;
            }
            else if (shIsKeyPressed(p_engine->window, SH_KEY_X)) {
                *p_red -= 1.0f * (float)p_engine->time.delta_time;
            }
        }
    }
    
}

void SH_ENGINE_EXPORT_FUNCTION simulation_close(ShEngine* p_engine, const uint32_t entity) {
    return;
}

#ifdef __cplusplus
}
#endif//__cplusplus
