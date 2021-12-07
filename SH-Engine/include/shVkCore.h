#ifndef SH_VK_CORE_H
#define SH_VK_CORE_H

#include <vulkan/vulkan.h>
#include <stdint.h>

#include "shVulkanVersion.h"

#include "shWindow.h"

#include "shCglmImplementation.h"

typedef struct ShVkPipelineData ShVkPipelineData;

typedef struct ShMesh ShMesh;

typedef struct ShVkCore {
	
	/*Initialization stuff*/
	VkInstance instance;
	VkPhysicalDevice physical_device;
	VkPhysicalDeviceProperties physical_device_properties;
	VkDevice device;
	
	/*Window and surface stuff*/
	ShWindow window;
	VkSurfaceKHR surface;

	/*Queues*/
	VkQueueFlags required_queue_flag;
	uint32_t queue_family_index_count;
	uint32_t graphics_queue_index;
	uint32_t present_queue_index;
	VkQueue graphics_queue;

	/*Commands*/
	VkCommandPool* p_cmd_pools;
	VkCommandBuffer* p_cmd_buffers;

	/*Swapchain*/
	VkSwapchainKHR swapchain;
	VkFormat image_format;
	uint32_t swapchain_image_count;
	VkImage* p_swapchain_images;
	VkImageView* p_swapchain_image_views;
	VkFramebuffer* p_frame_buffers;

	/*Render pass + sync objects*/
	VkRenderPass render_pass;
	VkSemaphore render_semaphore;
	VkSemaphore present_semaphore;
	VkFence render_fence;

} ShVkCore;



extern void shCreateWindowSurface(ShVkCore* p_core);

extern VkSurfaceCapabilitiesKHR shGetSurfaceCapabilities(const VkPhysicalDevice pDevice, const VkSurfaceKHR surface);



extern ShVkCore shVkCoreInitPrerequisites(uint32_t width, uint32_t height, const char* title);

extern void shInitVulkan(ShVkCore* p_core);

extern void shCreateInstance(ShVkCore* p_core);

extern void shSetPhysicalDevice(ShVkCore* p_core);

extern int CheckPhysicalDeviceExtensions(const ShVkCore core, const VkPhysicalDevice pDevice);



extern VkDeviceQueueCreateInfo shSetQueueInfo(const uint32_t queueFamilyIndex, const float* priority);

extern void shSetLogicalDevice(ShVkCore* p_core);

extern void shGetGraphicsQueue(ShVkCore* p_core);



extern void shCreateSwapchain(ShVkCore* p_core);

extern void shGetSwapchainImages(ShVkCore* p_core);

extern void shCreateSwapchainImageViews(ShVkCore* p_core);

extern void shInitSwapchainData(ShVkCore* p_core);



extern void shInitCommands(ShVkCore* p_core);

extern VkCommandPool shCreateCmdPool(const VkDevice device, uint32_t queueFamilyIndex);

extern VkCommandBuffer shCreateCmdBuffer(const VkDevice device, const VkCommandPool cmdPool);



extern void shCreateRenderPass(ShVkCore* p_core);

extern void shSetFramebuffers(ShVkCore* p_core);

extern void shSetSyncObjects(ShVkCore* p_core);

extern void shSwapchainRelease(ShVkCore* p_core);

extern void shSurfaceRelease(ShVkCore* p_core);

extern void shCmdRelease(ShVkCore* p_core);

extern void shRenderPassRelease(ShVkCore* p_core);

extern void shInstanceRelease(ShVkCore* p_core);

extern void shVulkanRelease(ShVkCore* p_core);

#endif