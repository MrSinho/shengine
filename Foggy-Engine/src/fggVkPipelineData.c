#include "fggVkCore.h"
#include "fggVkPipelineData.h"
#include "fggProjection.h"
#include "fggUtilities.h"

#include <stdio.h>
#include <stdlib.h>

FggVkPipelineData fggVkPipelineDataInitPrerequisitites() {

	FggVkPipelineData data = {0};
	
	return data;
}

void fggCreateShaderModule(const VkDevice device, VkShaderModule *shaderModule, const char* input) {
	
	uint32_t codeSize = 0;
	const char* shaderCode = fggReadCode(input, &codeSize, "rb");

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

	fggCheckVkResult(
		vkCreateShaderModule(device, &shaderModuleCreateInfo, NULL, shaderModule),
		"error creating shader module"
	);

}

void fggCreateShaderStage(const VkDevice device, const char *shaderPath, VkPipelineShaderStageCreateInfo *shInfo, const VkShaderStageFlagBits stageFlag) {
	
	VkShaderModule vModule = { 0 };
	fggCreateShaderModule(device, &vModule, shaderPath);

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

void fggSetVertexInputState(VkVertexInputBindingDescription* vertexBindDescription, uint32_t *vertexInputAttributeDescriptionCount, VkVertexInputAttributeDescription *pVertexInputAttributeDescriptions, VkPipelineVertexInputStateCreateInfo *vi) {
	
	pVertexInputAttributeDescriptions = (VkVertexInputAttributeDescription*)malloc(3 * sizeof(VkVertexInputAttributeDescription));

	VkVertexInputBindingDescription vertexBindingDescription = {
		0,									//binding;
		sizeof(float) * 8,					//stride;
		VK_VERTEX_INPUT_RATE_VERTEX 		//inputRate;
	};
	*vertexBindDescription = vertexBindingDescription;

	VkVertexInputAttributeDescription positionInputAttributeDescription = {
		0,							//location;
		0,							//binding;
		VK_FORMAT_R32G32B32_SFLOAT,	//format;
		0							//offset;
	};

	VkVertexInputAttributeDescription uvInputAttributeDescription = {
		1,							//location;
		0,							//binding;
		VK_FORMAT_R32G32_SFLOAT,	//format;
		sizeof(float)*3				//offset;
	};

	VkVertexInputAttributeDescription normalInputAttributeDescription = {
		2,								//location;
		0,								//binding;
		VK_FORMAT_R32G32B32_SFLOAT,		//format;
		sizeof(float) * 5				//offset;
	};

	*vertexInputAttributeDescriptionCount = 3;
	if (pVertexInputAttributeDescriptions != NULL) {
		pVertexInputAttributeDescriptions[0] = positionInputAttributeDescription;
		pVertexInputAttributeDescriptions[1] = uvInputAttributeDescription;
		pVertexInputAttributeDescriptions[2] = normalInputAttributeDescription;
	}
	else { exit(-1); }

	VkPipelineVertexInputStateCreateInfo vertexInput = {
		VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,//sType;
		NULL,								  //pNext;
		0,									  //flags;
		1,									  //vertexBindingDescriptionCount;
		vertexBindDescription,				  //pVertexBindingDescriptions;
		*vertexInputAttributeDescriptionCount,//vertexAttributeDescriptionCount;
		pVertexInputAttributeDescriptions,	  //pVertexAttributeDescriptions;
	};
	*vi = vertexInput;
}

void fggCreateInputAssembly(VkPipelineInputAssemblyStateCreateInfo* inputAssembly, VkPrimitiveTopology primitiveTopology, VkBool32 primitiveRestartEnable) {
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,	//sType;
		NULL,															//pNext;
		0,																//flags;
		primitiveTopology,												//topology;
		primitiveRestartEnable											//primitiveRestartEnable;
	};																	
	*inputAssembly = inputAssemblyStateCreateInfo;
}

