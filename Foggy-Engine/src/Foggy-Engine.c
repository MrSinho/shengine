#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Foggy-Engine.h"
#include "Utilities.h"

/*
* First setup
*/

VkData VKDataInitPrerequisites(uint32_t width, uint32_t height, const char *title) {
	
	VkData vkComputeData = {
		0,						//instance
		0,						//physicalDevice
		0,						//device
		VK_QUEUE_GRAPHICS_BIT,	//requiredQueueFlag
		0,						//graphicsQueueIndex
		0,						//presentQueueIndex
		{NULL, 
		width, 
		height, 
		title},					//window					
		0,						//surface
		VK_NULL_HANDLE,			//computeQueue
		NULL,					//cmdPools
		0,						//cmdPoolCount
		NULL,					//cmdBuffers
		0,						//cmdBufferCount
		NULL,					//shaderModules
		0						//shaderModuleCount
	};

	InitGLFW(&vkComputeData.window);

	return vkComputeData;
}

void InitVulkan(VkData *vkComputeData) {
	CreateInstance(vkComputeData);
	CreateWindowSurface(vkComputeData->instance, vkComputeData->window.window, &vkComputeData->surface);
	SetPhysicalDevice(vkComputeData);
	SetLogicalDevice(vkComputeData);
}

void CreateInstance(VkData* vkComputeData) {

	VkApplicationInfo applicationInfo = {
		VK_STRUCTURE_TYPE_APPLICATION_INFO,
		NULL,
		"VK-Compute Application",
		VK_MAKE_VERSION(0, 1, 0),
		"VK-Compute Engine",
		VK_MAKE_VERSION(0, 1, 0),
		VK_API_VERSION_1_2,
	};

	const char* khronos_validation = "VK_LAYER_KHRONOS_validation";

	uint32_t instanceExtensionsCount;
	const char** extensionNames = glfwGetRequiredInstanceExtensions(&instanceExtensionsCount);

	VkInstanceCreateInfo instanceCreateInfo = {
		VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		NULL,
		0,
		&applicationInfo,
		0,
		NULL,
		instanceExtensionsCount,
		&extensionNames[0]
	};

#ifndef NDEBUG
		if (CheckValidationLayer(khronos_validation)) {
			instanceCreateInfo.enabledLayerCount = 1;
			instanceCreateInfo.ppEnabledLayerNames = &khronos_validation;
		}

		puts("creating vkinstance");
#endif

	CheckVkResult(
		vkCreateInstance(&instanceCreateInfo, VK_NULL_HANDLE, &vkComputeData->instance),
		"error creating vkinstance"
	);
}

void CreateWindowSurface(const VkInstance instance, const GLFWwindow *window, VkSurfaceKHR *surface) {
	
	CheckVkResult(
		glfwCreateWindowSurface(instance, window, NULL, surface),
		"error creating window surface"
	);
}

