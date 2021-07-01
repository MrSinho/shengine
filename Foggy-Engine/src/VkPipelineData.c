#include "VkPipelineData.h"
#include "Utilities.h"

#include <stdio.h>

PipelineData PipelineDataInitPrerequisitites() {

	PipelineData data = {
		0,					//shaderStageCount;
		NULL,				//shaderStages;
		0,					//vertexInputStateInfo;
		0,					//inputAssembly;
		0,					//viewportState;
		0,					//rasterizer;
		0,					//colorBlendAttachment;
		0,					//MSAAInfo;
		0,					//pipelineLayout;
		0,					//pipeline;
	};
	
	return data;
}

void CreateShaderModule(const VkDevice device, VkShaderModule *shaderModule, const char* input) {
	
	uint32_t codeSize = 0;
	const char* shaderCode = ReadCode(input, &codeSize, "rb");

	VkShaderModuleCreateInfo shaderModuleCreateInfo = {
		VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,	//sType;
		NULL,											//pNext;
		0,												//flags;
		codeSize,										//codeSize;
		(const uint32_t*)(shaderCode)					//pCode;
	};

#ifndef NDEBUG
	printf("creating shader module using binary at: %s \n", input);
#endif

	CheckVkResult(
		vkCreateShaderModule(device, &shaderModuleCreateInfo, NULL, shaderModule),
		"error creating shader module"
	);

}

void CreateShaderStage(const VkDevice device, const char *shaderPath, VkPipelineShaderStageCreateInfo *shInfo, const VkShaderStageFlagBits stageFlag) {
	
	VkShaderModule vModule = { 0 };
	CreateShaderModule(device, &vModule, shaderPath);

	VkPipelineShaderStageCreateInfo shaderStageCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,	//sType;
		NULL,													//pNext;
		0,														//flags;
		stageFlag,												//stage;
		vModule,												//module;
		"main",													//pName;
		NULL,													//pSpecializationInfo;
	};

	*shInfo = shaderStageCreateInfo;
}

void SetVertexInputState(VkPipelineVertexInputStateCreateInfo *vi) {
	VkPipelineVertexInputStateCreateInfo vertexInput = {
		VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,//sType;
		NULL,	//pNext;
		0,		//flags;
		0,		//vertexBindingDescriptionCount;
		0,		//pVertexBindingDescriptions;
		0,		//vertexAttributeDescriptionCount;
		0,		//pVertexAttributeDescriptions;
	};

	*vi = vertexInput;
}

void CreateInputAssembly(VkPipelineInputAssemblyStateCreateInfo* inputAssembly, VkPrimitiveTopology primitiveTopology, VkBool32 primitiveRestartEnable) {
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,	//sType;
		NULL,															//pNext;
		0,																//flags;
		primitiveTopology,												//topology;
		primitiveRestartEnable											//primitiveRestartEnable;
	};																	
	*inputAssembly = inputAssemblyStateCreateInfo;
}

void CreateRasterizer(VkPipelineRasterizationStateCreateInfo *rasterizer) {
	VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,	//sType;
		NULL,														//pNext;
		0,															//flags;
		VK_TRUE,													//depthClampEnable;
		VK_FALSE,													//rasterizerDiscardEnable; //false let the rasterizer draw
		VK_POLYGON_MODE_FILL,										//polygonMode;	//fill the drawn faces
		VK_CULL_MODE_NONE,											//cullMode; //don't enable backface culling
		VK_FRONT_FACE_CLOCKWISE,									//frontFace;
		VK_FALSE,													//depthBiasEnable; // 
		0.0f,														//depthBiasConstantFactor; // level to apply to every pixel
		0.0f,														//depthBiasClamp; // depth bias clamp value
		0.0f,														//depthBiasSlopeFactor; 
		1.0f														//lineWidth;
	};

	*rasterizer = rasterizationStateCreateInfo;
}

void SetMultisampleState(VkPipelineMultisampleStateCreateInfo *multisampleState) {
	VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,	//sType;
		NULL,														//pNext;
		0,															//flags;
		VK_SAMPLE_COUNT_1_BIT,										//rasterizationSamples;
		VK_FALSE,													//sampleShadingEnable;
		1.0f,														//minSampleShading;
		NULL,														//pSampleMask;
		VK_FALSE,													//alphaToCoverageEnable;
		VK_FALSE													//alphaToOneEnable;
	};

	*multisampleState = multisampleStateCreateInfo;
}

void ColorBlendSettings(VkPipelineColorBlendAttachmentState *colorBlendAttachment) {
	VkPipelineColorBlendAttachmentState colorBlendAttachmentState = {
		VK_FALSE,							//blendEnable;
		0.0f,								//srcColorBlendFactor;
		0.0f,								//dstColorBlendFactor;
		0.0f,								//colorBlendOp;
		0.0f,								//srcAlphaBlendFactor;
		0.0f,								//dstAlphaBlendFactor;
		VK_FALSE,							//alphaBlendOp;
		VK_COLOR_COMPONENT_R_BIT | 			
		VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT 			//colorWriteMask;
	};

	*colorBlendAttachment = colorBlendAttachmentState;
}

void SetViewport(const Window window, VkPipelineViewportStateCreateInfo* viewportState) {

	VkViewport viewport = {
		0.0f,					//x; 
		0.0f,					//y;
		(float)window.width,	//width;
		(float)window.height,	//height;
		0.0f, 					//minDepth;
		1.0f					//maxDepth;
	};

	VkRect2D scissor = {
		{0, 0},							//offset
		{window.width, window.height}	//extent
	};

	VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,	//sType;
		NULL,													//pNext;
		0,														//flags;
		1, 														//viewportCount;
		&viewport,												//pViewports;
		1,														//scissorCount;
		&scissor												//pScissors;
	};

	*viewportState = viewportStateCreateInfo;
}

void SetupGraphicsPipeline(const VkDevice device, PipelineData* data, const char* input) {
	
	

}