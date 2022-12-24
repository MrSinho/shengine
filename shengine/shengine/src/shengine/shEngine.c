#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



#include <shengine/shEngine.h>
#include <shvulkan/shVkDrawLoop.h>
#include <shvulkan/shVkCore.h>

#include "shegui/shEgui.h"

#ifdef _MSC_VER
#pragma warning (disable: 6011)
#endif//_MSC_VER



void shEngineSafeState(ShEngine* p_engine) {
    shEngineWarning(1, "running safe window");

    ShVkCore* p_core      = &p_engine->core;
    ShWindow* p_window    = &p_engine->window;
    uint32_t  image_index = 0;

    while (shIsWindowActive(*p_window)) {

        shUpdateWindow(p_engine);

        if (
             shIsKeyDown    (*p_window, SH_KEY_LEFT_CONTROL) && 
             shIsKeyPressed (*p_window, SH_KEY_R)
            ) {

            shEngineManageState(
                p_engine, 
                shResetEngineState(p_engine, 1), 
                0
            );

            break;
        }
        shFrameReset (p_core, 0);
        shFrameBegin (p_core, 0, (VkClearColorValue) { 0.0f, 0.0f, 0.0f, 1.0f }, &image_index);
        shFrameEnd   (p_core, 0, image_index);
    }

    shVulkanRelease  (p_core);
    shClearWindow    (p_window);

    exit(-1);
}

ShEngineStatus shSetEngineState(ShEngine* p_engine) {
    shEngineError(p_engine == NULL, "invalid engine memory", return SH_ENGINE_INVALID_ENGINE_MEMORY);

    shGetIniProperties("loader.ini", &p_engine->loader_ini);

	shAppendAssetsPath((const char*)p_engine->loader_ini.assets_path, "/descriptors/", "materials.json",   &p_engine->materials_descriptor);
	shAppendAssetsPath((const char*)p_engine->loader_ini.assets_path, "/descriptors/", "scene.json",       &p_engine->scene_descriptor);
	shAppendAssetsPath((const char*)p_engine->loader_ini.assets_path, "/descriptors/", "application.json", &p_engine->application_descriptor);

    uint8_t mat_r = shLoadMaterials(
        &p_engine->core, 
        p_engine->materials_descriptor.dir,
        p_engine->materials_descriptor.filename, 
        1,
        &p_engine->material_count, 
        &p_engine->p_materials
    );

    if (p_engine->p_materials == NULL || mat_r == 0) {
        return 0;
    }

    shEngineError(
        shLoadScene(
            p_engine->scene_descriptor.dir,
            p_engine->scene_descriptor.filename,
            p_engine->material_count, 
            &p_engine->p_materials,
            &p_engine->scene
        ) == 0,
        "failed loading scene",
        return SH_ENGINE_SCENE_LOAD_FAILURE
    );

    shSceneInit(
        p_engine, 
        &p_engine->scene
    );

    if (p_engine->application_host.p_start == NULL || p_engine->application_host.p_update == NULL || p_engine->application_host.p_close == NULL) {
        shLoadApplication(
            p_engine->application_descriptor.path,
            &p_engine->application_host
        );

        shApplicationLoadSymbols(
            &p_engine->application_host
        );
    }

    if (p_engine->application_host.p_start != NULL) {
        shEngineError(
            shSharedSceneRun(
                p_engine, 
                p_engine->application_host.p_start
            ) == 0,
            "application start function failed",
            return SH_ENGINE_SIM_START_FAILURE
        )
    }

    shAllocateThreads(
        1, 
        &p_engine->thread_pool
    );

    if (p_engine->application_host.p_thread != NULL) {

        ShThreadParameters application_args = p_engine->p_ext;

        shCreateThread(
            SH_SIMULATION_THREAD_IDX, 
            p_engine->application_host.p_thread, 
            4096, 
            &p_engine->thread_pool
        );

        shLaunchThreads(
            SH_SIMULATION_THREAD_IDX,
            1,
            &application_args,
            &p_engine->thread_pool
        );

        shGetThreadState(
            0,
            &p_engine->app_thread_state,
            &p_engine->thread_pool
        );
    }

    return SH_ENGINE_SUCCESS;
}

ShEngineStatus shResetEngineState(ShEngine* p_engine, const uint8_t release_shared) {
    shEngineError(p_engine == NULL, "invalid engine memory", return SH_ENGINE_INVALID_ENGINE_MEMORY);
    shEngineRelease(
        p_engine, 
        release_shared
    );
    return shSetEngineState(
        p_engine
    ) == 1 ? SH_ENGINE_SUCCESS : 0;
}

