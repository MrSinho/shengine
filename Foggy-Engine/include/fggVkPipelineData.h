#ifndef FGG_PIPELINE_DATA_H
#define FGG_PIPELINE_DATA_H

#include <vulkan/vulkan.h>

#include "fggWindow.h"
#include "fggMesh.h"

typedef struct FggVkFixedStates {
	
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

} FggVkFixedStates;

typedef struct FggVkPipelineData {

	uint32_t shaderStageCount;
	VkPipelineShaderStageCreateInfo* pShaderStages;
	uint32_t shaderModuleCount;
	VkShaderModule* pShaderModules;

	VkPipelineLayout mainPipelineLayout;

	VkPipeline pipeline;

} FggVkPipelineData;


extern void fggCreateRasterizer(VkPipelineRasterizationStateCreateInfo* rasterizer);

extern void fggSetMultisampleState(VkPipelineMultisampleStateCreateInfo* multisampleState);

extern void fggColorBlendSettings(VkPipelineColorBlendAttachmentState* colorAttachment, VkPipelineColorBlendStateCreateInfo* colorBlendState);

extern void fggSetViewport(const FggWindow window, VkViewport *vprt, VkRect2D* scssr, VkPipelineViewportStateCreateInfo* vprtState);

extern void fggSetFixedStates(const FggVkCore core, FggVkFixedStates* pipeData);


 
extern void fggCreateShaderModule(const VkDevice device, const char* input, VkShaderModule* shaderModule);

extern void fggCreateShaderStage(const VkDevice device, const VkShaderModule shModule, const char* shaderPath, const VkShaderStageFlagBits stageFlag, VkPipelineShaderStageCreateInfo* pShInfo);

extern void fggSetVertexInputState(VkVertexInputBindingDescription* vertexBindDescription, uint32_t* vertexInputAttributeDescriptionCount, VkVertexInputAttributeDescription* pVertexInputAttributeDescriptions, VkPipelineVertexInputStateCreateInfo* vi);

extern void fggCreateInputAssembly(VkPipelineInputAssemblyStateCreateInfo* inputAssembly, VkPrimitiveTopology primitiveTopology, VkBool32 primitiveRestartEnable);

extern void fggInitPipelineData(const FggVkCore core, const char* vertexspv, const char* fragmentspv, FggVkPipelineData* pipeData);

extern void fggSetupGraphicsPipeline(const FggVkCore core, const FggVkFixedStates fStates, const VkPushConstantRange pushConstantRange, FggVkPipelineData* pipeData);


#endif