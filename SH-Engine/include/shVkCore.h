#ifndef SH_VK_CORE_H
#define SH_VK_CORE_H

#include <vulkan/vulkan.h>
#include <stdint.h>

#include "shVulkanVersion.h"

#include "shWindow.h"

#include "shCglmImplementation.h"

typedef struct ShVkPipelineData ShVkPipelineData;

typedef struct ShMesh ShMesh;

typedef enum shImageType {
	SH_SWAPCHAIN_IMAGE = 0b001,
	SH_DEPTH_IMAGE = 0b010
} shImageType;

#define SH_DEPTH_IMAGE_FORMAT VK_FORMAT_D32_SFLOAT

#define SH_SWAPCHAIN_IMAGE_FORMAT VK_FORMAT_R8G8B8A8_UNORM

typedef struct ShQueue {
	VkQueueFlags	required_queue_flag;
	uint32_t		queue_family_index;
	VkQueue			queue;
} ShQueue;

typedef struct ShVkCore {
	/*Primary*/
	VkInstance					instance;
	VkPhysicalDevice			physical_device;
	VkPhysicalDeviceProperties	physical_device_properties;
	VkPhysicalDeviceFeatures	physical_device_features;
	VkDevice		device;
	/*Window and surface*/
	ShWindow		window;
	VkSurfaceKHR	surface;
	/*Queues*/
	ShQueue			graphics_queue;
	ShQueue			compute_queue;
	/*Commands*/
	VkCommandPool	cmd_pools[2];
	VkCommandBuffer cmd_buffers[2];
	/*Swapchain*/
	VkSwapchainKHR	swapchain;
	VkFormat		swapchain_image_format;
	uint32_t		swapchain_image_count;
	VkImage*		p_swapchain_images;
	VkImageView*	p_swapchain_image_views;
	VkFramebuffer*	p_frame_buffers;
	/*Depth buffer*/
	VkImage			depth_image;
	VkDeviceMemory	depth_image_memory;
	VkImageView		depth_image_view;
	/*Render pass + sync objects*/
	VkRenderPass	render_pass;
	VkSemaphore		render_semaphore;
	VkSemaphore		present_semaphore;
	VkFence			render_fence;
} ShVkCore;


extern void shCreateWindowSurface(ShVkCore* p_core);

extern VkSurfaceCapabilitiesKHR shGetSurfaceCapabilities(const VkPhysicalDevice pDevice, const VkSurfaceKHR surface);

extern ShVkCore shVkCoreInitPrerequisites(uint32_t width, uint32_t height, const char* title);

extern void shCreateInstance(ShVkCore* p_core);

extern void shSetPhysicalDevice(ShVkCore* p_core);

extern int CheckPhysicalDeviceExtensions(const ShVkCore core, const VkPhysicalDevice pDevice);

extern VkDeviceQueueCreateInfo shSetQueueInfo(const uint32_t queueFamilyIndex, const float* priority);

extern void shSetLogicalDevice(ShVkCore* p_core);

extern void shGetGraphicsQueue(ShVkCore* p_core);

extern void shGetComputeQueue(ShVkCore* p_core);

extern void shCreateSwapchain(ShVkCore* p_core);

extern void shGetSwapchainImages(ShVkCore* p_core);

extern void shCreateImageView(ShVkCore* p_core, const VkImage image, const shImageType type, VkImageView* p_image_view);

extern void shCreateSwapchainImageViews(ShVkCore* p_core);

extern void shInitSwapchainData(ShVkCore* p_core);

extern void shCreateDepthImageView(ShVkCore* p_core);

extern void shInitCommands(ShVkCore* p_core);

extern VkCommandPool shCreateCmdPool(const VkDevice device, uint32_t queueFamilyIndex);

extern VkCommandBuffer shCreateCmdBuffer(const VkDevice device, const VkCommandPool cmdPool);

extern void shCreateRenderPass(ShVkCore* p_core);

extern void shSetFramebuffers(ShVkCore* p_core);

extern void shSetSyncObjects(ShVkCore* p_core);

extern void shSwapchainRelease(ShVkCore* p_core);

extern void shDepthBufferRelease(ShVkCore* p_core);

extern void shSurfaceRelease(ShVkCore* p_core);

extern void shCmdRelease(ShVkCore* p_core);

extern void shRenderPassRelease(ShVkCore* p_core);

extern void shInstanceRelease(ShVkCore* p_core);

extern void shVulkanRelease(ShVkCore* p_core);

#endif