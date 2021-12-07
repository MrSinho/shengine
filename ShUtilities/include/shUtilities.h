#ifndef SH_UTILITIES_H
#define SH_UTILITIES_H

#include <stdint.h>
#include <vulkan/vulkan.h>

extern int shCheckValidationLayer(const char* validationLayer);

extern const char* shTranslateVkResult(const VkResult vkResult);

extern void shCheckResult(int result, const char* errormsg);

extern void shCheckVkResult(VkResult result, const char* errormsg);

extern void shCheckValue(const int result, const int desiredValue, const char* errormsg);

extern void shCompileGLSLShader(const char* input, const char* output);

extern const char* shReadCode(const char* path, uint32_t* p_code_size, const char* mode);

#endif