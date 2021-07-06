#include "Utilities.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning (disable: 4996)
#pragma warning (disable: 6385)

const char *ReadCode(const char* path, uint32_t *_codeSize, const char *mode) {
	
	FILE* stream = fopen(path, mode);
	
	if (stream == NULL) {
		return (void*)(0);
	}

	fseek(stream, 0, SEEK_END);
	*_codeSize = ftell(stream);
	fseek(stream, 0, SEEK_SET);

	char* code = (char *)malloc(*_codeSize);

	if (stream != NULL && code != NULL) {
		fread(code, *_codeSize, 1, stream);
		return code;
	}

	free(code);

	return (void*)(0);
}

#ifndef NDEBUG

int CheckValidationLayer(const char* validationLayer) {

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

void BuildShader(const char* input, const char* output) {
	char cmd[256] = { 0 };
	const char* o = " -o ";
	strcpy(cmd, "glslc ");
	strncat(cmd, input, strlen(input));
	strncat(cmd, o, strlen(o));
	strncat(cmd, output, strlen(output));
#ifndef NDEBUG	
	puts(cmd);
#endif
	system(cmd);
}