#ifndef FGG_VK_CORE_H
#define FGG_VK_CORE_H

#include <vulkan/vulkan.h>
#include <stdint.h>

#include "fggVulkanVersion.h"

#include "fggWindow.h"

#include "fggCglmImplementation.h"

typedef struct FggVkPipelineData FggVkPipelineData;

typedef struct FggMesh FggMesh;

typedef struct FggVkCore {
	
	/*Initialization stuff*/
	VkInstance instance;
	VkPhysicalDevice physical_device;
	VkPhysicalDeviceProperties physical_device_properties;
	VkDevice device;
	
	/*Window and surface stuff*/
	FggWindow window;
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

} FggVkCore;



extern void fggCreateWindowSurface(FggVkCore* p_core);

extern VkSurfaceCapabilitiesKHR fggGetSurfaceCapabilities(const VkPhysicalDevice pDevice, const VkSurfaceKHR surface);



extern FggVkCore fggVkCoreInitPrerequisites(uint32_t width, uint32_t height, const char* title);

extern void fggInitVulkan(FggVkCore* p_core);

extern void fggCreateInstance(FggVkCore* p_core);

extern void fggSetPhysicalDevice(FggVkCore* p_core);

extern int CheckPhysicalDeviceExtensions(const FggVkCore core, const VkPhysicalDevice pDevice);



extern VkDeviceQueueCreateInfo fggSetQueueInfo(const uint32_t queueFamilyIndex, const float* priority);

extern void fggSetLogicalDevice(FggVkCore* p_core);

extern void fggGetGraphicsQueue(FggVkCore* p_core);



extern void fggCreateSwapchain(FggVkCore* p_core);

extern void fggGetSwapchainImages(FggVkCore* p_core);

extern void fggCreateSwapchainImageViews(FggVkCore* p_core);

extern void fggInitSwapchainData(FggVkCore* p_core);



extern void fggInitCommands(FggVkCore* p_core);

extern VkCommandPool fggCreateCmdPool(const VkDevice device, uint32_t queueFamilyIndex);

extern VkCommandBuffer fggCreateCmdBuffer(const VkDevice device, const VkCommandPool cmdPool);



extern void fggCreateRenderPass(FggVkCore* p_core);

extern void fggSetFramebuffers(FggVkCore* p_core);

extern void fggSetSyncObjects(FggVkCore* p_core);

extern void fggSwapchainRelease(FggVkCore* p_core);

extern void fggSurfaceRelease(FggVkCore* p_core);

extern void fggCmdRelease(FggVkCore* p_core);

extern void fggRenderPassRelease(FggVkCore* p_core);

extern void fggInstanceRelease(FggVkCore* p_core);

extern void fggVulkanRelease(FggVkCore* p_core);

#endif