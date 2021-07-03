#ifndef VULKAN_HANDLER_H
#define VULKAN_HANDLER_H

#include <vulkan/vulkan.h>
#include <stdint.h>

#include "Window.h"

typedef struct PipelineData PipelineData;
typedef struct Mesh Mesh;

typedef struct VkData {
	
	/*Initialization stuff*/
	VkInstance instance;
	VkPhysicalDevice physicalDevice;
	VkDevice device;
	
	/*Window and surface stuff*/
	Window window;
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

} VkData;

/*
*	Surface stuff
*/
extern void CreateWindowSurface(const VkInstance instance, GLFWwindow* window, VkSurfaceKHR* surface);
extern VkSurfaceCapabilitiesKHR GetSurfaceCapabilities(const VkPhysicalDevice pDevice, const VkSurfaceKHR surface);

/*
* First setup
*/
extern VkData VKDataInitPrerequisites(uint32_t width, uint32_t height, const char* title);
void InitVulkan(VkData* data);
extern void CreateInstance(VkData* data);
extern void SetPhysicalDevice(VkData* data);
extern int CheckPhysicalDeviceExtensions(const VkData data, const VkPhysicalDevice pDevice);

/*
*	Logical device creation 
*/

extern VkDeviceQueueCreateInfo SetQueueInfo(const uint32_t queueFamilyIndex, const float* priority);
extern void SetLogicalDevice(VkData* data);
extern void GetGraphicsQueue(VkData* data);

/*
*	Swapchain stuff
*/
extern void CreateSwapchain(VkData *data);
extern void GetSwapchainImages(VkData *data);
extern void CreateSwapchainImageViews(VkData *data);

/*
*	CmdPool + CmdBuffer stuff
*/
extern void InitCommands(VkData *data);
extern VkCommandPool CreateCommandPool(const VkDevice device, uint32_t queueFamilyIndex);
extern VkCommandBuffer CreateCmdBuffer(const VkDevice device, const VkCommandPool cmdPool);

/*
*	Render stuff
*/
extern void CreateRenderPass(VkData *data);
extern void SetFramebuffers(VkData *data);
extern void SetSyncObjects(VkData *data);
extern void Draw(VkData *data, PipelineData* pipeData, const Mesh mesh, const VkBuffer vertexBuffer);

/*
*/
extern void Cleanup(VkData *data);

#endif