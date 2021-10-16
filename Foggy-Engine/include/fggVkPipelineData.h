#ifndef FGG_PIPELINE_DATA_H
#define FGG_PIPELINE_DATA_H

#include <vulkan/vulkan.h>

#include "fggWindow.h"
#include "fggMesh.h"
#include "fggMaterialInfo.h"

typedef struct FggVkFixedStates {
	
	VkVertexInputBindingDescription vertex_binding_description;
	uint32_t vertex_input_attribute_description_count;
	VkVertexInputAttributeDescription* p_vertex_input_assembly_descriptions;
	VkPipelineVertexInputStateCreateInfo vertex_input_state_info;
	VkPipelineInputAssemblyStateCreateInfo input_assembly;

	VkViewport viewport;
	VkRect2D scissor;
	VkPipelineViewportStateCreateInfo viewport_state;

	VkPipelineRasterizationStateCreateInfo rasterizer;

	VkPipelineColorBlendAttachmentState color_blend_attachment;
	VkPipelineColorBlendStateCreateInfo color_blend_state;

	VkPipelineMultisampleStateCreateInfo multisample_state_info;

	FggFixedStateFlags fixed_state_flags;

} FggVkFixedStates;

typedef enum FggPipelineSetupFlags {
	FGG_PIPELINE_SETUP_PUSH_CONSTANTS_BIT = 0b0001,
	FGG_PIPELINE_SETUP_UNIFORM_BUFFER_BIT = 0b0010,
} FggPipelineSetupFlags;

typedef struct FggVkPipelineData {

	uint32_t vertexStride;

	/*Shaders*/
	uint32_t shaderStageCount;
	VkPipelineShaderStageCreateInfo* pShaderStages;
	uint32_t shaderModuleCount;
	VkShaderModule* pShaderModules;

	/*Push constants*/
	VkPushConstantRange		pushConstantRange;

	/*Descriptor stuff*/
	VkDescriptorSetLayoutBinding descriptorSetLayoutBinding;
	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	VkDescriptorBufferInfo descriptorBufferInfo;
	//int	uniform_updated;
	//int	shared_uniform;
	VkWriteDescriptorSet writeDescriptorSet;
	VkDescriptorSet descriptorSet;

	/*Uniform buffer*/
	VkBuffer uniformBuffer;
	uint32_t uniformBufferSize;
	VkDeviceMemory uniformBufferMemory;

	/*Pipeline*/
	VkPipelineLayout mainPipelineLayout;
	VkPipeline pipeline;

	FggPipelineSetupFlags setupFlags;

} FggVkPipelineData;



extern void fggCreateRasterizer(VkPipelineRasterizationStateCreateInfo* rasterizer);

extern void fggSetMultisampleState(VkPipelineMultisampleStateCreateInfo* multisampleState);

extern void fggColorBlendSettings(VkPipelineColorBlendAttachmentState* colorAttachment, VkPipelineColorBlendStateCreateInfo* color_blend_state);

extern void fggSetViewport(const FggWindow window, VkViewport *vprt, VkRect2D* scssr, VkPipelineViewportStateCreateInfo* vprtState);

extern void fggSetFixedStates(const FggVkCore core, FggFixedStateFlags flags, FggVkFixedStates* p_fixed_states);


 
extern void fggCreateShaderModule(const VkDevice device, const char* input, VkShaderModule* shaderModule);

extern void fggCreateShaderStage(const VkDevice device, const VkShaderModule shModule, const char* shaderPath, const VkShaderStageFlagBits stageFlag, VkPipelineShaderStageCreateInfo* pShInfo);



extern void fggSetVertexInputState(VkVertexInputBindingDescription* vertexBindDescription, uint32_t* vertex_input_attribute_description_count, VkVertexInputAttributeDescription* pVertexInputAttributeDescriptions, VkPipelineVertexInputStateCreateInfo* vi, FggFixedStateFlags flags);

extern void fggCreateInputAssembly(VkPipelineInputAssemblyStateCreateInfo* input_assembly, VkPrimitiveTopology primitiveTopology, VkBool32 primitiveRestartEnable);



extern void fggSetPushConstants(const VkShaderStageFlags shaderStageFlags, const uint32_t offset, const uint32_t size, FggVkPipelineData* pPipeData);


extern void fggAllocateUniformBufferData(const FggVkCore core, const uint32_t bufferSize, FggVkPipelineData* pPipeData);

extern void fggDescriptorSetLayout(const FggVkCore core, const uint32_t binding, const VkShaderStageFlags shaderStageFlags, FggVkPipelineData* pPipeData);

extern void fggCreateDescriptorPool(const FggVkCore core, FggVkPipelineData* pPipeData);

extern void fggAllocateDescriptorSets(const FggVkCore core, FggVkPipelineData* pPipeData);



extern void fggSetupShaders(const FggVkCore core, const char* vertexspv, const char* fragmentspv, FggVkPipelineData* pipeData);

extern void fggSetupGraphicsPipeline(const FggVkCore core, const FggVkFixedStates fStates, FggPipelineSetupFlags setupFlags, FggVkPipelineData* pipeData);



extern void fggDestroyPipeline(const FggVkCore core, FggVkPipelineData* pPipeData);

#endif