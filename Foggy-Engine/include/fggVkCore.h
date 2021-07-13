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
	VkPhysicalDevice physicalDevice;
	VkDevice device;
	
	/*FggWindow and surface stuff*/
	FggWindow window;
	VkSurfaceKHR surface;

	/*Queue stuff*/
	VkQueueFlags requiredQueueFlag;
	uint32_t queueFamilyIndexCount;
	uint32_t graphicsQueueIndex;
	uint32_t presentQueueIndex;
	VkQueue graphicsQueue;

	/*Command stuff*/
	VkCommandPool* pCmdPools;
	VkCommandBuffer* pCmdBuffers;

	/*Swapchain + images stuff*/
	VkSwapchainKHR swapchain;
	VkFormat imageFormat;
	uint32_t swapchainImageCount;
	VkImage* pSwapchainImages;
	VkImageView* pSwapchainImageViews;
	VkFramebuffer* pFramebuffers;

	/*Render pass + sync objects stuff*/
	VkRenderPass renderPass;
	VkSemaphore renderSemaphore;
	VkSemaphore presentSemaphore;
	VkFence renderFence;

} FggVkCore;



extern void fggCreateWindowSurface(const VkInstance instance, GLFWwindow* window, VkSurfaceKHR* surface);

extern VkSurfaceCapabilitiesKHR fggGetSurfaceCapabilities(const VkPhysicalDevice pDevice, const VkSurfaceKHR surface);



extern FggVkCore fggVkCoreInitPrerequisites(uint32_t width, uint32_t height, const char* title);

extern void fggInitVulkan(FggVkCore* core);

extern void fggCreateInstance(FggVkCore* core);

extern void fggSetPhysicalDevice(FggVkCore* core);

extern int CheckPhysicalDeviceExtensions(const FggVkCore core, const VkPhysicalDevice pDevice);



extern VkDeviceQueueCreateInfo fggSetQueueInfo(const uint32_t queueFamilyIndex, const float* priority);

extern void fggSetLogicalDevice(FggVkCore* core);

extern void fggGetGraphicsQueue(FggVkCore* core);



extern void fggCreateSwapchain(FggVkCore *core);

extern void fggGetSwapchainImages(FggVkCore *core);

extern void fggCreateSwapchainImageViews(FggVkCore *core);

extern void fggInitSwapchainData(FggVkCore *core);



extern void fggInitCommands(FggVkCore *core);

extern VkCommandPool fggCreateCmdPool(const VkDevice device, uint32_t queueFamilyIndex);

extern VkCommandBuffer fggCreateCmdBuffer(const VkDevice device, const VkCommandPool cmdPool);



extern void fggCreateRenderPass(FggVkCore *core);

extern void fggSetFramebuffers(FggVkCore *core);

extern void fggSetSyncObjects(FggVkCore *core);



extern void fggCleanup(FggVkCore *core);

#endif