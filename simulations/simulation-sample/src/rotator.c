#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shengine/shEngine.h>
#include <shengine/shExport.h>

#include <shecs/shIdentity.h>
#include <shecs/shTransform.h>

#include <stdio.h>
#include <string.h>


#include <shgui/shgui.h>


uint8_t SH_ENGINE_EXPORT_FUNCTION simulation_start(ShEngine* p_engine) {
    printf("SIMULATION IS RUNNING...\n");
	return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION simulation_update(ShEngine* p_engine) {

    if (p_engine->p_gui == NULL) { return 0; }
    shGuiWindow(
        p_engine->p_gui,
        250.0f,
        100.0f,
        0.0f,
        0.0f,
        "Simulation sample",
        SH_GUI_MOVABLE | SH_GUI_RESIZABLE | SH_GUI_PIXELS
    );
    shGuiWindowText(
        p_engine->p_gui,
        SH_GUI_WINDOW_TEXT_SIZE,
        "Hey, this is a demo",
        SH_GUI_CENTER_WIDTH
    );
    shGuiWindowSeparator(p_engine->p_gui);
    if (shGuiWindowButton(p_engine->p_gui, SH_GUI_WINDOW_TEXT_SIZE, "Reset", SH_GUI_CENTER_WIDTH)) {
        puts("Reset");
        return 0;
    }

    if (shIsKeyPressed(p_engine->window, SH_KEY_P)) {
        puts("Pressed");
    }

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
                if (shIsKeyPressed(p_engine->window, SH_KEY_Z)) {
                    *p_red += 1.0f * (float)p_engine->time.delta_time;
                }
                else if (shIsKeyPressed(p_engine->window, SH_KEY_X)) {
                    *p_red -= 1.0f * (float)p_engine->time.delta_time;
                }
            }
        }
    }

	return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION simulation_frame_update(ShEngine* p_engine) {
    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION simulation_close(ShEngine* p_engine, const uint32_t entity) {
    return 1;
}

#ifdef __cplusplus
}
#endif//__cplusplus
