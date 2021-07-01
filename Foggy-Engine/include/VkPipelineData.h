#ifndef PIPELINE_H
#define PIPELINE_H

#include <vulkan/vulkan.h>
#include "Window.h"

typedef struct PipelineData {

	uint32_t shaderStageCount;
	VkPipelineShaderStageCreateInfo* pShaderStages;

	VkPipelineVertexInputStateCreateInfo vertexInputStateInfo;
	VkPipelineInputAssemblyStateCreateInfo inputAssembly;

	VkPipelineViewportStateCreateInfo viewportState;

	VkPipelineRasterizationStateCreateInfo rasterizer;
	VkPipelineColorBlendAttachmentState colorBlendAttachment;

	VkPipelineMultisampleStateCreateInfo multisampleStateInfo;

	VkPipelineLayout pipelineLayout;

	VkPipeline pipeline;


} PipelineData;


extern PipelineData PipelineDataInitPrerequisitites();

extern void CreateShaderStage(const VkDevice device, const char* shaderPath, VkPipelineShaderStageCreateInfo* shInfo, const VkShaderStageFlagBits stageFlag);

extern void SetVertexInputState(VkPipelineVertexInputStateCreateInfo* vi);

extern void CreateInputAssembly(VkPipelineInputAssemblyStateCreateInfo* inputAssembly, VkPrimitiveTopology primitiveTopology, VkBool32 primitiveRestartEnable);

extern void CreateRasterizer(VkPipelineRasterizationStateCreateInfo* rasterizer);

extern void SetMultisampleState(VkPipelineMultisampleStateCreateInfo* multisampleState);

extern void ColorBlendSettings(VkPipelineColorBlendAttachmentState* colorBlendAttachment);

extern void SetViewport(const Window window, VkPipelineViewportStateCreateInfo *viewportState);

extern void SetupGraphicsPipeline(const VkDevice device, PipelineData *data);

#endif