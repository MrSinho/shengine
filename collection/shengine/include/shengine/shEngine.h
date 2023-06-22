#ifndef SH_ENGINE_H
#define SH_ENGINE_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



#include "shengine/shWindow.h"
#include "shengine/shTime.h"
#include "shengine/shInput.h"
#include "shengine/shScene.h"

#include "shsharedhost/shSharedHost.h"
#include "shenvironment/shEnvironment.h"

#include <shthreads/shthreads.h>
#include <shgui/shgui.h>
#include <smd/smd.h>



#define SH_ENGINE_SWAPCHAIN_IMAGE_COUNT       2
#define SH_ENGINE_MAX_SWAPCHAIN_IMAGE_COUNT   6
#define SH_ENGINE_RENDERPASS_ATTACHMENT_COUNT 3



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
																	              
	uint32_t                         graphics_queue_family_index;
	uint32_t                         present_queue_family_index;
																	              
	VkDevice                         device;
	uint32_t                         device_extension_count;
															                      
	VkQueue                          graphics_queue;
	VkQueue                          present_queue;
					           						                              
	VkCommandPool                    graphics_cmd_pool;                            
	VkCommandPool                    present_cmd_pool;
															                      
	VkCommandBuffer                  graphics_cmd_buffers [SH_ENGINE_MAX_SWAPCHAIN_IMAGE_COUNT];
	VkCommandBuffer                  present_cmd_buffer;
															                      
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

} ShEngineVkCore;



typedef struct ShEngine {
    ShEngineVkCore           core;
						     
    ShWindow                 window;
    ShTime                   time;
    ShGui*                   p_gui;
                             
    SmdFileHandle*           p_ini_smd;
	SmdFileHandle*           p_application_smd;
	SmdFileHandle*           p_host_memory_smd;
	SmdFileHandle*           p_vulkan_memory_smd;
	SmdFileHandle*           p_scene_smd;
	                    
	ShIniProperties          ini_properties;
	ShApplicationProperties  application_properties;
	ShHostMemoryProperties   host_memory_properties;
	ShVulkanMemoryProperties vulkan_memory_properties;
	ShSceneProperties        scene_properties;

    ShApplicationHandle      application_host;
	ShThreadPool             thread_pool;
	ShThreadState            app_thread_state;
							 
	ShVkPipelinePool         pipeline_pool;
	uint32_t                 pipeline_count;
							 
                        	 
    void*                    p_ext;

} ShEngine;

#define shAllocateEngine() calloc(1, sizeof(ShEngine));
#define shFreeEngine(p_engine) free(p_engine); (p_engine) = NULL

#define SH_APPLICATION_THREAD_IDX 0



static uint8_t shEngineWarning(int condition, const char* msg) {
    if ((int)(condition)) { printf("shengine warning: %s.\n", msg); return 1; }
    return 0;
}

#define shEngineError(condition, msg, failure_expression)\
	if ((int)(condition)) { printf("shengine error: %s.\n", msg); failure_expression; }



extern uint8_t shSetEngineState(
	ShEngine* p_engine,
	uint8_t   load_shared
);

extern uint8_t shResetEngineState(
	ShEngine* p_engine,
	uint8_t   release_shared
);

extern uint8_t shEngineSafeState(
	ShEngine* p_engine
);

extern uint8_t shEngineUpdateState(
	ShEngine* p_engine
);

extern uint8_t shEngineManageState(
	ShEngine* p_engine,
	uint8_t   ready,
	uint8_t   release_on_failure
);

extern uint8_t shEngineVulkanRelease(
	ShEngine* p_engine
);

extern uint8_t shEngineRelease(
	ShEngine* p_engine,
	uint8_t   release_shared
);

extern void shEngineShutdown(
	ShEngine* p_engine
);



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_ENGINE_H