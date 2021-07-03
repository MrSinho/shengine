#ifndef PIPELINE_H
#define PIPELINE_H

#include <vulkan/vulkan.h>
#include "Window.h"

typedef struct PipelineData {

	uint32_t shaderStageCount;
	VkPipelineShaderStageCreateInfo* pShaderStages;

	VkVertexInputBindingDescription vertexBindingDescription;
	uint32_t vertexInputAttributeDescriptionCount;
	VkVertexInputAttributeDescription* pVertexInputAssemblyDescriptions;
	VkPipelineVertexInputStateCreateInfo vertexInputStateInfo;
	VkPipelineInputAssemblyStateCreateInfo inputAssembly;

	VkViewport viewport;
	VkRect2D scissor;
	VkPipelineViewportStateCreateInfo viewportState;

	VkPipelineRasterizationStateCreateInfo rasterizer;

	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	VkPipelineColorBlendStateCreateInfo colorBlendState;

	VkPipelineMultisampleStateCreateInfo multisampleStateInfo;

	VkPipelineLayout pipelineLayout;

	VkPipeline pipeline;


} PipelineData;


extern PipelineData PipelineDataInitPrerequisitites();

extern void CreateShaderStage(const VkDevice device, const char* shaderPath, VkPipelineShaderStageCreateInfo* shInfo, const VkShaderStageFlagBits stageFlag);

extern void SetVertexInputState(VkVertexInputBindingDescription* vertexBindDescription, uint32_t *vertexInputAttributeDescriptionCount, VkVertexInputAttributeDescription* pVertexInputAttributeDescriptions, VkPipelineVertexInputStateCreateInfo* vi);

extern void CreateInputAssembly(VkPipelineInputAssemblyStateCreateInfo* inputAssembly, VkPrimitiveTopology primitiveTopology, VkBool32 primitiveRestartEnable);

extern void CreateRasterizer(VkPipelineRasterizationStateCreateInfo* rasterizer);

extern void SetMultisampleState(VkPipelineMultisampleStateCreateInfo* multisampleState);

extern void ColorBlendSettings(VkPipelineColorBlendAttachmentState* colorAttachment, VkPipelineColorBlendStateCreateInfo* colorBlendState);

extern void SetViewport(const Window window, PipelineData *pipeData);

extern void SetupGraphicsPipeline(const VkData data, PipelineData* pipeData);

#endif