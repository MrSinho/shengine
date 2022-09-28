#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shvulkan/shVkDrawLoop.h>

#include <shengine/shEngine.h>
#include <shengine/shExport.h>

#include <shegui/shEgui.h>

#include <shecs/shCamera.h>

#include <math.h>
#include <stdio.h>
#include <memory.h>


typedef struct Fractal {
    alignas(4) float zoom;
    alignas(8) float rotation[2];

    struct {
        uint8_t display_ui;
    } ui;

} Fractal;


uint8_t SH_ENGINE_EXPORT_FUNCTION noise_start(ShEngine* p_engine) {
    p_engine->p_ext = calloc(1, sizeof(Fractal));
    shEngineError(p_engine->p_ext == NULL, "invalid fractal properties memory", return 0);
    return shEngineGuiSetup(p_engine, 256, SH_GUI_THEME_DARK);
}

uint64_t SH_ENGINE_EXPORT_FUNCTION noise_thread(Fractal* p_fractal) {//void* ShEngine::p_engine_extension = NULL
    p_fractal->zoom = 1.0f;
    p_fractal->rotation[0] = 1.0f;
    p_fractal->rotation[1] = 1.0f;
    p_fractal->ui.display_ui = 1;
    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION noise_update_pending(ShEngine* p_engine) {

    return 1;
}


uint8_t SH_ENGINE_EXPORT_FUNCTION noise_after_thread(ShEngine* p_engine) {
    
    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION noise_update(ShEngine* p_engine) {
    //before recording cmd buffer
    ShGui* p_gui = p_engine->p_gui;

    Fractal* p_fractal = (Fractal*)p_engine->p_ext;

    if (shIsKeyPressed(p_engine->window, SH_KEY_H)) {
        p_fractal->ui.display_ui = p_fractal->ui.display_ui == 0;
    }

    if (p_fractal->ui.display_ui) {
        if (p_gui == NULL) { return 0; }
        shGuiWindow(
            p_gui,
            25.0f,
            15.0f,
            -75.0f,
            -85.0f,
            "Fractal demo",
            SH_GUI_MOVABLE | SH_GUI_RESIZABLE | SH_GUI_RELATIVE
        );
        if (shGuiWindowButton(p_gui, SH_GUI_WINDOW_TEXT_SIZE, "Reset", SH_GUI_CENTER_WIDTH)) {
            puts("Reset");
            shResetEngineState(p_engine, 0);
            return 1;
        }
        shGuiWindowSeparator(p_gui);
        if (shGuiWindowButton(p_gui, SH_GUI_WINDOW_TEXT_SIZE, "Quit", SH_GUI_CENTER_WIDTH)) {
            shEngineShutdown(p_engine);
        }

        shGuiText(
            p_gui,
            SH_GUI_WINDOW_TEXT_SIZE,
            10.0f,
            -10.0f,
            "Fractal",
            SH_GUI_EDGE_LEFT | SH_GUI_EDGE_TOP
        );

        char s_pixel_coords[16];
        sprintf(s_pixel_coords, "%.2f %.2f", 
            p_engine->window.input.cursor_pos_x / (float)p_engine->window.width * 2.0f, 
            p_engine->window.input.cursor_pos_y / (float)p_engine->window.height * 2.0f
        );
        shGuiText(
            p_gui,
            SH_GUI_WINDOW_TEXT_SIZE,
            p_engine->window.input.cursor_pos_x,
            -p_engine->window.input.cursor_pos_y - SH_GUI_WINDOW_TEXT_SIZE,
            s_pixel_coords,
            SH_GUI_CENTER_WIDTH | SH_GUI_CENTER_HEIGHT
        );
    }

    float dtime = (float)p_engine->time.delta_time;
    if (shIsKeyDown(p_engine->window, SH_KEY_Z)) {
        p_fractal->zoom += 1.0f * dtime;
    }
    else if (shIsKeyDown(p_engine->window, SH_KEY_X)) {
        p_fractal->zoom -= 1.0f * dtime;
    }

    if (shIsKeyDown(p_engine->window, SH_KEY_A)) {
        p_fractal->rotation[0] += 1.0f * dtime;
    }
    else if(shIsKeyDown(p_engine->window, SH_KEY_D)) {
        p_fractal->rotation[0] -= 1.0f * dtime;
    }

    if (shIsKeyDown(p_engine->window, SH_KEY_W)) {
        p_fractal->rotation[1] += 1.0f * dtime;
    }
    else if (shIsKeyDown(p_engine->window, SH_KEY_S)) {
        p_fractal->rotation[1] -= 1.0f * dtime;
    }

	return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION noise_frame_update(ShEngine* p_engine) {

    VkDevice device = p_engine->core.device;
    VkCommandBuffer cmd_buffer = p_engine->core.p_graphics_commands[0].cmd_buffer;
    ShVkPipeline* p_pipeline = &p_engine->p_materials[0].pipeline;

    Fractal* p_fractal = (Fractal*)p_engine->p_ext;

    if (p_pipeline->pipeline == NULL) {
        return 1;
    }

    shBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, p_pipeline);

    //{//PROJECTION AND VIEW MATRICES
    //    ShCamera* p_camera = shGetShCamera(&p_engine->scene, 0);
    //    shEngineError(p_camera == NULL, "invalid camera memory", return 0);
    //
    //    shPipelinePushConstants(cmd_buffer, p_camera->projection, p_pipeline);
    //}//PROJECTION AND VIEW MATRICES

    shPipelineUpdateDescriptorSets(device, p_pipeline);

    shPipelineWriteDescriptorBufferMemory(
        device,
        0,
        p_fractal,
        p_pipeline
    );

    shPipelineBindDescriptorSets(
        cmd_buffer,
        0,
        1,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        p_pipeline
    );

    shDraw(
        cmd_buffer,
        6
    );

    shEndPipeline(p_pipeline);

    return 1;
}


uint8_t SH_ENGINE_EXPORT_FUNCTION noise_frame_resize(ShEngine* p_engine) {
    
    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION noise_close(ShEngine* p_engine, const uint32_t entity) {
    free(p_engine->p_ext);
    return 1;
}

#ifdef __cplusplus
}
#endif//__cplusplus
