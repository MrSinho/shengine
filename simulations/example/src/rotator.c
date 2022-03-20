#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shengine/shEngine.h>
#include <shengine/shExport.h>

#include <shecs/shTransform.h>

void SH_ENGINE_EXPORT_FUNCTION rotate_entity(ShEngine* p_engine, const uint32_t entity) {
    ShTransform* p_transform = shGetShTransform(&p_engine->scene, entity);
    if (p_transform != NULL) {
        p_transform->rotation[0] += 50.0f * (float)p_engine->time.delta_time;
    }
}

#ifdef __cplusplus
}
#endif//__cplusplus
