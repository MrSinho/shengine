#include "shVkCheck.h"


#include <stdlib.h>

#ifndef NDEBUG

#include <stdio.h>
#include <string.h>

int shCheckValidationLayer(const char* validationLayer) {

	uint32_t availableLayerCount = 0;
	vkEnumerateInstanceLayerProperties(&availableLayerCount, NULL);

	VkLayerProperties* availableLayersProperties = (VkLayerProperties*)malloc(availableLayerCount * sizeof(VkLayerProperties));
	vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayersProperties);

	if (availableLayersProperties != NULL) {
		puts("Installed validation layers: ");
		for (uint32_t i = 0; i < availableLayerCount; i++) {
			puts(availableLayersProperties[i].layerName);
			if (strcmp(availableLayersProperties[i].layerName, validationLayer) == 0) {
				free(availableLayersProperties);
				return 1;
			}
		}
	}
	free(availableLayersProperties);

	return 0;
}

#endif

const char* shTranslateVkResult(const VkResult vkResult) {
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

void shCheckVkResult(VkResult result, const char* errormsg) {
	if (result != VK_SUCCESS) {
#ifndef NDEBUG
		printf("ShError: %s, %s\n", errormsg, shTranslateVkResult(result));
#endif // NDEBUG
		system("pause");
		exit(-1);
	}
}