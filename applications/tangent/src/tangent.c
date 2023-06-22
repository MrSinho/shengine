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


#define PI 3.1415926535f

typedef struct TangentData {

    float x_0;
    float x_scale;

    struct {
        uint8_t display_ui;
    } ui;

} TangentData;


uint8_t SH_ENGINE_EXPORT_FUNCTION tangent_start(ShEngine* p_engine) {
    p_engine->p_ext = calloc(1, sizeof(TangentData));
    shEngineError(p_engine->p_ext == NULL, "invalid tangent data properties memory", return 0);
    return shEngineGuiSetup(p_engine, 128, 1024, SH_GUI_THEME_DARK);
}

uint64_t SH_ENGINE_EXPORT_FUNCTION tangent_thread(TangentData* p_tangent_data) {//void* ShEngine::p_engine_extension = NULL
    p_tangent_data->x_0 = 0.0f;
    p_tangent_data->x_scale = 4.0f;
    p_tangent_data->ui.display_ui = 1;
    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION tangent_update_pending(ShEngine* p_engine) {

    return 1;
}


uint8_t SH_ENGINE_EXPORT_FUNCTION tangent_after_thread(ShEngine* p_engine) {
    
    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION tangent_update(ShEngine* p_engine) {
    //before recording cmd buffer
    ShGui* p_gui = p_engine->p_gui;

    TangentData* p_tangent_data = (TangentData*)p_engine->p_ext;

    if (shIsKeyPressed(p_engine->window, SH_KEY_H)) {
        p_tangent_data->ui.display_ui = p_tangent_data->ui.display_ui == 0;
    }

    if (p_tangent_data->ui.display_ui) {
        if (p_gui == NULL) { return 0; }
        shGuiWindow(
            p_gui,
            20.0f,
            30.0f,
            -80.0f,
            -70.0f,
            "colorful tangent",
            SH_GUI_MOVABLE | SH_GUI_RESIZABLE | SH_GUI_RELATIVE
        );

        char s_value[16];
        float pixel_x = p_engine->window.input.cursor_pos_x / (float)p_engine->window.width * 2.0f;
        float x_rad = p_tangent_data->x_scale * (p_tangent_data->x_0 + pixel_x);
        float k = 8 * x_rad / PI;

        sprintf(s_value, "x: %.3f rad", x_rad);
        if (!p_gui->region_infos.cursor_on_regions) {
            shGuiText(
                p_gui,
                SH_GUI_WINDOW_TEXT_SIZE,
                p_engine->window.input.cursor_pos_x,
                -p_engine->window.input.cursor_pos_y - SH_GUI_WINDOW_TEXT_SIZE,
                s_value,
                SH_GUI_CENTER_WIDTH | SH_GUI_CENTER_HEIGHT
            );
        }


        sprintf(s_value, "x0: %.3f", p_tangent_data->x_0);
        shGuiWindowText(
            p_gui,
            SH_GUI_WINDOW_TEXT_SIZE,
            s_value,
            0
        );
        sprintf(s_value, "x scale: %.3f", p_tangent_data->x_scale);
        shGuiWindowText(
            p_gui,
            SH_GUI_WINDOW_TEXT_SIZE,
            s_value,
            0
        );
        sprintf(s_value, "x: %.3f rad", x_rad);
        shGuiWindowText(
            p_gui,
            SH_GUI_WINDOW_TEXT_SIZE,
            s_value,
            0
        );
        sprintf(s_value, "k: %.3f", k);
        shGuiWindowText(
            p_gui,
            SH_GUI_WINDOW_TEXT_SIZE,
            s_value,
            0
        );
        
     
        shGuiText(
            p_gui,
            SH_GUI_WINDOW_TEXT_SIZE,
            10.0f,
            -10.0f,
            "Colorful tangent",
            SH_GUI_EDGE_LEFT | SH_GUI_EDGE_TOP
        );
    }

    float dtime = (float)p_engine->time.delta_time;
    if (shIsKeyDown(p_engine->window, SH_KEY_X)) {
        p_tangent_data->x_scale += 5.0f * dtime;
    }
    else if (shIsKeyDown(p_engine->window, SH_KEY_Z)) {
        p_tangent_data->x_scale -= 5.0f * dtime;
    }

    if (shIsKeyDown(p_engine->window, SH_KEY_D)) {
        p_tangent_data->x_0 += 1.0f * dtime;
    }
    else if (shIsKeyDown(p_engine->window, SH_KEY_A)) {
        p_tangent_data->x_0 -= 1.0f * dtime;
    }
    

	return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION tangent_frame_update(ShEngine* p_engine) {

    VkDevice device            = p_engine->core.device;
    VkCommandBuffer cmd_buffer = p_engine->core.p_graphics_commands[0].cmd_buffer;
    ShVkPipeline* p_pipeline   = &p_engine->p_materials[0].pipeline;

    TangentData* p_tangent_data = (TangentData*)p_engine->p_ext;

    if (p_pipeline->pipeline == NULL) {
        return 1;
    }

    shBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, p_pipeline);

    shPipelineUpdateDescriptorSets(device, p_pipeline);

    shPipelineWriteDescriptorBufferMemory(
        device,
        0,
        0,
        (uint32_t)p_pipeline->descriptor_buffer_infos[0].range,
        p_tangent_data,
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


uint8_t SH_ENGINE_EXPORT_FUNCTION tangent_frame_resize(ShEngine* p_engine) {
    
    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION tangent_close(ShEngine* p_engine, const uint32_t entity) {
    free(p_engine->p_ext);
    return 1;
}



#ifdef SH_APPLICATION_TARGET_TYPE_EXECUTABLE

#include <sheditor/shEditor.h>

int main() {
    ShEngine engine = { 0 };
    engine.application_host.p_start          = &tangent_start;
    engine.application_host.p_thread         = &tangent_thread;
    engine.application_host.p_update_pending = &tangent_update_pending;
    engine.application_host.p_after_thread   = &tangent_after_thread;
    engine.application_host.p_update         = &tangent_update;
    engine.application_host.p_frame_update   = &tangent_frame_update;
    engine.application_host.p_frame_resize   = &tangent_frame_resize;
    engine.application_host.p_close          = &tangent_close;
    engine.window.title                      = "tangent";
    return shEditorMain(&engine);
}
#endif//SH_APPLICATION_TARGET_TYPE_EXECUTABLE

#ifdef __cplusplus
}
#endif//__cplusplus
