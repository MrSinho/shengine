#ifndef FGG_VK_CORE_H
#define FGG_VK_CORE_H

#include <vulkan/vulkan.h>
#include <stdint.h>

#include "fggWindow.h"

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

extern void fggInitVulkan(FggVkCore* data);

extern void fggCreateInstance(FggVkCore* data);

extern void fggSetPhysicalDevice(FggVkCore* data);

extern int CheckPhysicalDeviceExtensions(const FggVkCore data, const VkPhysicalDevice pDevice);



extern VkDeviceQueueCreateInfo fggSetQueueInfo(const uint32_t queueFamilyIndex, const float* priority);

extern void fggSetLogicalDevice(FggVkCore* data);

extern void fggGetGraphicsQueue(FggVkCore* data);



extern void fggCreateSwapchain(FggVkCore *data);

extern void fggGetSwapchainImages(FggVkCore *data);

extern void fggCreateSwapchainImageViews(FggVkCore *data);

extern void fggInitSwapchainData(FggVkCore *data);



extern void fggInitCommands(FggVkCore *data);

extern VkCommandPool fggCreateCmdPool(const VkDevice device, uint32_t queueFamilyIndex);

extern VkCommandBuffer fggCreateCmdBuffer(const VkDevice device, const VkCommandPool cmdPool);



extern void fggCreateRenderPass(FggVkCore *data);

extern void fggSetFramebuffers(FggVkCore *data);

extern void fggSetSyncObjects(FggVkCore *data);

extern void fggDraw(FggVkCore *data, FggVkPipelineData* pipeData, const FggMesh mesh, const VkBuffer vertexBuffer);



extern void fggCleanup(FggVkCore *data);

#endif