void shEngineUpdateState(ShEngine* p_engine) {
    ShVkCore*           p_core                  = &p_engine->core;
    ShWindow*           p_window                = &p_engine->window;
    ShApplicationHandle* p_shared_host           = &p_engine->application_host;
    ShThreadState*      p_app_thread_state      = &p_engine->app_thread_state;

    while (shIsWindowActive(*p_window)) {
        
        shGetWindowSize(p_window);

        shGetThreadState(
            SH_SIMULATION_THREAD_IDX, 
            p_app_thread_state, 
            &p_engine->thread_pool
        );

        if (
             shSurfaceResizePending(*p_window) && 
             (*p_app_thread_state) == SH_THREAD_RETURNED
            ) {

            p_window->surface_resize_pending = 0;

            shWaitDeviceIdle(p_engine->core.device);

            shRenderPassRelease  (p_core);
            shSwapchainRelease   (p_core);
            shSurfaceRelease     (p_core);
            shDepthBufferRelease (p_core);

            shWindowCreateSurface(p_engine);
            shInitSwapchainData  (p_core);
            shInitDepthData      (p_core);
            shCreateRenderPass   (p_core);
            shSetFramebuffers    (p_core);

            for (uint32_t material_idx = 0; material_idx < p_engine->material_count; material_idx++) {
                ShMaterialHost* p_material = &p_engine->p_materials[material_idx];

                VkDescriptorBufferInfo buffer_infos[32];
                VkBuffer descriptor_buffers[32];
                VkDeviceMemory descriptor_buffers_memory[32];

                uint32_t descriptor_count = p_material->pipeline.descriptor_count;
                memcpy(
                    buffer_infos, 
                    p_material->pipeline.descriptor_buffer_infos, 
                    descriptor_count * sizeof(VkDescriptorBufferInfo)
                );
                memcpy(
                    descriptor_buffers,
                    p_material->pipeline.descriptor_buffers,
                    descriptor_count * sizeof(VkBuffer)
                );
                memcpy(
                    descriptor_buffers_memory,
                    p_material->pipeline.descriptor_buffers_memory,
                    descriptor_count * sizeof(VkDeviceMemory)
                );
                shPipelineRelease(
                     p_engine->core.device, 
                    &p_engine->p_materials[material_idx].pipeline
                );
                memcpy(
                    p_material->pipeline.descriptor_buffer_infos,
                    buffer_infos,
                    descriptor_count * sizeof(VkDescriptorBufferInfo)
                );
                memcpy(
                    p_material->pipeline.descriptor_buffers,
                    descriptor_buffers,
                    descriptor_count * sizeof(VkBuffer)
                );
                memcpy(
                    p_material->pipeline.descriptor_buffers_memory,
                    descriptor_buffers_memory,
                    descriptor_count * sizeof(VkDeviceMemory)
                );
            }

            uint8_t mat_r = shLoadMaterials(
                p_core, 
                p_engine->materials_descriptor.dir,
                p_engine->materials_descriptor.filename, 
                0,
                &p_engine->material_count, 
                &p_engine->p_materials
            );
            shEngineError(
                mat_r == 0,
                "shEngineUpdateState: failed loading materials",
                return
            );

            for (uint32_t material_idx = 0; material_idx < p_engine->material_count; material_idx++) {
                ShMaterialHost* p_material = &p_engine->p_materials[material_idx];
                for (uint32_t descriptor_idx = 0; descriptor_idx < p_material->pipeline.descriptor_count; descriptor_idx++) {
                    p_material->pipeline.write_descriptor_sets[descriptor_idx].pBufferInfo = &p_material->pipeline.descriptor_buffer_infos[descriptor_idx];
                }
            }

            if (p_engine->p_gui != NULL) {
                p_engine->p_gui->core.surface     = p_engine->core.surface.surface;
                p_engine->p_gui->core.render_pass = p_engine->core.render_pass;
                
                shGuiDestroyPipelines(
                    p_engine->p_gui
                );
                shGuiUnload(
                    p_engine->p_gui
                );
                shGuiBuildRegionPipeline(
                    p_engine->p_gui,
                    NULL,
                    NULL,
                    p_engine->p_gui->region_infos.max_region_items
                );
                shGuiBuildTextPipeline(
                    p_engine->p_gui,
                    NULL,
                    NULL,
                    p_engine->p_gui->text_infos.max_char_raw_size / SH_GUI_MAX_CHAR_VERTEX_SIZE
                );
                shGuiSetDefaultValues(
                    p_engine->p_gui, 
                    p_engine->p_gui->default_infos.default_values, 
                    SH_GUI_RECORD
                );
            }

            shEngineManageState(
                p_engine,
                shSharedSceneRun(
                    p_engine, 
                    p_engine->application_host.p_frame_resize
                ),
                1
            );

        }

        shUpdateWindow(p_engine);

        if (
             shIsKeyDown(*p_window,    SH_KEY_LEFT_CONTROL) && 
             shIsKeyPressed(*p_window, SH_KEY_R)
            ) {

            shEngineManageState(
                p_engine, 
                shResetEngineState(p_engine, 1), 
                0
            );

            break;
        }

        if ((*p_app_thread_state) == SH_THREAD_RETURNED) {
            if (p_engine->application_host.after_thread_called == 0) {
                if (
                    !shSharedSceneRun(
                        p_engine, 
                        p_engine->application_host.p_after_thread
                    )) {

                    shEngineManageState(
                        p_engine, 
                        0, 
                        1
                    );
                }
                p_engine->application_host.after_thread_called++;
            }

            if (
                !shSharedSceneRun(
                    p_engine, 
                    p_engine->application_host.p_update
                )) {

                shEngineManageState(
                    p_engine, 
                    0, 
                    1
                );
            }
        }
        else {
            if (!shSharedSceneRun(
                 p_engine, 
                 p_shared_host->p_update_pending)
                ) {
                shEngineManageState(
                    p_engine,
                    0,
                    1
                );
            }
        }

        if (p_engine->p_gui != NULL) {
            shGuiWriteMemory(
                p_engine->p_gui,
                1
            );
            uint8_t active_cursor_icon_idx = 3;
            for (uint8_t i = 0; i < sizeof(ShGuiCursorIcons) / sizeof(int32_t); i++) {
                if (p_engine->p_gui->inputs.active_cursor_icon == p_engine->p_gui->inputs.p_cursor_icons[i]) {
                    active_cursor_icon_idx = i;
                    break;
                }
            }
            shSetCursor(
                p_window->window,
                p_window->default_cursors[active_cursor_icon_idx]
            );
            p_engine->p_gui->inputs.active_cursor_icon = GLFW_CROSSHAIR_CURSOR;
        }

        shFrameReset(p_core, 0);

        uint32_t image_index = 0;
        shFrameBegin(
            p_core, 
            0, 
            (VkClearColorValue) { 0.0f, 0.0f, 0.0f, 1.0f}, 
            &image_index
        );

        if ((*p_app_thread_state) == SH_THREAD_RETURNED) {
            shSharedHostWarning(
                shSharedSceneRun(
                    p_engine, 
                    p_engine->application_host.p_frame_update
                ),
                "application update frame failed"
            );
        }
        
        shSceneUpdate(p_engine);

        if (p_engine->p_gui != NULL) {

            shGuiUpdateInputs(
                p_engine->p_gui
            );
            shGuiRender(
                p_engine->p_gui
            );

        }

        shFrameEnd(
            p_core, 
            0, 
            image_index
        );
    }

    shEngineRelease(p_engine, 1);
    shVulkanRelease(p_core);
    shClearWindow  (p_window);
    exit(0);
}

