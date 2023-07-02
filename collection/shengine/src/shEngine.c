#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shengine/shEngine.h>

#include <stdlib.h>
#include <memory.h>

#ifdef _MSC_VER
#pragma warning (disable: 6011)
#endif//_MSC_VER



uint8_t shEngineSafeState(
    ShEngine* p_engine,
    uint8_t   load_shared
) {
    shEngineWarning(1, "running safe window");

    ShEngineVkCore* p_core      = &p_engine->core;
    ShWindow*       p_window    = &p_engine->window;
    uint32_t        image_index = 0;

    while (shIsWindowActive(*p_window)) {

        shUpdateWindow(p_engine);

        if (
             shIsKeyDown    (*p_window, SH_KEY_LEFT_CONTROL) && 
             shIsKeyPressed (*p_window, SH_KEY_R)
            ) {

            shEngineManageState(
                p_engine, 
                shResetEngineState(p_engine, load_shared, 1),
                load_shared,
                0
            );

            break;
        }
    }

    shEngineVulkanRelease(p_engine);
    shClearWindow        (p_window);
    shFreeEngine         (p_engine);

    exit(-1);
}

uint8_t shSetEngineState(
    ShEngine* p_engine,
    uint8_t   load_shared
) {
    shEngineError(p_engine == NULL, "shSetEngineState: invalid engine memory", return 0);

    shEngineError(
        shGetIniProperties("ini.smd", p_engine->p_ini_smd, &p_engine->ini_properties) == 0,
        "shSetEngineState: failed getting ini properties",
        return 0
    );

    shEngineError(p_engine->ini_properties.application_smd_path  [0] == '\0', "shSetEngineState: could not detect application smd path",   return 0);
    shEngineError(p_engine->ini_properties.host_memory_smd_path  [0] == '\0', "shSetEngineState: could not detect host memory smd path",   return 0);
    shEngineError(p_engine->ini_properties.vulkan_memory_smd_path[0] == '\0', "shSetEngineState: could not detect vulkan memory smd path", return 0);
    shEngineError(p_engine->ini_properties.serial_smd_path       [0] == '\0', "shSetEngineState: could not detect serial smd path",        return 0);
    shEngineError(p_engine->ini_properties.scene_smd_path        [0] == '\0', "shSetEngineState: could not detect scene smd path",         return 0);

    shEngineError(
        shGetApplicationProperties(p_engine->ini_properties.application_smd_path, p_engine->p_application_smd, &p_engine->application_properties) == 0,
        "shSetEngineState: failed getting application properties",
        return 0
    );

    if (p_engine->ini_properties.host_memory_smd_path[0] != '\0') {
        shEngineError(
            shGetHostMemoryProperties(p_engine->ini_properties.host_memory_smd_path, p_engine->p_host_memory_smd, &p_engine->host_memory_properties) == 0,
            "shSetEngineState: failed getting host memory properties",
            return 0
        );
    }

    if (p_engine->ini_properties.vulkan_memory_smd_path[0] != '\0') {
        shEngineError(
            shGetVulkanMemoryProperties(p_engine->ini_properties.vulkan_memory_smd_path, p_engine->p_vulkan_memory_smd, &p_engine->vulkan_memory_properties) == 0,
            "shSetEngineState: failed loading vulkan memory",
            return 0
        );
    }
    
    if (p_engine->ini_properties.serial_smd_path[0] != '\0') {
        shEngineError(
            shGetSerialProperties(p_engine->ini_properties.serial_smd_path, p_engine->p_serial_smd, &p_engine->serial_properties) == 0,
            "shSetEngineState: failed getting serial properties",
            return 0
        );
    }

    if (p_engine->ini_properties.scene_smd_path[0] != '\0') {
        shEngineError(
            shGetSceneProperties(p_engine->ini_properties.scene_smd_path, p_engine->p_scene_smd, &p_engine->scene_properties) == 0,
            "shSetEngineState: failed getting scene properties",
            return 0
        );
    }
    
    if (load_shared) {
        shEngineError(
            shLoadApplication(
                p_engine->application_properties.run, p_engine->application_properties.shared_path,
                p_engine->application_properties.s_start,
                p_engine->application_properties.s_thread,
                p_engine->application_properties.s_update_pending,
                p_engine->application_properties.s_after_thread,
                p_engine->application_properties.s_update,
                p_engine->application_properties.s_main_cmd_buffer,
                p_engine->application_properties.s_main_renderpass,
                p_engine->application_properties.s_frame_resize,
                p_engine->application_properties.s_close,
                &p_engine->application_host
            ) == 0,
            "shSetEngineState: failed loading application",
            return 0
        );
    }

    //host memory already loaded
    
    //vulkan memory
    for (uint32_t buffer_idx = 0; buffer_idx < p_engine->vulkan_memory_properties.buffer_count; buffer_idx++) {
              
        uint8_t               index_buffer_bit        = p_engine->vulkan_memory_properties.buffers_usage_index_buffer_bit   [buffer_idx];
        uint8_t               uniform_buffer_bit      = p_engine->vulkan_memory_properties.buffers_usage_uniform_buffer_bit [buffer_idx];
        uint8_t               storage_buffer_bit      = p_engine->vulkan_memory_properties.buffers_usage_storage_buffer_bit [buffer_idx];
        uint8_t               transfer_dst_bit        = p_engine->vulkan_memory_properties.buffers_usage_transfer_dst_bit   [buffer_idx];
        uint8_t               transfer_src_bit        = p_engine->vulkan_memory_properties.buffers_usage_transfer_src_bit   [buffer_idx];
        uint8_t               vertex_buffer_bit       = p_engine->vulkan_memory_properties.buffers_usage_vertex_buffer_bit  [buffer_idx];
                                                      
        uint8_t               sharing_mode_exclusive  = p_engine->vulkan_memory_properties.buffers_memory_sharing_mode_exclusive[buffer_idx];
        uint8_t               sharing_mode_concurrent = p_engine->vulkan_memory_properties.buffers_memory_sharing_mode_concurrent[buffer_idx];

        uint8_t               device_local_bit        = p_engine->vulkan_memory_properties.buffers_memory_property_device_local_bit  [buffer_idx];
        uint8_t               host_visible_bit        = p_engine->vulkan_memory_properties.buffers_memory_property_host_visible_bit  [buffer_idx];
        uint8_t               host_coherent_bit       = p_engine->vulkan_memory_properties.buffers_memory_property_host_coherent_bit [buffer_idx];
                                                      
        VkDevice              device                  = p_engine->core.device;
        VkPhysicalDevice      physical_device         = p_engine->core.physical_device;
                                                      
        uint32_t              buffer_size             = p_engine->vulkan_memory_properties.buffers_size   [buffer_idx];
        VkBufferUsageFlags    buffer_usage_flags      = 0;                                               
        VkSharingMode         memory_sharing_mode     = 0;                                               
        VkMemoryPropertyFlags memory_property_flags   = 0;                                               
        VkBuffer*             p_buffer                = &p_engine->vulkan_memory_properties.buffers        [buffer_idx];
        VkDeviceMemory*       p_device_memory         = &p_engine->vulkan_memory_properties.buffers_memory [buffer_idx];

        (index_buffer_bit  )      && (buffer_usage_flags    |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT  );
        (uniform_buffer_bit)      && (buffer_usage_flags    |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
        (storage_buffer_bit)      && (buffer_usage_flags    |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
        (transfer_dst_bit  )      && (buffer_usage_flags    |= VK_BUFFER_USAGE_TRANSFER_DST_BIT  );
        (transfer_src_bit  )      && (buffer_usage_flags    |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT  );
        (vertex_buffer_bit )      && (buffer_usage_flags    |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );

        (sharing_mode_exclusive ) && (memory_sharing_mode    = VK_SHARING_MODE_EXCLUSIVE );
        (sharing_mode_concurrent) && (memory_sharing_mode    = VK_SHARING_MODE_CONCURRENT);

        (device_local_bit  )      && (memory_property_flags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
        (host_visible_bit  )      && (memory_property_flags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT );
        (host_coherent_bit )      && (memory_property_flags |= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
                                  
        shEngineError(
            shCreateBuffer(device, buffer_size, buffer_usage_flags, memory_sharing_mode, p_buffer) == 0,
            "shSetEngineState: failed creating vulkan buffer",
            return 0
        );

        shEngineError(
            shAllocateBufferMemory(device, physical_device, (*p_buffer), memory_property_flags, p_device_memory) == 0,
            "shSetEngineState: failed allocating vulkan buffer memory",
            return 0
        );

        shEngineError(
            shBindBufferMemory(device, (*p_buffer), 0, (*p_device_memory)) == 0,
            "shSetEngineState: failed binding vulkan buffer memory",
            return 0
        );

    }

    //scene already loaded

    shEngineError(
        shSceneInit(p_engine) == 0,
        "shSetEngineState: failed initializing scene",
        return 0
    );

    if (p_engine->application_properties.run) {
        shEngineError(
            shSharedSceneRun(p_engine, p_engine->application_host.p_start) == 0,
            "shSetEngineState: application start function failed",
            return 0
        );
    }

    if (p_engine->thread_pool.p_threads == NULL) {
        shAllocateThreads(
            1,
            &p_engine->thread_pool
        );
    }
    

    if (p_engine->application_host.p_thread != NULL) {

        ShThreadParameters application_args = p_engine->p_ext;

        shEngineError(
            shCreateThread(SH_APPLICATION_THREAD_IDX, p_engine->application_host.p_thread, 4096, &p_engine->thread_pool) != SH_THREADS_SUCCESS,
            "shSetEngineState: failed creating thread",
            return 0
        );
        

        shEngineError(
            shLaunchThreads(SH_APPLICATION_THREAD_IDX, 1, &application_args, &p_engine->thread_pool) != SH_THREADS_SUCCESS,
            "shSetEngineState: failed launching threads",
            return 0
        );

        shEngineError(
            shGetThreadState(SH_APPLICATION_THREAD_IDX, &p_engine->app_thread_state, &p_engine->thread_pool) != SH_THREADS_SUCCESS,
            "shSetEngineState: failed getting thread state",
            return 0
        );

    }

    return 1;
}

uint8_t shResetEngineState(
    ShEngine* p_engine,
    uint8_t   load_shared,
    uint8_t   release_shared_on_failure
) {
    shEngineError(p_engine == NULL, "shResetEngineState: invalid engine memory", return 0);

    shEngineRelease(
        p_engine, 
        release_shared_on_failure
    );

    return shSetEngineState(p_engine, load_shared);
}

uint8_t shEngineUpdateState(
    ShEngine* p_engine,
    uint8_t   load_shared
) {
    ShEngineVkCore*      p_core               = &p_engine->core;
    ShWindow*            p_window             = &p_engine->window;
    ShApplicationHandle* p_shared_host        = &p_engine->application_host;
    ShThreadState*       p_app_thread_state   = &p_engine->app_thread_state;

    uint8_t              swapchain_suboptimal = 0;

    while (shIsWindowActive(*p_window)) {
        shUpdateWindow(p_engine);

		int _width = 0;
		int _height = 0;
		glfwGetWindowSize(p_window->window, &_width, &_height);

		if ( _width != 0 && _height != 0 && //otherwise it's minimized
            (_width != p_window->width || _height != p_window->height)) {
            
            uint64_t return_value = 0;
            shWaitForThreads(SH_APPLICATION_THREAD_IDX, 1, UINT64_MAX, &return_value, &p_engine->thread_pool);
            if (!return_value) {
                shEngineManageState(p_engine, 0, load_shared, 1);
            }

            //if (p_engine->p_gui != NULL) {
            //    shGuiResizeInterface(p_engine->p_gui, p_window->width, p_window->height, _width, _height);
            //}
            
            p_window->width  = _width;
            p_window->height = _height;
            
            shWaitDeviceIdle(p_core->device);
            
            shDestroyRenderpass(p_core->device, p_core->renderpass);
            shDestroyFramebuffers(p_core->device, p_core->swapchain_image_count, p_core->framebuffers);
            shDestroyImageViews(p_core->device, p_core->swapchain_image_count, p_core->swapchain_image_views);
            shDestroySwapchain(p_core->device, p_core->swapchain);
            shDestroySurface(p_core->instance, p_core->surface);
            
            shClearImageMemory(p_core->device, p_core->depth_image, p_core->depth_image_memory);
            shClearImageMemory(p_core->device, p_core->input_color_image, p_core->input_color_image_memory);
            shDestroyImageViews(p_core->device, 1, &p_core->depth_image_view);
            shDestroyImageViews(p_core->device, 1, &p_core->input_color_image_view);
            
            glfwCreateWindowSurface(p_core->instance, p_window->window, VK_NULL_HANDLE, &p_core->surface);
            uint8_t graphics_supported = 0;
            shGetPhysicalDeviceSurfaceSupport(p_core->physical_device, p_core->graphics_queue_family_index, p_core->surface, &graphics_supported);//always true
            shGetPhysicalDeviceSurfaceCapabilities(p_core->physical_device, p_core->surface, &p_core->surface_capabilities);
            shCreateSwapchain(
            	p_core->device, p_core->physical_device, p_core->surface,
            	p_core->swapchain_image_format, 
            	&p_core->swapchain_image_format,
            	SH_ENGINE_SWAPCHAIN_IMAGE_COUNT,
            	p_core->swapchain_image_sharing_mode,
            	0, 
            	&p_core->swapchain_image_count,
            	&p_core->swapchain
            );
            shGetSwapchainImages(p_core->device, p_core->swapchain, &p_core->swapchain_image_count, p_core->swapchain_images);
            for (uint32_t i = 0; i < p_core->swapchain_image_count; i++) {
            	shCreateImageView(
            		p_core->device, p_core->swapchain_images[i], 
            		VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT, 
            		1, p_core->swapchain_image_format, 
            		&p_core->swapchain_image_views[i]
            	);
            }
            
            shCreateImage(
            	p_core->device, VK_IMAGE_TYPE_2D,
            	p_window->width, p_window->height, 1,
            	VK_FORMAT_D32_SFLOAT, 
            	1, p_core->sample_count,
            	VK_IMAGE_TILING_OPTIMAL,
            	VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            	VK_SHARING_MODE_EXCLUSIVE, &p_core->depth_image
            );
            shAllocateImageMemory(
            	p_core->device, p_core->physical_device, p_core->depth_image,
            	VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            	&p_core->depth_image_memory
            );
            shBindImageMemory(
            	p_core->device,p_core-> depth_image, 0, p_core->depth_image_memory
            );
            shCreateImageView(
            	p_core->device, p_core->depth_image, VK_IMAGE_VIEW_TYPE_2D,
            	VK_IMAGE_ASPECT_DEPTH_BIT, 1,
            	VK_FORMAT_D32_SFLOAT, &p_core->depth_image_view
            );
            
            shCreateImage(
            	p_core->device, VK_IMAGE_TYPE_2D,
            	p_window->width, p_window->height, 1,
            	p_core->swapchain_image_format, 
            	1, p_core->sample_count,
            	VK_IMAGE_TILING_OPTIMAL,
            	VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            	VK_SHARING_MODE_EXCLUSIVE,
            	&p_core->input_color_image
            );
            shAllocateImageMemory(
            	p_core->device, p_core->physical_device, p_core->input_color_image,
            	VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            	&p_core->input_color_image_memory
            );
            shBindImageMemory(
            	p_core->device, p_core->input_color_image, 0, p_core->input_color_image_memory
            );
            shCreateImageView(
            	p_core->device, p_core->input_color_image, VK_IMAGE_VIEW_TYPE_2D,
            	VK_IMAGE_ASPECT_COLOR_BIT, 1, p_core->swapchain_image_format,
            	&p_core->input_color_image_view
            );
            
            VkAttachmentDescription attachment_descriptions[SH_ENGINE_RENDERPASS_ATTACHMENT_COUNT] = {
            	p_engine->core.input_color_attachment, p_engine->core.depth_attachment, p_engine->core.resolve_attachment
            };
            shCreateRenderpass(p_core->device, SH_ENGINE_RENDERPASS_ATTACHMENT_COUNT, attachment_descriptions, 1, &p_core->subpass, &p_core->renderpass);
            for (uint32_t i = 0; i < p_core->swapchain_image_count; i++) {
            	VkImageView image_views[SH_ENGINE_RENDERPASS_ATTACHMENT_COUNT] = { 
            		p_core->input_color_image_view, p_core->depth_image_view, p_core->swapchain_image_views[i]
            	};
            	shCreateFramebuffer(p_core->device, p_core->renderpass, SH_ENGINE_RENDERPASS_ATTACHMENT_COUNT, image_views, _width, _height, 1, &p_core->framebuffers[i]);
            }
            
            //if (p_engine->p_gui != NULL) {
            //    shGuiDestroyPipelines(p_engine->p_gui);
            //    shGuiSetSurface(p_engine->p_gui, p_engine->core.surface);
            //    shGuiSetRenderpass(p_engine->p_gui, p_engine->core.renderpass);
            //    shGuiBuildRegionPipeline(p_engine->p_gui, NULL, NULL);
            //    shGuiBuildCharPipeline(p_engine->p_gui, NULL, NULL);
            //}
            
            if (shSharedSceneRun(p_engine, p_engine->application_host.p_frame_resize) == 0) {
                shEngineManageState(p_engine, 0, load_shared, 1);
            }
            
            shWaitDeviceIdle(p_core->device);
		}

        if (
             shIsKeyDown(*p_window,    SH_KEY_LEFT_CONTROL) && 
             shIsKeyPressed(*p_window, SH_KEY_R)
            ) {
                shEngineManageState(p_engine, shResetEngineState(p_engine, load_shared, 1), load_shared, 1);
                break;
        }

        shEngineError(
            shGetThreadState(SH_APPLICATION_THREAD_IDX, &p_engine->app_thread_state, &p_engine->thread_pool) != SH_THREADS_SUCCESS,
            "shSetEngineState: failed getting thread state",
            return 0
        );

        if ((*p_app_thread_state) == SH_THREAD_RETURNED) {
            if (p_engine->application_host.after_thread_called == 0) {
                if (shSharedSceneRun(p_engine, p_engine->application_host.p_after_thread) == 0) {
                    shEngineManageState(p_engine, 0, load_shared, 1);
                }
                p_engine->application_host.after_thread_called++;
            }
            if (shSharedSceneRun(p_engine, p_engine->application_host.p_main_cmd_buffer) == 0) {
                shEngineManageState(p_engine, 0, load_shared, 1);
            }
        }
        else {
            if (shSharedSceneRun(p_engine, p_shared_host->p_update_pending) == 0) {
                shEngineManageState(p_engine, 0, load_shared, 1);
            }
        }

        if (shSharedSceneRun(p_engine, p_engine->application_host.p_update) == 0) {
            shEngineManageState(p_engine, 0, load_shared, 1);
        }
		
        if (_width != 0 && _height != 0) {
            shAcquireSwapchainImage(
                p_core->device,//device
                p_core->swapchain,//swapchain
                UINT64_MAX,//timeout_ns
                p_core->current_image_acquired_semaphore,//acquired_signal_semaphore
                VK_NULL_HANDLE,//acquired_signal_fence
                &p_core->swapchain_image_idx,//p_swapchain_image_index
                &swapchain_suboptimal
            );

            shWaitForFences(
                p_core->device,//device
                1,//fence_count
                &p_core->graphics_cmd_fences[p_core->swapchain_image_idx],//p_fences
                1,//wait_for_all
                UINT64_MAX//timeout_ns
            );

            shResetFences(
                p_core->device,//device
                1,//fence_count
                &p_core->graphics_cmd_fences[p_core->swapchain_image_idx]//p_fences
            );

            VkCommandBuffer cmd_buffer = p_core->graphics_cmd_buffers[p_core->swapchain_image_idx];

            shBeginCommandBuffer(cmd_buffer);

            //if (p_engine->p_gui != NULL) {
            //    shGuiWriteMemory(p_engine->p_gui, cmd_buffer, 0);
            //}

            if (shSharedSceneRun(p_engine, p_engine->application_host.p_main_cmd_buffer) == 0) {
                shEngineManageState(p_engine, 0, load_shared, 1);
            }

            VkClearValue clear_values[2] = { 0 };
            float* p_colors = clear_values[0].color.float32;
            p_colors[0] = 0.1f;
            p_colors[1] = 0.1f;
            p_colors[2] = 0.1f;

            clear_values[1].depthStencil.depth = 1.0f;

            shBeginRenderpass(
                cmd_buffer,//graphics_cmd_buffer
                p_core->renderpass,//renderpass
                0,//render_offset_x
                0,//render_offset_y
                p_core->surface_capabilities.currentExtent.width,//render_size_x
                p_core->surface_capabilities.currentExtent.height,//render_size_y
                2,//only attachments with VK_ATTACHMENT_LOAD_OP_CLEAR
                clear_values,//p_clear_values
                p_core->framebuffers[p_core->swapchain_image_idx]//framebuffer
            );

            if (shSharedSceneRun(p_engine, p_engine->application_host.p_main_renderpass) == 0) {
                shEngineManageState(p_engine, 0, load_shared, 1);
            }

            //if (p_engine->p_gui != NULL) {
            //    shGuiRender(p_engine->p_gui, cmd_buffer, p_engine->core.swapchain_image_idx);
            //}

            shEndRenderpass(cmd_buffer);

            shEndCommandBuffer(cmd_buffer);

            shQueueSubmit(
                1,//cmd_buffer_count
                &cmd_buffer,//p_cmd_buffers
                p_core->graphics_queue,//queue
                p_core->graphics_cmd_fences[p_core->swapchain_image_idx],//fence
                1,//semaphores_to_wait_for_count
                &p_core->current_image_acquired_semaphore,//p_semaphores_to_wait_for
                VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT,//wait_stage
                1,//signal_semaphore_count
                &p_core->current_graphics_queue_finished_semaphore//p_signal_semaphores
            );

            shQueuePresentSwapchainImage(
                p_core->present_queue,//present_queue
                1,//semaphores_to_wait_for_count
                &p_core->current_graphics_queue_finished_semaphore,//p_semaphores_to_wait_for
                p_core->swapchain,//swapchain
                p_core->swapchain_image_idx//swapchain_image_idx
            );

            p_core->swapchain_image_idx = (p_core->swapchain_image_idx + 1) % p_core->swapchain_image_count;
        }
	}

    shEngineRelease      (p_engine, 1);
    shEngineVulkanRelease(p_engine);
    shClearWindow        (p_window);
    shFreeEngine         (p_engine);

    exit(0);
}

uint8_t shEngineManageState(
    ShEngine* p_engine,
    uint8_t   ready,
    uint8_t   load_shared,
    uint8_t   release_shared_on_failure
) {
    shEngineError(p_engine == NULL, "shEngineManageState: invalid engine memory", return 0);

    if (ready) {
        if (!shEngineUpdateState(p_engine, load_shared)) {
            shEngineRelease(p_engine, release_shared_on_failure);
            shEngineSafeState(p_engine, load_shared);
        }
    }
    else {
        shEngineRelease(p_engine, 0);
        shEngineSafeState(p_engine, load_shared);
    }

    return 1;
}

uint8_t shEngineVulkanRelease(
    ShEngine* p_engine
) {
    shEngineError(p_engine == NULL, "shEngineVulkanRelease: invalid engine memory", return 0);

    shDestroySemaphores(p_engine->core.device, 1, &p_engine->core.current_image_acquired_semaphore);
    shDestroySemaphores(p_engine->core.device, 1, &p_engine->core.current_graphics_queue_finished_semaphore);

    shDestroyFences(p_engine->core.device, p_engine->core.swapchain_image_count, p_engine->core.graphics_cmd_fences);

    shDestroyCommandBuffers(p_engine->core.device, p_engine->core.graphics_cmd_pool, p_engine->core.swapchain_image_count,  p_engine->core.graphics_cmd_buffers);
    shDestroyCommandBuffers(p_engine->core.device, p_engine->core.present_cmd_pool,  1,                                    &p_engine->core.present_cmd_buffer);

    shDestroyCommandPool(p_engine->core.device, p_engine->core.graphics_cmd_pool);
    if (p_engine->core.graphics_queue_family_index != p_engine->core.present_queue_family_index) {
        shDestroyCommandPool(p_engine->core.device, p_engine->core.present_cmd_pool);
    }

    shClearImageMemory(p_engine->core.device, p_engine->core.depth_image,       p_engine->core.depth_image_memory);
	shClearImageMemory(p_engine->core.device, p_engine->core.input_color_image, p_engine->core.input_color_image_memory);

	shDestroyImageViews(p_engine->core.device, 1, &p_engine->core.depth_image_view);
	shDestroyImageViews(p_engine->core.device, 1, &p_engine->core.input_color_image_view);

	shDestroyRenderpass(p_engine->core.device, p_engine->core.renderpass);

	shDestroyFramebuffers(p_engine->core.device, p_engine->core.swapchain_image_count, p_engine->core.framebuffers);

	shDestroyImageViews(p_engine->core.device, p_engine->core.swapchain_image_count, p_engine->core.swapchain_image_views);

	shDestroySwapchain(p_engine->core.device, p_engine->core.swapchain);

	shDestroyDevice(p_engine->core.device);

	shDestroySurface(p_engine->core.instance, p_engine->core.surface);

	shDestroyInstance(p_engine->core.instance);

    return 1;
}

uint8_t shEngineRelease(
    ShEngine* p_engine,
    uint8_t   release_shared
) {
    shEngineError(p_engine == NULL, "shEngineRelease: invalid engine memory", return 0);

    if (release_shared && p_engine->application_host.p_close != NULL) {
        shEngineError(
            shSharedSceneRun(p_engine, p_engine->application_host.p_close) == 0,
            "shEngineRelease: failed closing application",
            return 0
        );
    }

    if (p_engine->thread_pool.p_handles != NULL) {
        uint64_t return_value = 0;

        shEngineError(
            shWaitForThreads(
                SH_APPLICATION_THREAD_IDX,
                1,
                UINT64_MAX,
                &return_value,
                &p_engine->thread_pool
            ) != SH_THREADS_SUCCESS,
            "shEngineRelease: failed waiting for threads",
            return 0
        );

        shEngineError(
            shReleaseThreads(&p_engine->thread_pool) != SH_THREADS_SUCCESS,
            "shEngineRelease: failed releasing threads",
            return 0
        );
        
        shEngineError(
            return_value == 0, 
            "shEngineRelease: application thread returned with an error", 
            return 0
        );
    }
    p_engine->application_host.after_thread_called = 0;


    if (release_shared && p_engine->application_host.shared) {
        shSharedRelease(&p_engine->application_host.shared);
    }

    //if (p_engine->p_gui != NULL) {
    //    shEngineError(
    //        shGuiRelease(p_engine->p_gui) == 0,
    //        "shEngineRelease: failed releasing shgui memory",
    //        return 0
    //    );
    //    free(p_engine->p_gui);
    //}

    for (uint32_t host_buffer_idx = 0; host_buffer_idx < p_engine->host_memory_properties.buffer_count; host_buffer_idx++) {
        free(p_engine->host_memory_properties.p_buffers_memory[host_buffer_idx]);
    }
    
    for (uint32_t vulkan_buffer_idx = 0; vulkan_buffer_idx < p_engine->vulkan_memory_properties.buffer_count; vulkan_buffer_idx++) {
        shEngineError(
            shClearBufferMemory(
                p_engine->core.device,
                p_engine->vulkan_memory_properties.buffers       [vulkan_buffer_idx],
                p_engine->vulkan_memory_properties.buffers_memory[vulkan_buffer_idx]
            ) == 0,
            "shEngineRelease: failed releasing vulkan buffer memory",
            return 0
        );
    }

    free(p_engine->p_ini_smd          );
    free(p_engine->p_application_smd  );
    free(p_engine->p_host_memory_smd  );
    free(p_engine->p_vulkan_memory_smd);
    free(p_engine->p_scene_smd        );



    memset(&p_engine->ini_properties,           0, sizeof(ShIniProperties));
    memset(&p_engine->application_properties,   0, sizeof(ShApplicationProperties));
    memset(&p_engine->host_memory_properties,   0, sizeof(ShHostMemoryProperties));
    memset(&p_engine->vulkan_memory_properties, 0, sizeof(ShVulkanMemoryProperties));
    memset(&p_engine->scene_properties,         0, sizeof(ShSceneProperties));


    //memset(&p_engine->application_host, 0, sizeof(ShApplicationHandle));//we need to keep the functions information
    memset(&p_engine->thread_pool,      0, sizeof(ShThreadPool));
    memset(&p_engine->app_thread_state, 0, sizeof(ShThreadState));

    //user should release pipeline pool
    memset(&p_engine->pipeline_count, 0, sizeof(uint32_t));

    
    shEndScene(p_engine);

    return 1;
}

void shEngineShutdown(
    ShEngine* p_engine
) {
    shEngineError(p_engine == NULL, "shEngineShutdown: invalid engine memory", return);

    shEngineRelease       (p_engine, 0);
    shEngineVulkanRelease (p_engine);
    shClearWindow         (&p_engine->window);
    shFreeEngine          (p_engine);

    exit(0);
}


#ifdef __cplusplus
}
#endif//__cplusplus