#ifndef SH_ENGINE_VK_CORE_H
#define SH_ENGINE_VK_CORE_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



#include "shvulkan/shVulkan.h"



#define SH_ENGINE_SWAPCHAIN_IMAGE_COUNT         2
#define SH_ENGINE_MAX_SWAPCHAIN_IMAGE_COUNT     6
#define SH_ENGINE_RENDERPASS_ATTACHMENT_COUNT   3
#define SH_ENGINE_SUBASS_COLOR_ATTACHMENT_COUNT 1



typedef struct ShEngineVkCore {

	//uint32_t                         instance_extension_count;
	//char**                           pp_instance_extensions;

    VkInstance                       instance;
											                                      
	VkSurfaceKHR                     surface;
	VkSurfaceCapabilitiesKHR         surface_capabilities;
																	              
	VkPhysicalDevice                 physical_device;            
	VkPhysicalDeviceProperties       physical_device_properties;            
	VkPhysicalDeviceFeatures         physical_device_features;            
	VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
	
	uint32_t                         graphics_queue_family_count;
	uint32_t                         present_queue_family_count;
	uint32_t                         compute_queue_family_count;
	uint32_t                         transfer_queue_family_count;

	uint32_t                         graphics_queue_family_indices [SH_MAX_STACK_QUEUE_FAMILY_COUNT];
	uint32_t                         present_queue_family_indices  [SH_MAX_STACK_QUEUE_FAMILY_COUNT];
	uint32_t                         compute_queue_family_indices  [SH_MAX_STACK_QUEUE_FAMILY_COUNT];
	uint32_t                         transfer_queue_family_indices [SH_MAX_STACK_QUEUE_FAMILY_COUNT];

	uint32_t                         default_graphics_queue_family_index;
	uint32_t                         default_present_queue_family_index;
	uint32_t                         default_compute_queue_family_index;
	uint32_t                         default_transfer_queue_family_index;

	VkDevice                         device;
	uint32_t                         device_extension_count;
															                      
	VkQueue                          graphics_queue;
	VkQueue                          present_queue;
	VkQueue                          compute_queue;
	VkQueue                          transfer_queue;
					           						                              
	VkCommandPool                    graphics_cmd_pool;                            
	VkCommandPool                    present_cmd_pool;
															                      
	VkCommandBuffer                  graphics_cmd_buffers [SH_ENGINE_MAX_SWAPCHAIN_IMAGE_COUNT];
	VkCommandBuffer                  present_cmd_buffer;
	VkCommandBuffer                  compute_cmd_buffer;
	VkCommandBuffer                  transfer_cmd_buffer;

	VkFence                          graphics_cmd_fences  [SH_ENGINE_MAX_SWAPCHAIN_IMAGE_COUNT];
															                      
	VkSemaphore                      current_image_acquired_semaphore;    
	VkSemaphore                      current_graphics_queue_finished_semaphore;

	VkSwapchainKHR                   swapchain;                       
	VkSharingMode                    swapchain_image_sharing_mode;
	VkFormat                         swapchain_image_format;                       
	uint32_t                         swapchain_image_count;
																	              
	uint32_t                         sample_count;

	VkAttachmentDescription          input_color_attachment;             
	VkAttachmentReference            input_color_attachment_reference;             
	VkAttachmentDescription          depth_attachment;             
	VkAttachmentReference            depth_attachment_reference;             
	VkAttachmentDescription          resolve_attachment;             
	VkAttachmentReference            resolve_attachment_reference;             
	VkSubpassDescription             subpass;
																	              
	VkRenderPass                     renderpass;

	VkImage                          swapchain_images          [SH_ENGINE_MAX_SWAPCHAIN_IMAGE_COUNT];
	VkImageView                      swapchain_image_views     [SH_ENGINE_MAX_SWAPCHAIN_IMAGE_COUNT];
	VkImage                          depth_image;
	VkDeviceMemory                   depth_image_memory;
	VkImageView                      depth_image_view;
	VkImage                          input_color_image;
	VkDeviceMemory                   input_color_image_memory;
	VkImageView                      input_color_image_view;

	VkFramebuffer                    framebuffers              [SH_ENGINE_MAX_SWAPCHAIN_IMAGE_COUNT];

	uint32_t                         swapchain_image_idx;
	uint8_t                          swapchain_suboptimal;//not used

} ShEngineVkCore;



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_ENGINE_VK_CORE_H