void SetPhysicalDevice(VkData* vkComputeData) {

	uint32_t pDeviceCount = 0;
	vkEnumeratePhysicalDevices(vkComputeData->instance, &pDeviceCount, NULL);

	VkPhysicalDevice *pDevices = (VkPhysicalDevice*)malloc(pDeviceCount*sizeof(VkPhysicalDevice));
	vkEnumeratePhysicalDevices(vkComputeData->instance, &pDeviceCount, pDevices);

	if (pDeviceCount == 0) {
		printf("No Vulkan compatible gpu has been found");
		exit(-1);
	}

	uint32_t* graphicsQueueFamilyIndices = (uint32_t*)malloc(pDeviceCount * sizeof(uint32_t));
	uint32_t* surfaceQueueFamilyIndices  = (uint32_t*)malloc(pDeviceCount * sizeof(uint32_t));
	uint32_t suitableDeviceCount = 0;

	for (uint32_t i = 0; i < pDeviceCount; i++) { //DEVICE LOOP

		uint32_t queueFamilyPropertyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(pDevices[i], &queueFamilyPropertyCount, NULL);
		VkQueueFamilyProperties* pQueueFamilyProperties = (VkQueueFamilyProperties*)malloc(queueFamilyPropertyCount * sizeof(VkQueueFamilyProperties));
		vkGetPhysicalDeviceQueueFamilyProperties(pDevices[i], &queueFamilyPropertyCount, pQueueFamilyProperties);
		
		VkBool32 surfaceSupport = 0;

		for (uint32_t j = 0; j < queueFamilyPropertyCount; j++) {

			//SURFACE SUPPORT
			if (!surfaceSupport) {
				vkGetPhysicalDeviceSurfaceSupportKHR(pDevices[i], j, vkComputeData->surface, &surfaceSupport);
				if (surfaceSupport) {
					surfaceQueueFamilyIndices[i] = j;
				}
			}

			// GRAPHICS QUEUE
			if (pQueueFamilyProperties[j].queueFlags & vkComputeData->requiredQueueFlag) {
				graphicsQueueFamilyIndices[i] = j;
			}

			//SUITABLE
			if (pQueueFamilyProperties[j].queueFlags & vkComputeData->requiredQueueFlag && surfaceSupport) {
				suitableDeviceCount += 1;
			}
		}
	}

	if (suitableDeviceCount == 0) {
		puts("No suitable device has been found");
		exit(-1);
	}

	uint32_t *scores = calloc(suitableDeviceCount, sizeof(uint32_t));

	for (uint32_t i = 0; i < suitableDeviceCount; i++) {

		VkPhysicalDeviceProperties pDeviceProperties;
		vkGetPhysicalDeviceProperties(pDevices[i], &pDeviceProperties);

		VkPhysicalDeviceFeatures pDeviceFeatures;
		vkGetPhysicalDeviceFeatures(pDevices[i], &pDeviceFeatures);
		
		scores[i] += pDeviceProperties.limits.maxComputeSharedMemorySize;
		scores[i] += pDeviceProperties.limits.maxComputeWorkGroupInvocations;

	}

	if (suitableDeviceCount > 1) {

		for (uint32_t i = 1; i < suitableDeviceCount; i++) {
			if (scores[i] > scores[i - 1]) {
				vkComputeData->physicalDevice = pDevices[i];
				vkComputeData->graphicsQueueIndex = graphicsQueueFamilyIndices[i];
				vkComputeData->presentQueueIndex = surfaceQueueFamilyIndices[i];
			}
			else {
				vkComputeData->physicalDevice = pDevices[i - 1 ];
				vkComputeData->graphicsQueueIndex = graphicsQueueFamilyIndices[i - 1];
				vkComputeData->presentQueueIndex = surfaceQueueFamilyIndices[i - 1];
			}
		}
	}
	else {
		vkComputeData->physicalDevice = pDevices[0];
		vkComputeData->graphicsQueueIndex = graphicsQueueFamilyIndices[0];
		vkComputeData->presentQueueIndex = surfaceQueueFamilyIndices[0];
	}

#ifndef NDEBUG
		VkPhysicalDeviceProperties physicalDeviceProperties;
		vkGetPhysicalDeviceProperties(vkComputeData->physicalDevice, &physicalDeviceProperties);
		printf("using %s \n", physicalDeviceProperties.deviceName);
#endif

}

/*
*	Logical device creation + command buffers
*/

VkDeviceQueueCreateInfo CreateQueue(const uint32_t queueFamilyIndex, const float *priority) {
	
	VkDeviceQueueCreateInfo queueCreateInfo = {
		VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		NULL,
		0,
		queueFamilyIndex,
		1,
		priority,
	};
	
	return queueCreateInfo;
}

