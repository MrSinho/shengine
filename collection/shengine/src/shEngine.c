#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shengine/shEngine.h>



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <inttypes.h>



#ifdef _MSC_VER
#pragma warning (disable: 6011)
#endif//_MSC_VER



uint8_t shEngineSetupVulkan(
	ShEngine* p_engine
) {
	shEngineError(p_engine == NULL, "shEditorSetupVulkan: invalid engine memory", return 0);

	shCreateInstance(
		//application_name, engine_name, enable_validation_layers,
		"vulkan app", "vulkan engine", 1,
		p_engine->window.instance_extension_count, p_engine->window.pp_instance_extensions,
		VK_MAKE_API_VERSION(1, 3, 0, 0),//api_version,
		&p_engine->core.instance
	);

	shEngineError(
		glfwCreateWindowSurface(
			p_engine->core.instance, p_engine->window.window, VK_NULL_HANDLE, &p_engine->core.surface
		) != VK_SUCCESS,
		"shEditorSetupVulkan: failed creating window surface",
		return 0
	);
	

	shSelectPhysicalDevice(
		p_engine->core.instance,//instance,
		p_engine->core.surface,//surface,
		VK_QUEUE_GRAPHICS_BIT |
		VK_QUEUE_COMPUTE_BIT |
		VK_QUEUE_TRANSFER_BIT,//requirements,
		&p_engine->core.physical_device,//p_physical_device,
		&p_engine->core.physical_device_properties,//p_physical_device_properties,
		&p_engine->core.physical_device_features,//p_physical_device_features,
		&p_engine->core.physical_device_memory_properties//p_physical_device_memory_properties
	);

	shGetPhysicalDeviceQueueFamilies(
		p_engine->core.physical_device,//physical_device
		p_engine->core.surface,//surface
		VK_NULL_HANDLE,//p_queue_family_count
		&p_engine->core.graphics_queue_family_count,//p_graphics_queue_family_count
		&p_engine->core.present_queue_family_count,//p_surface_queue_family_count
		&p_engine->core.compute_queue_family_count,//p_compute_queue_family_count
		&p_engine->core.transfer_queue_family_count,//p_transfer_queue_family_count
		p_engine->core.graphics_queue_family_indices,//p_graphics_queue_family_indices
		p_engine->core.present_queue_family_indices,//p_surface_queue_family_indices
		p_engine->core.compute_queue_family_indices,//p_compute_queue_family_indices
		p_engine->core.transfer_queue_family_indices,//p_transfer_queue_family_indices
		VK_NULL_HANDLE//p_queue_families_properties
	);
	p_engine->core.default_graphics_queue_family_index = p_engine->core.graphics_queue_family_indices[0];
	p_engine->core.default_present_queue_family_index  = p_engine->core.present_queue_family_indices[0];
	p_engine->core.default_compute_queue_family_index  = p_engine->core.compute_queue_family_indices[0];
	p_engine->core.default_transfer_queue_family_index = p_engine->core.transfer_queue_family_indices[0];

	shGetPhysicalDeviceSurfaceCapabilities(
		p_engine->core.physical_device, p_engine->core.surface, &p_engine->core.surface_capabilities
	);

	float                   default_queue_priority     = 1.0f;
	VkDeviceQueueCreateInfo default_graphics_device_queue_info = { 0 };
	VkDeviceQueueCreateInfo default_present_device_queue_info  = { 0 };
	//no need to query for default 

	shQueryForDeviceQueueInfo(
		p_engine->core.default_graphics_queue_family_index,//queue_family_index
		1,//queue_count
		&default_queue_priority,//p_queue_priorities
		SH_FALSE,//protected
		&default_graphics_device_queue_info//p_device_queue_info
	);

	shQueryForDeviceQueueInfo(
		p_engine->core.default_present_queue_family_index,
		1,
		&default_queue_priority,
		SH_FALSE,
		&default_present_device_queue_info
	);


	VkDeviceQueueCreateInfo default_device_queue_infos[2] = {
		default_graphics_device_queue_info,
		default_graphics_device_queue_info
	};
	char* device_extensions[2]  = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	uint32_t default_device_queue_count = (p_engine->core.default_graphics_queue_family_index == p_engine->core.default_present_queue_family_index) ? 1 : 2;
	shSetLogicalDevice(
		p_engine->core.physical_device,//physical_device
		&p_engine->core.device,//p_device
		1,//extension_count
		device_extensions,//pp_extension_names
		default_device_queue_count,//device_queue_count
		default_device_queue_infos//p_device_queue_infos
	);

	shGetDeviceQueues(
		p_engine->core.device,//device
		1,//queue_count
		&p_engine->core.default_graphics_queue_family_index,//p_queue_family_indices
		&p_engine->core.graphics_queue//p_queues
	);

	shGetDeviceQueues(
		p_engine->core.device,//device
		1,//queue_count
		&p_engine->core.default_present_queue_family_index,//p_queue_family_indices
		&p_engine->core.present_queue//p_queues
	);
	
	p_engine->core.default_compute_queue_family_index  = p_engine->core.default_graphics_queue_family_index;
	p_engine->core.default_transfer_queue_family_index = p_engine->core.default_graphics_queue_family_index;
	p_engine->core.compute_queue                       = p_engine->core.graphics_queue;
	p_engine->core.transfer_queue                      = p_engine->core.graphics_queue;

	VkSharingMode swapchain_image_sharing_mode = VK_SHARING_MODE_EXCLUSIVE;
	if (p_engine->core.default_graphics_queue_family_index != p_engine->core.default_present_queue_family_index) {
		swapchain_image_sharing_mode = VK_SHARING_MODE_CONCURRENT;
	}
	shCreateSwapchain(
		p_engine->core.device,//device
		p_engine->core.physical_device,//physical_device
		p_engine->core.surface,//surface
		VK_FORMAT_R8G8B8_UNORM,//image_format
		&p_engine->core.swapchain_image_format,//p_image_format
		SH_ENGINE_SWAPCHAIN_IMAGE_COUNT,//swapchain_image_count
		swapchain_image_sharing_mode,//image_sharing_mode
		SH_FALSE,//vsync
		&p_engine->core.swapchain_image_count,
		&p_engine->core.swapchain//p_swapchain
	);//need p_swapchain_image_count

	shCreateCommandPool(
		p_engine->core.device,//device
		p_engine->core.default_graphics_queue_family_index,//queue_family_index
		&p_engine->core.graphics_cmd_pool//p_cmd_pool
	);

	shAllocateCommandBuffers(
		p_engine->core.device,//device
		p_engine->core.graphics_cmd_pool,//cmd_pool
		p_engine->core.swapchain_image_count,//cmd_buffer_count
		p_engine->core.graphics_cmd_buffers//p_cmd_buffer
	);

	if (p_engine->core.default_graphics_queue_family_index != p_engine->core.default_present_queue_family_index) {
		shCreateCommandPool(
			p_engine->core.device,//device
			p_engine->core.default_present_queue_family_index,//queue_family_index
			&p_engine->core.present_cmd_pool//p_cmd_pool
		);
	}
	else {
		p_engine->core.present_cmd_pool   = p_engine->core.graphics_cmd_pool;
	}
	shAllocateCommandBuffers(
		p_engine->core.device,//device
		p_engine->core.graphics_cmd_pool,//cmd_pool
		1,//cmd_buffer_count
		&p_engine->core.present_cmd_buffer//p_cmd_buffer
	);

	shAllocateCommandBuffers(
		p_engine->core.device,//device
		p_engine->core.graphics_cmd_pool,//cmd_pool
		1,//cmd_buffer_count
		&p_engine->core.compute_cmd_buffer//p_cmd_buffer
	);

	shAllocateCommandBuffers(
		p_engine->core.device,//device
		p_engine->core.graphics_cmd_pool,//cmd_pool
		1,//cmd_buffer_count
		&p_engine->core.transfer_cmd_buffer//p_cmd_buffer
	);

	shCreateFences(
		p_engine->core.device,//device
		p_engine->core.swapchain_image_count,//fence_count
		1,//signaled
		p_engine->core.graphics_cmd_fences//p_fences
	);

	shGetSwapchainImages(
		p_engine->core.device,//device
		p_engine->core.swapchain,//swapchain
		&p_engine->core.swapchain_image_count,//p_swapchain_image_count
		p_engine->core.swapchain_images//p_swapchain_images
	);

	for (uint32_t i = 0; i < p_engine->core.swapchain_image_count; i++) {
		shCreateImageView(
			p_engine->core.device,//device
			p_engine->core.swapchain_images[i],//image
			VK_IMAGE_VIEW_TYPE_2D,//view_type
			VK_IMAGE_ASPECT_COLOR_BIT,//image_aspect
			1,//mip_levels
			p_engine->core.swapchain_image_format,//format
			&p_engine->core.swapchain_image_views[i]//p_image_view
		);
	}

	shCombineMaxSamples(
		p_engine->core.physical_device_properties, 64,
		1, 1, &p_engine->core.sample_count
	);

	shCreateRenderpassAttachment(
		p_engine->core.swapchain_image_format,//format
		p_engine->core.sample_count,//sample_count
		VK_ATTACHMENT_LOAD_OP_CLEAR,//load_treatment
		VK_ATTACHMENT_STORE_OP_STORE,//store_treatment
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,//stencil_load_treatment
		VK_ATTACHMENT_STORE_OP_DONT_CARE,//stencil_store_treatment
		VK_IMAGE_LAYOUT_UNDEFINED,//initial_layout
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,//final_layout
		&p_engine->core.input_color_attachment//p_attachment_description
	);
	shCreateRenderpassAttachmentReference(
		0,//attachment_idx
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,//layout
		&p_engine->core.input_color_attachment_reference//p_attachment_reference
	);

	shCreateRenderpassAttachment(
		VK_FORMAT_D32_SFLOAT,
		p_engine->core.sample_count,
		VK_ATTACHMENT_LOAD_OP_CLEAR,
		VK_ATTACHMENT_STORE_OP_STORE,
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		VK_ATTACHMENT_STORE_OP_DONT_CARE,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
		&p_engine->core.depth_attachment
	);
	shCreateRenderpassAttachmentReference(
		1,
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
		&p_engine->core.depth_attachment_reference
	);
	
	shCreateRenderpassAttachment(
		p_engine->core.swapchain_image_format,
		1,//sample count
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		VK_ATTACHMENT_STORE_OP_STORE,
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		VK_ATTACHMENT_STORE_OP_DONT_CARE,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
		&p_engine->core.resolve_attachment
	);
	shCreateRenderpassAttachmentReference(
		2,
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		&p_engine->core.resolve_attachment_reference
	);

	shCreateSubpass(
		VK_PIPELINE_BIND_POINT_GRAPHICS,//bind_point
		0,//input_attachment_count
		VK_NULL_HANDLE,//p_input_attachments_reference
		SH_ENGINE_SUBASS_COLOR_ATTACHMENT_COUNT,//color_attachment_count
		&p_engine->core.input_color_attachment_reference,//p_color_attachments_reference
		&p_engine->core.depth_attachment_reference,//p_depth_stencil_attachment_reference
		&p_engine->core.resolve_attachment_reference,//p_resolve_attachment_reference
		0,//preserve_attachment_count
		VK_NULL_HANDLE,//p_preserve_attachments
		&p_engine->core.subpass//p_subpass
	);

	VkAttachmentDescription attachment_descriptions[SH_ENGINE_RENDERPASS_ATTACHMENT_COUNT] = {
		p_engine->core.input_color_attachment, p_engine->core.depth_attachment, p_engine->core.resolve_attachment
	};
	shCreateRenderpass(
		p_engine->core.device,//device
		SH_ENGINE_RENDERPASS_ATTACHMENT_COUNT,//attachment_count
		attachment_descriptions,//p_attachments_descriptions
		1,//subpass_count
		&p_engine->core.subpass,//p_subpasses
		&p_engine->core.renderpass//p_renderpass
	);
	
	shCreateImage(
		p_engine->core.device,
		VK_IMAGE_TYPE_2D,
		p_engine->window.width, p_engine->window.height, 1,
		VK_FORMAT_D32_SFLOAT,
		1, p_engine->core.sample_count,
	VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_SHARING_MODE_EXCLUSIVE,
		&p_engine->core.depth_image
	);
	shAllocateImageMemory(
		p_engine->core.device, p_engine->core.physical_device, p_engine->core.depth_image,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		&p_engine->core.depth_image_memory
	);
	shBindImageMemory(
		p_engine->core.device, p_engine->core.depth_image, 0, p_engine->core.depth_image_memory
	);
	shCreateImageView(
		p_engine->core.device, p_engine->core.depth_image, VK_IMAGE_VIEW_TYPE_2D,
		VK_IMAGE_ASPECT_DEPTH_BIT, 1,
		VK_FORMAT_D32_SFLOAT, &p_engine->core.depth_image_view
	);

	shCreateImage(
		p_engine->core.device, VK_IMAGE_TYPE_2D, p_engine->window.width, p_engine->window.height, 1,
		p_engine->core.swapchain_image_format, 1, p_engine->core.sample_count,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_SHARING_MODE_EXCLUSIVE,
		&p_engine->core.input_color_image
	);
	shAllocateImageMemory(
		p_engine->core.device, p_engine->core.physical_device, p_engine->core.input_color_image,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &p_engine->core.input_color_image_memory
	);
	shBindImageMemory(
		p_engine->core.device, p_engine->core.input_color_image, 0, p_engine->core.input_color_image_memory
	);
	shCreateImageView(
		p_engine->core.device, p_engine->core.input_color_image, VK_IMAGE_VIEW_TYPE_2D,
		VK_IMAGE_ASPECT_COLOR_BIT, 1, p_engine->core.swapchain_image_format,
		&p_engine->core.input_color_image_view
	);

	for (uint32_t i = 0; i < p_engine->core.swapchain_image_count; i++) {
		VkImageView image_views[SH_ENGINE_RENDERPASS_ATTACHMENT_COUNT] = {
			p_engine->core.input_color_image_view, p_engine->core.depth_image_view, p_engine->core.swapchain_image_views[i]
		};
		shCreateFramebuffer(
			p_engine->core.device,//device
			p_engine->core.renderpass,//renderpass
			SH_ENGINE_RENDERPASS_ATTACHMENT_COUNT,//image_view_count
			image_views,//p_image_views
			p_engine->core.surface_capabilities.currentExtent.width,//x
			p_engine->core.surface_capabilities.currentExtent.height,//y
			1,//z
			&p_engine->core.framebuffers[i]//p_framebuffer
		);
	}

	shCreateSemaphores(
		p_engine->core.device,//device
		1,//semaphore_count
		&p_engine->core.current_image_acquired_semaphore//p_semaphores
	);

	shCreateSemaphores(
		p_engine->core.device,//device
		1,//semaphore_count
		&p_engine->core.current_graphics_queue_finished_semaphore//p_semaphores
	);
	
	return 1;
}

