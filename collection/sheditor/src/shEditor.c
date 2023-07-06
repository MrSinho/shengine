#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>



#include <shsharedhost/shSharedHost.h>
#include <shenvironment/shEnvironment.h>

#include <shengine/shEngine.h>
#include <shenvironment/shEnvironment.h>

#include "sheditor/shEditor.h"




#define SH_ENGINE_RENDERPASS_ATTACHMENT_COUNT 3
#define SH_ENGINE_SWAPCHAIN_IMAGE_COUNT       2
#define SH_ENGINE_MAX_SWAPCHAIN_IMAGE_COUNT   6



uint8_t shEditorSetupVulkan(
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
		p_engine->core.present_cmd_pool,//cmd_pool
		1,//cmd_buffer_count
		&p_engine->core.present_cmd_buffer//p_cmd_buffer
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


int shEditorMain(ShEngine* p_engine) {

	char window_title[256];
	if (p_engine->window.title != NULL) {
		strcpy(window_title, p_engine->window.title);
	}
	else {
		strcpy(window_title, "shengine editor");
	}
	shWindowSetup(window_title, &p_engine->window);

	shEditorSetupVulkan(p_engine);

#ifdef SH_APPLICATION_TARGET_TYPE_EXECUTABLE
	shEngineManageState(p_engine, shSetEngineState(p_engine, 0), 0, 1);
#else
	shEngineManageState(p_engine, shSetEngineState(p_engine, 1), 1, 1);
#endif//SH_APPLICATION_TARGET_TYPE_EXECUTABLE

	return 0;
}


#ifndef SH_APPLICATION_TARGET_TYPE_EXECUTABLE
int main() {
	ShEngine* p_engine = shAllocateEngine();
	return shEditorMain(p_engine);
}
#endif//SH_APPLICATION_TARGET_TYPE_EXECUTABLE

#ifdef __cplusplus
}
#endif//__cplusplus
