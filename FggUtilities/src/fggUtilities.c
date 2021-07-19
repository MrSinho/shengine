#include "fggUtilities.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning (disable: 4996)
#pragma warning (disable: 6385)
#ifdef __GNU__
	#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif
const char *fggReadCode(const char* path, uint32_t *pCodeSize, const char *mode) {
	
	FILE* stream = fopen(path, mode);
	
	if (stream == NULL) {
		return (void*)(0);
	}

	fseek(stream, 0, SEEK_END);
	*pCodeSize = ftell(stream);
	fseek(stream, 0, SEEK_SET);

	char* code = (char *)malloc(*pCodeSize);

	if (stream != NULL && code != NULL) {
		fggCheckValue((int)fread(code, *pCodeSize, 1, stream), 1, "error reading buffer");
		return code;
	}

	free(code);

	return (void*)(0);
}

#ifndef NDEBUG

int fggCheckValidationLayer(const char* validationLayer) {

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



void fggCheckVkResult(VkResult result, const char* errormsg) {
	if (result != VK_SUCCESS) {
#ifndef NDEBUG
		printf("%s, %s \n", errormsg, fggTranslateVkResult(result));
#endif	
		exit(-1);
	}
}

void fggCheckValue(const int result, const int desiredValue, const char *errormsg) {
	if (result != desiredValue) {
		perror(errormsg);
		exit(EXIT_FAILURE);
	}
}

const char* fggTranslateVkResult(const VkResult vkResult) {
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

void fggCompileGLSLShader(const char* input, const char* output) {
	char cmd[256] = { 0 };
	const char* o = " -o ";
	strcpy(cmd, "glslc ");
	strncat(cmd, input, strlen(input));
	strncat(cmd, o, strlen(o));
	strncat(cmd, output, strlen(output));
#ifndef NDEBUG	
	puts(cmd);
#endif
	int rtrn = system(cmd);
}