uint8_t shEngineSafeState(
    ShEngine* p_engine
) {
    shEngineError(1, "running safe window", ;);

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
                shResetEngineState(p_engine)
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
    ShEngine* p_engine
) {
    shEngineError(p_engine == NULL, "shSetEngineState: invalid engine memory", return 0);

    shEngineError(
        shGetIniProperties("ini.smd", &p_engine->ini_smd, &p_engine->ini_properties) == 0,
        "shSetEngineState: failed getting ini properties",
        return 0
    );

    shEngineError(
        p_engine->ini_properties.application_smd_path[0] == '\0', 
        "shSetEngineState: could not detect application smd path",
        return 0
    );

    shEngineError(
        shGetApplicationProperties(p_engine->ini_properties.application_smd_path, &p_engine->application_smd, &p_engine->application_properties) == 0,
        "shSetEngineState: failed getting application properties",
        return 0
    );

    if (p_engine->ini_properties.host_memory_smd_path[0] != '\0') {
        shEngineError(
            shGetHostMemoryProperties(p_engine->ini_properties.host_memory_smd_path, &p_engine->host_memory_smd, &p_engine->host_memory_properties) == 0,
            "shSetEngineState: failed getting host memory properties",
            return 0
        );
    }

    if (p_engine->ini_properties.vulkan_memory_smd_path[0] != '\0') {
        shEngineError(
            shGetVulkanMemoryProperties(p_engine->ini_properties.vulkan_memory_smd_path, &p_engine->vulkan_memory_smd, &p_engine->vulkan_memory_properties) == 0,
            "shSetEngineState: failed loading vulkan memory",
            return 0
        );
    }
    
    if (p_engine->ini_properties.serial_smd_path[0] != '\0') {
        shEngineError(
            shGetSerialProperties(p_engine->ini_properties.serial_smd_path, &p_engine->serial_smd, &p_engine->serial_properties) == 0,
            "shSetEngineState: failed getting serial properties",
            return 0
        );
    }

    if (p_engine->ini_properties.scene_smd_path[0] != '\0') {
        shEngineError(
            shGetSceneProperties(p_engine->ini_properties.scene_smd_path, &p_engine->scene_smd, &p_engine->scene_properties) == 0,
            "shSetEngineState: failed getting scene properties",
            return 0
        );
    }
    
    if (p_engine->load_shared) {
        shEngineError(
            shGetSharedApplication(
                p_engine->application_properties.shared_name,
                p_engine->application_properties.s_start,
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

    if (p_engine->application_properties.additional_thread_count) {
        shAllocateThreads(
            p_engine->application_properties.additional_thread_count,
            &p_engine->thread_pool
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

    shEngineError(
        shApplicationRun(p_engine, p_engine->application_host.p_start) == 0,
        "shSetEngineState: application start function failed",
        return 0
    );

    return 1;
}

uint8_t shResetEngineState(
    ShEngine* p_engine
) {
    shEngineError(p_engine == NULL, "shResetEngineState: invalid engine memory", return 0);

    shEngineRelease(p_engine);

    return shSetEngineState(p_engine);
}

uint8_t shEngineFrameResize(
    ShEngine* p_engine
) {
    shEngineError(p_engine == NULL, "shEngineFrameResize: invalid engine memory", return 0);

    ShEngineVkCore* p_core   = &p_engine->core;
    ShWindow*       p_window = &p_engine->window;
    
    int _width  = 0;
    int _height = 0;
    glfwGetWindowSize(p_window->window, &_width, &_height);

    if (_width == 0 || _height == 0 || (_width == p_window->width && _height == p_window->height)) {
        p_window->width  = _width;
        p_window->height = _height;
        return 1;
    }

    if (p_engine->gui.core.swapchain_image_count != 0) {//is initialized
       
        shGuiResizeInterface(
            &p_engine->gui,
            p_window->width,
            p_window->height,
            _width,
            _height
        );
    }
    
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
    shGetPhysicalDeviceSurfaceSupport(p_core->physical_device, p_core->default_graphics_queue_family_index, p_core->surface, &graphics_supported);//always true
    shGetPhysicalDeviceSurfaceCapabilities(p_core->physical_device, p_core->surface, &p_core->surface_capabilities);
    shCreateSwapchain(
        p_core->device, p_core->physical_device, p_core->surface,
        p_core->swapchain_image_format,
        &p_core->swapchain_image_format,
        SH_ENGINE_SWAPCHAIN_IMAGE_COUNT,
        p_core->swapchain_image_sharing_mode,
        SH_FALSE,
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
        p_core->device, p_core->depth_image, 0, p_core->depth_image_memory
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
    
    if (p_engine->gui.core.swapchain_image_count != 0) {//is initialized

        ShGui* p_gui = &p_engine->gui;

        shGuiDestroyPipelines(p_gui);
        shGuiSetSurface(p_gui, p_engine->core.surface);

        shGuiSetRenderpass(p_gui, p_engine->core.renderpass);

        shGuiBuildRegionPipeline(
            p_gui,
            NULL,
            NULL
        );
        shGuiBuildCharPipeline(
            p_gui,
            NULL,
            NULL
        );
    }

    if (shApplicationRun(p_engine, p_engine->application_host.p_frame_resize) == 0) {
        shEngineManageState(p_engine, SH_ENGINE_NOT_READY);
    }
    
    shWaitDeviceIdle(p_core->device);

    return 1;
}

uint8_t shEngineProfilingUpdate(
    ShEngine* p_engine
) {
    shEngineError(p_engine == NULL, "shEngineProfilingUpdate: invalid engine memory", return 0);

    ShProfilingTimer* p_timer = &p_engine->profiling_timer;

    if (p_engine->time.now - p_timer->main_profiling_tool_last_time >= 1.0f) {
        p_timer->main_profiling_tool_last_time = p_engine->time.now;
        
        smdCommentLine(&p_timer->export, "\n\n\tENGINE PROFILING TIMERS\n\n");
        smdWriteLine  (&p_timer->export, 1, "MAIN_THREAD_____________________ms", SMD_VAR_TYPE_DOUBLE64, &p_timer->main_thread_dtime_ms);
        smdWriteLine  (&p_timer->export, 1, "APPLICATION_UPDATE______________ms", SMD_VAR_TYPE_DOUBLE64, &p_timer->application_update_dtime_ms);
        smdWriteLine  (&p_timer->export, 1, "APPLICATION_MAIN_CMD_BUFFER_____ms", SMD_VAR_TYPE_DOUBLE64, &p_timer->application_main_cmd_buffer_dtime_ms);
        smdWriteLine  (&p_timer->export, 1, "APPLICATION_MAIN_RENDERPASS_____ms", SMD_VAR_TYPE_DOUBLE64, &p_timer->application_main_renderpass_dtime_ms);
        for (uint32_t submission = 0; submission < SH_ENGINE_MAX_SWAPCHAIN_IMAGE_COUNT; submission++) {
            if (p_timer->main_cmd_buffer_wait_dtime_s[submission] != 0.0) {
                SmdLine submission_msg = { 0 };
                snprintf(submission_msg, SMD_VAR_NAME_MAX_SIZE, "MAIN_CMD_BUFFER_WAIT_%" PRIu32 "__________ms", submission);
                smdWriteLine(&p_timer->export, 1, submission_msg, SMD_VAR_TYPE_DOUBLE64, &p_timer->main_cmd_buffer_wait_dtime_s[submission]);
            }
        }
        if (p_timer->ext_count) {
            smdCommentLine(&p_timer->export, "\n\n\tDEVELOPER PROFILING TIMERS\n\n");
        }
        for (uint32_t ext_idx = 0; ext_idx < p_timer->ext_count; ext_idx++) {
            smdWriteLine(&p_timer->export, 1, p_timer->ext_names[ext_idx], SMD_VAR_TYPE_DOUBLE64, &p_timer->ext_dtime_ms[ext_idx]);
        }
        smdWriteFile(&p_timer->export, "profiling.smd");
        smdExportHandleRelease(&p_timer->export);
    }

    return 1;
}

uint8_t shEngineVulkanUpdate(
    ShEngine* p_engine
) {
    shEngineError(p_engine == NULL, "shEngineVulkanUpdate: invalid engine memory", return 0);

    ShEngineVkCore* p_core   = &p_engine->core;
    ShWindow*       p_window = &p_engine->window;

    if (p_window->width == 0 || p_window->height == 0 ) {
        return 1;
    }

    shAcquireSwapchainImage(
        p_core->device,//device
        p_core->swapchain,//swapchain
        UINT64_MAX,//timeout_ns
        p_core->current_image_acquired_semaphore,//acquired_signal_semaphore
        VK_NULL_HANDLE,//acquired_signal_fence
        &p_core->swapchain_image_idx,//p_swapchain_image_index
        &p_core->swapchain_suboptimal
    );

    shProfilingTimerStart(&p_engine->profiling_timer, SH_PROFILING_TIMER_MAIN_CMD_BUFFER_WAIT_0 + p_core->swapchain_image_idx);//might look a bit counter intuitive
    shWaitForFences(
        p_core->device,//device
        1,//fence_count
        &p_core->graphics_cmd_fences[p_core->swapchain_image_idx],//p_fences
        1,//wait_for_all
        UINT64_MAX//timeout_ns
    );
    shProfilingTimerEnd(&p_engine->profiling_timer, SH_PROFILING_TIMER_MAIN_CMD_BUFFER_WAIT_0 + p_core->swapchain_image_idx);//might look like a bit counter intuitive

    shResetFences(
        p_core->device,//device
        1,//fence_count
        &p_core->graphics_cmd_fences[p_core->swapchain_image_idx]//p_fences
    );

    VkCommandBuffer cmd_buffer = p_core->graphics_cmd_buffers[p_core->swapchain_image_idx];

    shBeginCommandBuffer(cmd_buffer);

    shProfilingTimerStart(&p_engine->profiling_timer, SH_PROFILING_TIMER_APPLICATION_MAIN_CMD_BUFFER);
    if (shApplicationRun(p_engine, p_engine->application_host.p_main_cmd_buffer) == 0) {
        shEngineManageState(p_engine, SH_ENGINE_NOT_READY);
    }
    shProfilingTimerEnd(&p_engine->profiling_timer, SH_PROFILING_TIMER_APPLICATION_MAIN_CMD_BUFFER);

    if (p_engine->gui.core.swapchain_image_count != 0) {
        shGuiWriteMemory(
            &p_engine->gui,
            cmd_buffer,
            0
        );
    }

    VkClearValue clear_values[2] = { 0 };

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

    shProfilingTimerStart(&p_engine->profiling_timer, SH_PROFILING_TIMER_APPLICATION_MAIN_RENDERPASS);
    if (shApplicationRun(p_engine, p_engine->application_host.p_main_renderpass) == 0) {
        shEngineManageState(p_engine, SH_ENGINE_NOT_READY);
    }
    shProfilingTimerEnd(&p_engine->profiling_timer, SH_PROFILING_TIMER_APPLICATION_MAIN_RENDERPASS);

    if (p_engine->gui.core.swapchain_image_count != 0) {
        shGuiRender(
            &p_engine->gui,
            cmd_buffer,
            p_core->swapchain_image_idx
        );
    }
    
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

    return 1;
}

uint8_t shEngineUpdateState(
    ShEngine* p_engine
) {
    ShEngineVkCore*    p_core             = &p_engine->core;
    ShWindow*          p_window           = &p_engine->window;
    ShApplicationHost* p_application_host = &p_engine->application_host;

    while (shIsWindowActive(*p_window)) {
        shProfilingTimerStart(&p_engine->profiling_timer, SH_PROFILING_TIMER_MAIN_THREAD);
        
        shUpdateWindow(p_engine);

        shEngineFrameResize(p_engine);
        
        if (
             shIsKeyDown(*p_window, SH_KEY_LEFT_CONTROL) &&
             shIsKeyPressed(*p_window, SH_KEY_R)
            ) {
                shEngineManageState(p_engine, shResetEngineState(p_engine));
                break;
        }


        shProfilingTimerStart(&p_engine->profiling_timer, SH_PROFILING_TIMER_APPLICATION_UPDATE);
        if (shApplicationRun(p_engine, p_application_host->p_update) == 0) {
            shEngineManageState(p_engine, SH_ENGINE_NOT_READY);
        }
        shProfilingTimerEnd(&p_engine->profiling_timer, SH_PROFILING_TIMER_APPLICATION_UPDATE);

        shEngineVulkanUpdate(p_engine);
        if (p_engine->gui.core.swapchain_image_count != 0) {//is initialized
            shGuiResetWidgetCount(&p_engine->gui);
        }
        shProfilingTimerEnd(&p_engine->profiling_timer, SH_PROFILING_TIMER_MAIN_THREAD);
        shEngineProfilingUpdate(p_engine);
    }

    shEngineRelease      (p_engine);
    shEngineVulkanRelease(p_engine);
    shClearWindow        (p_window);
    shFreeEngine         (p_engine);

    exit(0);
}

uint8_t shEngineManageState(
    ShEngine* p_engine,
    uint8_t   ready
) {
    shEngineError(p_engine == NULL, "shEngineManageState: invalid engine memory", return 0);

    if (ready) {
        if (!shEngineUpdateState(p_engine)) {
            shEngineRelease(p_engine);
            shEngineSafeState(p_engine);
        }
    }
    else {
        shEngineRelease(p_engine);
        shEngineSafeState(p_engine);
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

    shDestroyCommandBuffers(p_engine->core.device, p_engine->core.graphics_cmd_pool,  p_engine->core.swapchain_image_count,  p_engine->core.graphics_cmd_buffers);
    shDestroyCommandBuffers(p_engine->core.device, p_engine->core.present_cmd_pool,   1, &p_engine->core.present_cmd_buffer);
    shDestroyCommandBuffers(p_engine->core.device, p_engine->core.graphics_cmd_pool,  1, &p_engine->core.compute_cmd_buffer);
    shDestroyCommandBuffers(p_engine->core.device, p_engine->core.graphics_cmd_pool,  1, &p_engine->core.transfer_cmd_buffer);

    shDestroyCommandPool(p_engine->core.device, p_engine->core.graphics_cmd_pool);
    if (p_engine->core.default_graphics_queue_family_index != p_engine->core.default_present_queue_family_index) {
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
    ShEngine* p_engine
) {
    shEngineError(p_engine == NULL, "shEngineRelease: invalid engine memory", return 0);

    if (p_engine->application_host.p_close != NULL) {
        shEngineError(
            shApplicationRun(p_engine, p_engine->application_host.p_close) == 0,
            "shEngineRelease: failed closing application",
            return 0
        );
    }

    if (p_engine->thread_pool.p_handles != NULL) {
        shEngineError(
            shReleaseThreads(&p_engine->thread_pool) != SH_THREADS_SUCCESS,
            "shEngineRelease: failed releasing threads",
            return 0
        );
    }

    if (p_engine->gui.core.swapchain_image_count != 0) {
        shEngineGuiRelease(p_engine);
    }

    if (p_engine->application_host.shared) {
        shSharedRelease(&p_engine->application_host.shared);
    }

    for (uint32_t host_buffer_idx = 0; host_buffer_idx < p_engine->host_memory_properties.buffer_count; host_buffer_idx++) {
        free(p_engine->host_memory_properties.p_buffers_memory[host_buffer_idx]);
    }
    
    for (uint32_t vulkan_buffer_idx = 0; vulkan_buffer_idx < p_engine->vulkan_memory_properties.buffer_count; vulkan_buffer_idx++) {
        if (p_engine->vulkan_memory_properties.buffers       [vulkan_buffer_idx] != VK_NULL_HANDLE &&
            p_engine->vulkan_memory_properties.buffers_memory[vulkan_buffer_idx] != VK_NULL_HANDLE
        ) {
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
    }

    memset(&p_engine->ini_smd,           0, sizeof(SmdFileHandle));
    memset(&p_engine->application_smd,   0, sizeof(SmdFileHandle));
    memset(&p_engine->host_memory_smd,   0, sizeof(SmdFileHandle));
    memset(&p_engine->vulkan_memory_smd, 0, sizeof(SmdFileHandle));
    memset(&p_engine->scene_smd,         0, sizeof(SmdFileHandle));



    memset(&p_engine->ini_properties,           0, sizeof(ShIniProperties));
    memset(&p_engine->application_properties,   0, sizeof(ShApplicationProperties));
    memset(&p_engine->host_memory_properties,   0, sizeof(ShHostMemoryProperties));
    memset(&p_engine->vulkan_memory_properties, 0, sizeof(ShVulkanMemoryProperties));
    memset(&p_engine->scene_properties,         0, sizeof(ShSceneProperties));


    //memset(&p_engine->application_host, 0, sizeof(ShApplicationHandle));//we need to keep the functions information
    memset(&p_engine->thread_pool,    0, sizeof(ShThreadPool));
    memset(&p_engine->pipeline_pool,  0, sizeof(ShVkPipelinePool));

    
    shEndScene(p_engine);

    return 1;
}

void shEngineShutdown(
    ShEngine* p_engine
) {
    shEngineError(p_engine == NULL, "shEngineShutdown: invalid engine memory", return);

    shEngineRelease       (p_engine);
    shEngineVulkanRelease (p_engine);
    shClearWindow         (&p_engine->window);
    shFreeEngine          (p_engine);

    exit(0);
}


#ifdef __cplusplus
}
#endif//__cplusplus