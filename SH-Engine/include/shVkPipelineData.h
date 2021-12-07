#ifndef SH_PIPELINE_DATA_H
#define SH_PIPELINE_DATA_H

#include <vulkan/vulkan.h>

#include "shWindow.h"
#include "shMesh.h"
#include "shMaterialInfo.h"

typedef struct ShVkFixedStates {
	
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

	ShFixedStateFlags fixed_state_flags;

} ShVkFixedStates;

typedef enum ShPipelineSetupFlags {
	SH_PIPELINE_SETUP_PUSH_CONSTANTS_BIT = 0b0001,
	SH_PIPELINE_SETUP_UNIFORM_BUFFER_BIT = 0b0010,
} ShPipelineSetupFlags;

typedef struct ShVkPipelineData {

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

	ShPipelineSetupFlags setupFlags;

} ShVkPipelineData;



extern void shCreateRasterizer(VkPipelineRasterizationStateCreateInfo* rasterizer);

extern void shSetMultisampleState(VkPipelineMultisampleStateCreateInfo* multisampleState);

extern void shColorBlendSettings(VkPipelineColorBlendAttachmentState* colorAttachment, VkPipelineColorBlendStateCreateInfo* color_blend_state);

extern void shSetViewport(const ShWindow window, VkViewport *vprt, VkRect2D* scssr, VkPipelineViewportStateCreateInfo* vprtState);

extern void shSetFixedStates(const ShVkCore core, ShFixedStateFlags flags, ShVkFixedStates* p_fixed_states);


 
extern void shCreateShaderModule(const VkDevice device, const char* input, VkShaderModule* shaderModule);

extern void shCreateShaderStage(const VkDevice device, const VkShaderModule shModule, const VkShaderStageFlagBits stageFlag, VkPipelineShaderStageCreateInfo* pShInfo);



extern void shSetVertexInputState(VkVertexInputBindingDescription* vertexBindDescription, uint32_t* vertex_input_attribute_description_count, VkVertexInputAttributeDescription* pVertexInputAttributeDescriptions, VkPipelineVertexInputStateCreateInfo* vi, ShFixedStateFlags flags);

extern void shCreateInputAssembly(VkPipelineInputAssemblyStateCreateInfo* input_assembly, VkPrimitiveTopology primitiveTopology, VkBool32 primitiveRestartEnable);



extern void shSetPushConstants(const VkShaderStageFlags shaderStageFlags, const uint32_t offset, const uint32_t size, ShVkPipelineData* pPipeData);


extern void shAllocateUniformBufferData(const ShVkCore core, const uint32_t bufferSize, ShVkPipelineData* pPipeData);

extern void shDescriptorSetLayout(const ShVkCore core, const uint32_t binding, const VkShaderStageFlags shaderStageFlags, ShVkPipelineData* pPipeData);

extern void shCreateDescriptorPool(const ShVkCore core, ShVkPipelineData* pPipeData);

extern void shAllocateDescriptorSets(const ShVkCore core, ShVkPipelineData* pPipeData);



extern void shSetupShaders(const ShVkCore core, const char* vertexspv, const char* fragmentspv, ShVkPipelineData* pipeData);

extern void shSetupGraphicsPipeline(const ShVkCore core, const ShVkFixedStates fStates, ShPipelineSetupFlags setupFlags, ShVkPipelineData* pipeData);



extern void shDestroyPipeline(const ShVkCore core, ShVkPipelineData* pPipeData);

#endif