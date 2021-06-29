#ifndef VULKAN_HANDLER_H
#define VULKAN_HANDLER_H

#include <vulkan/vulkan.h>
#include <stdint.h>

#include "Window.h"

typedef struct VkData {
	
	VkInstance instance;
	VkPhysicalDevice physicalDevice;
	VkDevice device;

	VkQueueFlags requiredQueueFlag;

	uint32_t graphicsQueueIndex;
	uint32_t presentQueueIndex;

	Window window;
	VkSurfaceKHR surface;

	VkSwapchainKHR swapchain;
	
	VkFormat imageFormat;

	uint32_t swapchainImageCount;
	VkImage* pSwapchainImages;

	uint32_t swapchainImageViewCount;
	VkImageView* pSwapchainImageViews;

	VkQueue graphicsQueue;

	uint32_t cmdPoolCount;
	VkCommandPool *pCmdPools;

	uint32_t cmdBufferCount;
	VkCommandBuffer *pCmdBuffers;

	uint32_t shaderModuleCount;
	VkShaderModule *pShaderModules;

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
*	Pipeline stuff
*/
extern VkShaderModule CreateShaderModule(const VkDevice device, const char* input, const char* output);

/*
*	Utilities
*/

extern int CheckValidationLayer(const char *validationLayer);
extern const char* TranslateVkResult(const VkResult vkResult);
extern const char* TranslateQueueFlags(const VkQueueFlags queueFlag);
extern void CheckVkResult(VkResult result, const char* errormsg);
extern void Compile_glslc_Shader(const char* input, const char* output);
extern void Cleanup(VkData *data);

#endif