void SetLogicalDevice(VkData *vkComputeData) {
	
	const float queuePriority = 1.0f;
	VkDeviceQueueCreateInfo graphicsQueueInfo = CreateQueue(vkComputeData->graphicsQueueIndex, &queuePriority);

	VkDeviceCreateInfo deviceCreateInfo = {
		VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		NULL,
		0,
		1, 
		&graphicsQueueInfo,
		0, 
		NULL,
		0, 
		NULL,
		NULL
	};

#ifndef NDEBUG
	puts("creating logical device");
#endif

	CheckVkResult(
		vkCreateDevice(vkComputeData->physicalDevice, &deviceCreateInfo, NULL, &vkComputeData->device),
		"error creating logical device"
	);
		
}

VkCommandPool CreateCommandPool(const VkDevice device, uint32_t queueFamilyIndex) {

	VkCommandPoolCreateInfo cmdPoolCreateInfo = {
		VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		NULL,
		0,
		queueFamilyIndex
	};


#ifndef NDEBUG
	puts("creating command pool");
#endif

	VkCommandPool cmdPool = NULL;
	CheckVkResult(
		vkCreateCommandPool(device, &cmdPoolCreateInfo, NULL, &cmdPool), 
		"error creating command pool"
	);

	return cmdPool;
}

VkCommandBuffer CreateCmdBuffer(const VkDevice device, const VkCommandPool cmdPool) {
	VkCommandBufferAllocateInfo cmdBufferAllocateInfo = {
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		NULL,
		cmdPool,
		VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		1
	};

#ifndef NDEBUG
	puts("creating command buffer");
#endif

	VkCommandBuffer cmdBuffer;
	CheckVkResult(
		vkAllocateCommandBuffers(device, &cmdBufferAllocateInfo, &cmdBuffer), 
		"error creating command buffer"
	);

	return cmdBuffer;
}

VkShaderModule CreateShaderModule(const VkDevice device, const char* input, const char* output) {

	Compile_glslc_Shader(input, output);
	const char *shaderBinaryCode = ReadCode(output, "rb");
	size_t shaderbSize = sizeof(shaderBinaryCode);

	VkShaderModuleCreateInfo shaderModuleCreateInfo = {
		VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		NULL,
		0,
		shaderbSize/sizeof(char),
		(const uint32_t*)(shaderBinaryCode)
	};

#ifndef NDEBUG
	puts("creating shader module using binary at: ");
#endif

	VkShaderModule shaderModule;
	CheckVkResult(
		vkCreateShaderModule(device, &shaderModuleCreateInfo, NULL, &shaderModule),
		"error creating shader module"
	);

	return shaderModule;
}

#ifndef NDEBUG

int CheckValidationLayer(const char* validationLayer) {
	
	uint32_t availableLayerCount = 0;
	vkEnumerateInstanceLayerProperties(&availableLayerCount, NULL);

	VkLayerProperties *availableLayersProperties = (VkLayerProperties*)malloc(availableLayerCount * sizeof(VkLayerProperties));
	vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayersProperties);
	
	if (availableLayersProperties) {
		puts("Installed validation layers: ");
		for (uint32_t i = 0; i < availableLayerCount; i++) {
			puts(availableLayersProperties[i].layerName);
			if (strcmp(availableLayersProperties[i].layerName, validationLayer) == 0) {
				return 1;
			}
		}
	}

	return 0;
}

#endif

/*
*	Utilities
*/

void CheckVkResult(VkResult result, const char* errormsg) {
	if (result != VK_SUCCESS) {
#ifndef NDEBUG
		printf("%s, %s \n", errormsg, TranslateVkResult(result));
#endif	
		exit(-1);
	}
}