void shEngineManageState(ShEngine* p_engine, const ShEngineStatus ready, const uint8_t release_on_failure) {
    if (ready == SH_ENGINE_SUCCESS) {
        shEngineUpdateState(p_engine);
    }
    else {
        if (release_on_failure) {
            shEngineRelease(p_engine, 0);
        }
        shEngineSafeState(p_engine);
    }
}

void shEngineRelease(ShEngine* p_engine, const uint8_t release_shared) {
    shEngineError(p_engine == NULL, "invalid engine memory", exit(-1));

    if (p_engine->application_host.shared != NULL) {

        shEngineWarning(

            shSharedSceneRun(
                p_engine, 
                p_engine->application_host.p_close
            ) == 0, 
            "failed closing application"
        );

        if (p_engine->thread_pool.p_handles != NULL) {
            uint64_t return_value = 0;

            shWaitForThreads(
                SH_SIMULATION_THREAD_IDX,
                1,
                UINT64_MAX,
                &return_value,
                &p_engine->thread_pool
            );

            shReleaseThreads(
                &p_engine->thread_pool
            );

            shEngineWarning(return_value == 0, "application thread returned with an error");
        }

        if (release_shared) {
            shSharedRelease(&p_engine->application_host.shared);
        }

        p_engine->application_host.after_thread_called = 0;
    }
    if (p_engine->p_gui != NULL) {
        shGuiDestroyPipelines(p_engine->p_gui);
        shGuiReleaseDefaultValues(p_engine->p_gui);
        shGuiUnload(p_engine->p_gui);
        shGuiRelease(p_engine->p_gui);
        p_engine->p_gui = NULL;
    }

    shMaterialsRelease(
        &p_engine->core, 
        &p_engine->material_count, 
        &p_engine->p_materials
    );

    shEndScene(p_engine);
}

void shEngineShutdown(ShEngine* p_engine) {
    shEngineRelease  (p_engine, 0);
    shVulkanRelease (&p_engine->core);
    shClearWindow   (&p_engine->window);
    exit(0);
}


#ifdef __cplusplus
}
#endif//__cplusplus