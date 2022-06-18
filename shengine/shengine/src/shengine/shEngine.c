#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



#include <shengine/shEngine.h>
#include <shvulkan/shVkDrawLoop.h>
#include <shvulkan/shVkCore.h>

#ifdef _MSC_VER
#pragma warning (disable: 6011)
#endif//_MSC_VER


void shEngineSafeState(ShEngine* p_engine) {
    shEngineWarning(1, "running safe window");
    double input_dtime = 0.0;
    double input_last_time = 0.0;

    while (shIsWindowActive(p_engine->window.window)) {
        shUpdateWindow(p_engine);

        input_dtime = p_engine->time.now - input_last_time;
        if (input_dtime >= 2.0) {
            if (shIsKeyPressed(p_engine->window, SH_KEY_LEFT_CONTROL) && shIsKeyPressed(p_engine->window, SH_KEY_R)) {
                input_last_time = p_engine->time.now;
                shSetTime(0.0, &p_engine->time);
                input_last_time = 0.0;
                shEngineManageState(p_engine, shResetEngineState(p_engine));
                break;
            }
        }

        shFrameReset(&p_engine->core, 0);

        uint32_t image_index = 0;
        shFrameBegin(&p_engine->core, 0, &image_index);

        shFrameEnd(&p_engine->core, 0, image_index);
    }

    shVulkanRelease(&p_engine->core);
    exit(-1);
}

uint8_t shSetEngineState(ShEngine* p_engine) {
    shEngineError(p_engine == NULL, "invalid engine memory");
    uint8_t mat_r = shLoadMaterials(&p_engine->core, p_engine->materials_descriptor.path, &p_engine->material_count, &p_engine->p_materials);
    if (p_engine->p_materials == NULL || mat_r == 0) {
        return 0;
    }
    if (!shLoadScene(p_engine->scene_descriptor.path, &p_engine->p_materials, &p_engine->scene)) {
        return 0;
    }
    shSceneInit(p_engine, &p_engine->scene);
    shLoadSimulation(p_engine->simulation_descriptor.path, p_engine, &p_engine->simulation_host);
    shSimulationLoadSymbols(&p_engine->simulation_host);
    if (p_engine->simulation_host.shared != NULL) {
        shSimulationStart(p_engine);
    }
    return 1;
}

uint8_t shResetEngineState(ShEngine* p_engine) {
    shEngineError(p_engine == NULL, "invalid engine memory");
    shEngineRelease(p_engine);
    return shSetEngineState(p_engine);
}

void shEngineUpdateState(ShEngine* p_engine) {
    double input_dtime = 0.0;
    double input_last_time = 0.0;

    while (shIsWindowActive(p_engine->window.window)) {
        shUpdateWindow(p_engine);

        input_dtime = p_engine->time.now - input_last_time;
        if (input_dtime >= 2.0) {
            if (shIsKeyPressed(p_engine->window, SH_KEY_LEFT_CONTROL) && shIsKeyPressed(p_engine->window, SH_KEY_R)) {
                input_last_time = p_engine->time.now;
                shSetTime(0.0, &p_engine->time);
                input_last_time = 0.0;
                shEngineManageState(p_engine, shResetEngineState(p_engine));
                break;
            }
        }

        shFrameReset(&p_engine->core, 0);

        uint32_t image_index = 0;
        shFrameBegin(&p_engine->core, 0, &image_index);

        shSimulationUpdate(p_engine);

        shSceneUpdate(p_engine);

        shFrameEnd(&p_engine->core, 0, image_index);
    }

    shEngineRelease(p_engine);
    shVulkanRelease(&p_engine->core);
    exit(0);
}

void shEngineManageState(ShEngine* p_engine, const uint8_t ready) {
    if (ready) {
        shEngineUpdateState(p_engine);
    }
    else {
        shEngineSafeState(p_engine);
    }
}

void shEngineRelease(ShEngine* p_engine) {
    shEngineError(p_engine == NULL, "invalid engine memory");
    if (p_engine->simulation_host.shared != NULL) {
        shSimulationClose(p_engine);
        shSharedRelease(&p_engine->simulation_host.shared);
    }
    shMaterialsRelease(&p_engine->core, &p_engine->material_count, &p_engine->p_materials);
    shSceneRelease(p_engine);
}



#ifdef __cplusplus
}
#endif//__cplusplus