const char* TranslateVkResult(const VkResult vkResult) {
	switch (vkResult) {
	case VK_SUCCESS:return "VK_SUCCESS";
	case VK_NOT_READY:return "VK_NOT_READY";
	case VK_TIMEOUT:return "VK_TIMEOUT";
	case VK_EVENT_SET:return "VK_EVENT_SET";
	case VK_EVENT_RESET:return "VK_EVENT_RESET";
	case VK_INCOMPLETE:return "VK_INCOMPLETE";
	case VK_ERROR_OUT_OF_HOST_MEMORY:return "VK_ERROR_OUT_OF_HOST_MEMORY";
	case VK_ERROR_OUT_OF_DEVICE_MEMORY:return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
	case VK_ERROR_INITIALIZATION_FAILED:return "VK_ERROR_INITIALIZATION_FAILED";
	case VK_ERROR_DEVICE_LOST:return "VK_ERROR_DEVICE_LOST";
	case VK_ERROR_MEMORY_MAP_FAILED:return "VK_ERROR_MEMORY_MAP_FAILED";
	case VK_ERROR_LAYER_NOT_PRESENT:return "VK_ERROR_LAYER_NOT_PRESENT";
	case VK_ERROR_EXTENSION_NOT_PRESENT:return "VK_ERROR_EXTENSION_NOT_PRESENT";
	case VK_ERROR_FEATURE_NOT_PRESENT:return "VK_ERROR_FEATURE_NOT_PRESENT";
	case VK_ERROR_INCOMPATIBLE_DRIVER:return "VK_ERROR_INCOMPATIBLE_DRIVER";
	case VK_ERROR_TOO_MANY_OBJECTS:return "VK_ERROR_TOO_MANY_OBJECTS";
	case VK_ERROR_FORMAT_NOT_SUPPORTED:return "VK_ERROR_FORMAT_NOT_SUPPORTED";
	case VK_ERROR_SURFACE_LOST_KHR:return "VK_ERROR_SURFACE_LOST_KHR";
	case VK_SUBOPTIMAL_KHR:return "VK_SUBOPTIMAL_KHR";
	case VK_ERROR_OUT_OF_DATE_KHR:return "VK_ERROR_OUT_OF_DATE_KHR";
	case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
	case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
	case VK_ERROR_VALIDATION_FAILED_EXT:return "VK_ERROR_VALIDATION_FAILED_EXT";
	case VK_RESULT_MAX_ENUM:return "VK_RESULT_MAX_ENUM";
	}
	return "unknown vkresult";
}

const char* TranslateQueueFlags(const VkQueueFlags queueFlag) {
	if (queueFlag & VK_QUEUE_GRAPHICS_BIT) { return "VK_QUEUE_GRAPHICS_BIT"; }
	if (queueFlag & VK_QUEUE_COMPUTE_BIT) { return "VK_QUEUE_COMPUTE_BIT"; }
	if (queueFlag & VK_QUEUE_TRANSFER_BIT) { return "VK_QUEUE_TRANSFER_BIT"; }
	if (queueFlag & VK_QUEUE_SPARSE_BINDING_BIT) { return "VK_QUEUE_SPARSE_BINDING_BIT"; }
	if (queueFlag & VK_QUEUE_PROTECTED_BIT) { return "VK_QUEUE_PROTECTED_BIT"; }
#ifdef VK_ENABLE_BETA_EXTENSIONS
	if (queueFlags & VK_QUEUE_VIDEO_DECODE_BIT_KHR) { return "VK_QUEUE_VIDEO_DECODE_BIT_KHR"; }
#endif											
#ifdef VK_ENABLE_BETA_EXTENSIONS				
	if (queueFlags & VK_QUEUE_VIDEO_ENCODE_BIT_KHR) { return "VK_QUEUE_VIDEO_ENCODE_BIT_KHR"; }
#endif											
	if (queueFlag & VK_QUEUE_FLAG_BITS_MAX_ENUM) { return "VK_QUEUE_FLAG_BITS_MAX_ENUM"; }
	return "unknown flag";
}

void Compile_glslc_Shader(const char* input, const char* output) {
	char* cmd = "glslc ";
	strcat_s(cmd, sizeof(cmd), input);
	strcat_s(cmd, sizeof(cmd), "-o");
	strcat_s(cmd, sizeof(cmd), output);
#ifndef NDEBUG	
	puts(cmd);
#endif
	system(cmd);
}