void fggCreateRasterizer(VkPipelineRasterizationStateCreateInfo *rasterizer) {
	VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,	//sType;
		NULL,														//pNext;
		0,															//flags;
		VK_FALSE,													//depthClampEnable;
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

void fggSetMultisampleState(VkPipelineMultisampleStateCreateInfo *multisampleState) {
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

void fggColorBlendSettings(VkPipelineColorBlendAttachmentState *colorBlendAttachment, VkPipelineColorBlendStateCreateInfo* colorBlendState) {
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

	VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		NULL,
		0,
		VK_FALSE,
		VK_LOGIC_OP_COPY,
		1,
		colorBlendAttachment,
		{0.0f, 0.0f, 0.0f}
	};
	*colorBlendState = colorBlendStateCreateInfo;
}

void fggSetViewport(const FggWindow window, VkViewport* vprt, VkRect2D* scssr, VkPipelineViewportStateCreateInfo* vprtState) {
	VkViewport viewport = {
		0.0f,					//x; 
		0.0f,					//y;
		(float)window.width,	//width;
		(float)window.height,	//height;
		0.0f, 					//minDepth;
		1.0f					//maxDepth;
	};
	*vprt = viewport;

	VkRect2D scissor = {
		{0, 0},							//offset
		{window.width, window.height}	//extent
	};
	*scssr = scissor;

	VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,	//sType;
		NULL,													//pNext;
		0,														//flags;
		1, 														//viewportCount;
		vprt,													//pViewports;
		1,														//scissorCount;
		scssr													//pScissors;
	};
	*vprtState = viewportStateCreateInfo;
}

FggVkFixedStates fggFixedStatesInitPrerequisites() {
	FggVkFixedStates fStates = {
		0
	};
	return fStates;
}

void fggSetFixedStates(const FggVkCore data, FggVkFixedStates* fStates) {
	
	fggSetVertexInputState(&fStates->vertexBindingDescription, &fStates->vertexInputAttributeDescriptionCount, fStates->pVertexInputAssemblyDescriptions, &fStates->vertexInputStateInfo);
	fggCreateInputAssembly(&fStates->inputAssembly, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);
	fggCreateRasterizer(&fStates->rasterizer);
	fggSetMultisampleState(&fStates->multisampleStateInfo);
	fggColorBlendSettings(&fStates->colorBlendAttachment, &fStates->colorBlendState);
	fggSetViewport(data.window, &fStates->viewport, &fStates->scissor, &fStates->viewportState);

}

void fggSetupGraphicsPipeline(const FggVkCore data, const FggVkFixedStates fStates, FggVkPipelineData* pipeData) {
	
	VkPushConstantRange pushConstantRange = {
		VK_SHADER_STAGE_VERTEX_BIT,		//stageFlags;
		0,								//offset;
		sizeof(FggProjection)			//size;
	};

	VkPipelineLayoutCreateInfo mainPipelineLayoutCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,	//sType;
		NULL,											//pNext;
		0,												//flags;
		0,												//setLayoutCount;
		NULL,											//pSetLayouts;
		1,												//pushConstantRangeCount;
		&pushConstantRange								//pPushConstantRanges;
	};

	fggCheckVkResult(
		vkCreatePipelineLayout(data.device, &mainPipelineLayoutCreateInfo, NULL, &pipeData->mainPipelineLayout),
		"error creating main pipeline layout"
	);

	VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {
		VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,	//sType;
		NULL,												//pNext;
		0,													//flags;
		pipeData->shaderStageCount,							//stageCount;
		pipeData->pShaderStages,							//pStages;
		&fStates.vertexInputStateInfo,						//pVertexInputState;
		&fStates.inputAssembly,								//pInputAssemblyState;
		NULL,												//pTessellationState;
		&fStates.viewportState,								//pViewportState;
		&fStates.rasterizer,									//pRasterizationState;
		&fStates.multisampleStateInfo,						//pMultisampleState;
		NULL,												//pDepthStencilState;
		&fStates.colorBlendState,								//pColorBlendState;
		NULL,												//pDynamicState;
		pipeData->mainPipelineLayout,						//layout;
		data.renderPass,									//renderPass;
		0,													//subpass;
		NULL,												//basePipelineHandle;
		0													//basePipelineIndex;
	};

	fggCheckVkResult(
		vkCreateGraphicsPipelines(data.device, NULL, 1, &graphicsPipelineCreateInfo, NULL, &pipeData->pipeline),
		"error creating graphics pipeline"
	);
}