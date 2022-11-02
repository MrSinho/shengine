#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shengine/shEngine.h>
#include <shengine/shExport.h>

#include <shecs/shIdentity.h>
#include <shecs/shTransform.h>

#include <stdio.h>
#include <string.h>

#include "shegui/shEgui.h"


//0 as return value: failure

uint8_t SH_ENGINE_EXPORT_FUNCTION sim_start(ShEngine* p_engine) {
    puts("SIMULATION IS RUNNING...");
    return shEngineGuiSetup(p_engine, 256, SH_GUI_THEME_EXTRA_DARK);
}

uint8_t SH_ENGINE_EXPORT_FUNCTION sim_update(ShEngine* p_engine) {
    //before recording cmd buffer
    ShGui* p_gui = p_engine->p_gui;

    if (p_gui == NULL) { return 0; }
    shGuiWindow(
        p_gui,
        250.0f,
        200.0f,
        0.0f,
        0.0f,
        "Simulation sample",
        SH_GUI_MOVABLE | SH_GUI_RESIZABLE | SH_GUI_PIXELS
    );
    shGuiWindowText(
        p_gui,
        SH_GUI_WINDOW_TEXT_SIZE,
        "Hey, this is a demo",
        SH_GUI_CENTER_WIDTH
    );
    shGuiWindowSeparator(p_gui);
    if (shGuiWindowButton(p_gui, SH_GUI_WINDOW_TEXT_SIZE, "Reset", SH_GUI_CENTER_WIDTH)) {
        puts("Reset");
        shResetEngineState(p_engine, 0);
        return 1;
    }
    shGuiWindowSeparator(p_gui);
    if (shGuiWindowButton(p_gui, SH_GUI_WINDOW_TEXT_SIZE, "Quit", SH_GUI_CENTER_WIDTH)) {
        shEngineShutdown(p_engine);
    }
	return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION sim_frame_update(ShEngine* p_engine) {
    //cmd buffer is recording

    for (uint32_t entity = 0; entity < p_engine->scene.entity_count; entity++) {
        ShIdentity* p_identity = shGetShIdentity(&p_engine->scene, entity);
        if (p_identity != NULL) {
            if (strcmp(p_identity->name, "rotator") == 0) {
                ShTransform* p_transform = shGetShTransform(&p_engine->scene, entity);
                p_transform->rotation[1] += 1.0f * (float)p_engine->time.delta_time;
                if (shIsKeyDown(p_engine->window, SH_KEY_1)) {
                    shGetShTransform(&p_engine->scene, entity)->position[1] += 5.0f * (float)p_engine->time.delta_time;
                }
                if (shIsKeyDown(p_engine->window, SH_KEY_2)) {
                    shGetShTransform(&p_engine->scene, entity)->position[1] -= 5.0f * (float)p_engine->time.delta_time;
                }
            }
            if (strcmp(p_identity->name, "circle") == 0) {
                ShMaterialHost* p_material = &p_engine->p_materials[2];
                //If the uniform parameter is not recognized by the engine as an official extension, you must directly change the material host uniform parameters
                //UNIFORM PARAMETERS OFFSET = sizeof(model_matrix considering dynamic offset alignment ) + sizeof(vec4) = 80
                float* p_red = (float*)&((char*)p_material->p_material_clients[entity].p_uniform_parameters)[p_material->pipeline.descriptor_buffer_infos[0].range + 16];
                if (shIsKeyDown(p_engine->window, SH_KEY_Z)) {
                    *p_red += 1.0f * (float)p_engine->time.delta_time;
                }
                else if (shIsKeyDown(p_engine->window, SH_KEY_X)) {
                    *p_red -= 1.0f * (float)p_engine->time.delta_time;
                }
            }
        }
    }
    return 1;
}

uint64_t SH_ENGINE_EXPORT_FUNCTION sim_thread(void* p_ext) {//void* ShEngine::p_engine_extension = NULL
    //puts("Simulation thread running, but there's nothing to do...");
    //
    //shThreadsSleep(2000);
    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION sim_update_pending(ShEngine* p_engine) {

    //shGuiText(
    //    p_engine->p_gui,
    //    SH_GUI_WINDOW_TEXT_SIZE * 2.0f,
    //    0.0f,
    //    0.0f,
    //    "DEMONSTRATION OF A LOADING THREAD",
    //    SH_GUI_CENTER_WIDTH | SH_GUI_CENTER_HEIGHT
    //);

    return 1;
}


uint8_t SH_ENGINE_EXPORT_FUNCTION sim_after_thread(ShEngine* p_engine) {
    
    puts("Simulation thread terminated");

    return 1;
}


uint8_t SH_ENGINE_EXPORT_FUNCTION sim_frame_resize(ShEngine* p_engine) {
    puts("Window resized");
    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION sim_close(ShEngine* p_engine, const uint32_t entity) {
    puts("Shutting down sim");
    return 1;
}



#ifdef SH_SIMULATION_TARGET_TYPE_EXECUTABLE

#include <sheditor/shEditor.h>

int main() {
    ShEngine engine = { 0 };
    engine.simulation_host.p_start          = &sim_start;
    engine.simulation_host.p_thread         = &sim_thread;
    engine.simulation_host.p_update_pending = &sim_update_pending;
    engine.simulation_host.p_after_thread   = &sim_after_thread;
    engine.simulation_host.p_update         = &sim_update;
    engine.simulation_host.p_frame_update   = &sim_frame_update;
    engine.simulation_host.p_frame_resize   = &sim_frame_resize;
    engine.simulation_host.p_close          = &sim_close;
    engine.window.title                     = "simulation sample";
    return shEditorMain(&engine);
}
#endif//SH_SIMULATION_TARGET_TYPE_EXECUTABLE

#ifdef __cplusplus
}
#endif//__cplusplus
