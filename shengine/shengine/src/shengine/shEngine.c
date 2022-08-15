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
            if (shIsKeyDown(p_engine->window, SH_KEY_LEFT_CONTROL) && shIsKeyPressed(p_engine->window, SH_KEY_R)) {
                input_last_time = p_engine->time.now;
                shSetTime(0.0, &p_engine->time);
                input_last_time = 0.0;
                shEngineManageState(p_engine, shResetEngineState(p_engine, 1));
                break;
            }
        }

        shFrameReset(&p_engine->core, 0);

        uint32_t image_index = 0;
        shFrameBegin(&p_engine->core, 0, (VkClearColorValue) { 0.0f, 0.0f, 0.0f, 1.0f }, & image_index);

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
    if (!shLoadScene(p_engine->scene_descriptor.path, p_engine->material_count, &p_engine->p_materials, &p_engine->scene)) {
        return 0;
    }
    shSceneInit(p_engine, &p_engine->scene);
    
    ShGuiCore gui_core = {
        p_engine->core.device,
        p_engine->core.physical_device,
        p_engine->core.graphics_queue,
        p_engine->core.p_graphics_commands[0].cmd_buffer,
        p_engine->core.p_graphics_commands[0].fence,
        p_engine->core.surface.surface,
        p_engine->core.render_pass
    };
    (p_engine->p_gui == NULL) && (p_engine->p_gui = shGuiInit(gui_core));

    shGuiLinkInputs(
        &p_engine->window.width,
        &p_engine->window.height,
        &p_engine->window.input.cursor_pos_x,
        &p_engine->window.input.cursor_pos_y,
        p_engine->window.input.key_events,
        p_engine->window.input.mouse_events,
        p_engine->window.default_cursor_icons,
        &p_engine->time.delta_time,
        p_engine->p_gui
    );

    shGuiBuildRegionPipeline(p_engine->p_gui, 256);
    shGuiBuildTextPipeline(p_engine->p_gui, 256);
    shGuiSetDefaultValues(p_engine->p_gui, SH_GUI_THEME_DARK, SH_GUI_RECORD | SH_GUI_INITIALIZE);

    shLoadSimulation(p_engine->simulation_descriptor.path, p_engine, &p_engine->simulation_host);
    shSimulationLoadSymbols(&p_engine->simulation_host);

    if (p_engine->simulation_host.shared != NULL) {
        return shSharedSceneRun(p_engine, p_engine->simulation_host.p_start);
    }

    return 1;
}

uint8_t shResetEngineState(ShEngine* p_engine, const uint8_t release_shared) {
    shEngineError(p_engine == NULL, "invalid engine memory");
    shEngineRelease(p_engine, release_shared);
    return shSetEngineState(p_engine);
}

void shEngineUpdateState(ShEngine* p_engine) {
    double input_dtime = 0.0;
    double input_last_time = 0.0;

    while (shIsWindowActive(p_engine->window.window)) {    
        
        uint32_t width = p_engine->window.width;
        uint32_t height = p_engine->window.height;
        shGetWindowSize(&p_engine->window);
        if (width != p_engine->window.width || height != p_engine->window.height) {
            shWaitDeviceIdle(p_engine->core.device);

            shRenderPassRelease(&p_engine->core);
            shSwapchainRelease(&p_engine->core);
            shSurfaceRelease(&p_engine->core);
            shDepthBufferRelease(&p_engine->core);

            shWindowCreateSurface(p_engine);
            shInitSwapchainData(&p_engine->core);
            shInitDepthData(&p_engine->core);
            shCreateRenderPass(&p_engine->core);
            shSetFramebuffers(&p_engine->core);

            shMaterialsRelease(&p_engine->core, &p_engine->material_count, &p_engine->p_materials);
            uint8_t mat_r = shLoadMaterials(&p_engine->core, p_engine->materials_descriptor.path, &p_engine->material_count, &p_engine->p_materials);
            if (p_engine->p_materials == NULL || mat_r == 0) {
                return;
            }
            shEndScene(p_engine);
            if (!shLoadScene(p_engine->scene_descriptor.path, p_engine->material_count, &p_engine->p_materials, &p_engine->scene)) {
                return;
            }
            shSceneInit(p_engine, &p_engine->scene);

            p_engine->p_gui->core.surface = p_engine->core.surface.surface;
            p_engine->p_gui->core.render_pass = p_engine->core.render_pass;
            shGuiDestroyPipelines(p_engine->p_gui);
            shGuiBuildRegionPipeline(p_engine->p_gui, 256);
            shGuiBuildTextPipeline(p_engine->p_gui, 256);
            shGuiSetDefaultValues(p_engine->p_gui, SH_GUI_THEME_DARK, SH_GUI_RECORD);
        }

        shUpdateWindow(p_engine);

        input_dtime = p_engine->time.now - input_last_time;
        if (input_dtime >= 2.0) {
            if (shIsKeyDown(p_engine->window, SH_KEY_LEFT_CONTROL) && shIsKeyPressed(p_engine->window, SH_KEY_R)) {
                input_last_time = p_engine->time.now;
                shSetTime(0.0, &p_engine->time);
                input_last_time = 0.0;
                shEngineManageState(p_engine, shResetEngineState(p_engine, 1));
                break;
            }
        }

        if (!shSharedSceneRun(p_engine, p_engine->simulation_host.p_update)) {
            shEngineManageState(p_engine, shResetEngineState(p_engine, 1));
        }
        
        if (p_engine->p_gui != NULL) {
            shGuiWriteMemory(p_engine->p_gui, 1);
            uint8_t active_cursor_icon_idx = 3;
            for (uint8_t i = 0; i < sizeof(ShGuiCursorIcons) / sizeof(int32_t); i++) {
                if (p_engine->p_gui->inputs.active_cursor_icon == p_engine->p_gui->inputs.p_cursor_icons[i]) {
                    active_cursor_icon_idx = i;
                    break;
                }
            }
            shSetCursor(p_engine->window.window, p_engine->window.default_cursors[active_cursor_icon_idx]);
            p_engine->p_gui->inputs.active_cursor_icon = GLFW_CROSSHAIR_CURSOR;

        }

        shFrameReset(&p_engine->core, 0);

        uint32_t image_index = 0;
        shFrameBegin(&p_engine->core, 0, (VkClearColorValue) { 0.0f, 0.0f, 0.0f, 1.0f}, & image_index);

        shSharedHostWarning(
            shSharedSceneRun(p_engine, p_engine->simulation_host.p_frame_update), 
            "simulation update frame failed"
        );
        shSceneUpdate(p_engine);

        if (p_engine->p_gui != NULL) {
            shGuiUpdateInputs(p_engine->p_gui);
            shGuiRender(p_engine->p_gui);
        }

        shFrameEnd(&p_engine->core, 0, image_index);
    }

    shEngineRelease(p_engine, 1);
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

void shEngineRelease(ShEngine* p_engine, const uint8_t release_shared) {
    shEngineError(p_engine == NULL, "invalid engine memory");
    
    shGuiRelease(p_engine->p_gui);
    p_engine->p_gui = NULL;
    if (p_engine->simulation_host.shared != NULL) {
        shSharedSceneRun(p_engine, p_engine->simulation_host.p_close);
        if (release_shared) {
            shSharedRelease(&p_engine->simulation_host.shared);
        }
    }
    shMaterialsRelease(&p_engine->core, &p_engine->material_count, &p_engine->p_materials);
    shEndScene(p_engine);
}



#ifdef __cplusplus
}
#endif//__cplusplus