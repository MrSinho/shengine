#include "fggVkCore.h"
#include "fggVkPipelineData.h"
#include "fggProjection.h"
#include "fggUtilities.h"

#include <stdio.h>
#include <stdlib.h>

#include <fggVkMemoryInfo.h>

void fggAllocateUniformBufferData(const FggVkCore core, const uint32_t bufferSize, FggVkPipelineData* pPipeData) {
	pPipeData->uniformBufferSize = bufferSize;
	fggCreateBuffer(core.device, pPipeData->uniformBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, &pPipeData->uniformBuffer);
	fggAllocateMemory(core.device, core.physicalDevice, pPipeData->uniformBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &pPipeData->uniformBufferMemory);
}

void fggDescriptorSetLayout(const FggVkCore core, const uint32_t binding, const VkShaderStageFlags shaderStageFlags, FggVkPipelineData* pPipeData) {

	VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {
		binding,							//binding;
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,	//descriptorType;
		1,									//descriptorCount;
		shaderStageFlags,					//stageFlags;
		NULL								//pImmutableSamplers;
	};

	VkDescriptorSetLayoutCreateInfo descriptorSetLayout = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,	//sType;
		NULL,													//pNext;
		0,														//flags;
		1,														//bindingCount;
		&descriptorSetLayoutBinding								//pBindings;
	};

	fggCheckVkResult(
		vkCreateDescriptorSetLayout(core.device, &descriptorSetLayout, NULL, &pPipeData->descriptorSetLayout),
		"error creating descriptor set layout"
	);
}

void fggCreateDescriptorPool(const FggVkCore core, FggVkPipelineData* pPipeData) {
	VkDescriptorPoolSize descriptorPoolSize = {
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,	//type;
		core.swapchainImageCount			//descriptorCount;
	};

	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,	//sType;
		NULL,											//pNext;
		0,												//flags;
		core.swapchainImageCount,						//maxSets;
		1,												//poolSizeCount;
		&descriptorPoolSize								//pPoolSizes;
	};

	fggCheckVkResult(
		vkCreateDescriptorPool(core.device, &descriptorPoolCreateInfo, NULL, &pPipeData->descriptorPool),
		"error creating descriptor pool"
	);
}

void fggAllocateDescriptorSets(const FggVkCore core, FggVkPipelineData* pPipeData) {
	
	VkDescriptorSetAllocateInfo allocateInfo = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,	//sType;
		NULL,											//pNext;
		pPipeData->descriptorPool,						//descriptorPool;
		1,												//descriptorSetCount;
		&pPipeData->descriptorSetLayout					//pSetLayouts;
	};

	fggCheckVkResult(
		vkAllocateDescriptorSets(core.device, &allocateInfo, &pPipeData->descriptorSet),
		"error allocating descriptor set"
	);

	VkDescriptorBufferInfo descriptorBufferInfo = {
		pPipeData->uniformBuffer,		//buffer;
		0,								//offset;
		pPipeData->uniformBufferSize,	//range;
	};
	pPipeData->descriptorBufferInfo = descriptorBufferInfo;

	VkWriteDescriptorSet writeDescriptorSet = {		
		VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,					//sType;
		NULL,													//pNext;
		pPipeData->descriptorSet,								//dstSet;
		0,														//dstBinding;
		0,														//dstArrayElement;
		1,														//descriptorCount;
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,						//descriptorType;
		NULL,													//pImageInfo;
		&pPipeData->descriptorBufferInfo,						//pBufferInfo;
		NULL													//pTexelBufferView;
	};
	pPipeData->writeDescriptorSet = writeDescriptorSet;
}

void fggSetPushConstants(const VkShaderStageFlags shaderStageFlags, const uint32_t offset, const uint32_t size, void** ppData, FggVkPipelineData* pPipeData) {
	pPipeData->pushConstantRange.offset		= offset;
	pPipeData->pushConstantRange.size		= size;
	pPipeData->pushConstantRange.stageFlags = shaderStageFlags;
	pPipeData->ppPushConstantData = ppData;
}

void fggSetupShaders(const FggVkCore core, const char* vertexspv, const char* fragmentspv, FggVkPipelineData* pipeData) {

	pipeData->shaderModuleCount = 2;
	pipeData->shaderStageCount = 2;

	pipeData->pShaderModules = calloc(pipeData->shaderModuleCount, sizeof(VkShaderModule));
	if (pipeData->pShaderModules != NULL) {
		fggCreateShaderModule(core.device, vertexspv, &pipeData->pShaderModules[0]);
		fggCreateShaderModule(core.device, fragmentspv, &pipeData->pShaderModules[1]);
	
		pipeData->pShaderStages = calloc(pipeData->shaderModuleCount, sizeof(VkPipelineShaderStageCreateInfo));
		if (pipeData->pShaderStages != NULL) {
			fggCreateShaderStage(core.device, pipeData->pShaderModules[0], vertexspv, VK_SHADER_STAGE_VERTEX_BIT, &pipeData->pShaderStages[0]);
			fggCreateShaderStage(core.device, pipeData->pShaderModules[1], fragmentspv, VK_SHADER_STAGE_FRAGMENT_BIT, &pipeData->pShaderStages[1]);
		}
	}

}

void fggCreateShaderModule(const VkDevice device, const char* input, VkShaderModule* shaderModule) {
	
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

void fggCreateShaderStage(const VkDevice device, const VkShaderModule shModule, const char *shaderPath, const VkShaderStageFlagBits stageFlag, VkPipelineShaderStageCreateInfo* pShInfo) {
	
	VkPipelineShaderStageCreateInfo shaderStageCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,	//sType;
		NULL,													//pNext;
		0,														//flags;
		stageFlag,												//stage;
		shModule,												//module;
		"main",													//pName;
		NULL,													//pSpecializationInfo;
	};

	*pShInfo = shaderStageCreateInfo;

}

void fggSetVertexInputState(VkVertexInputBindingDescription* vertexBindDescription, uint32_t *vertexInputAttributeDescriptionCount, VkVertexInputAttributeDescription *pVertexInputAttributeDescriptions, VkPipelineVertexInputStateCreateInfo *vi, FggFixedStateFlags flags) {
	
	*vertexInputAttributeDescriptionCount = 0;
	
	VkVertexInputBindingDescription vertexBindingDescription = {
		0,									//binding;
		0,									//stride;
		VK_VERTEX_INPUT_RATE_VERTEX 		//inputRate;
	};
	

	VkVertexInputAttributeDescription positionInputAttributeDescription = {
		0,							//location;
		0,							//binding;
		VK_FORMAT_R32G32B32_SFLOAT,	//format;
		0							//offset;
	};
	if (flags & FGG_FIXED_STATES_VERTEX_POSITIONS_BIT) {
		*vertexInputAttributeDescriptionCount += 1;
		vertexBindingDescription.stride += sizeof(float) * 3;
	}
	
	VkVertexInputAttributeDescription normalInputAttributeDescription = {
		1,								//location;
		0,								//binding;
		VK_FORMAT_R32G32B32_SFLOAT,		//format;
		sizeof(float) * 5				//offset;
	};
	if (flags & FGG_FIXED_STATES_VERTEX_NORMALS_BIT) {
		*vertexInputAttributeDescriptionCount += 1;
		vertexBindingDescription.stride += sizeof(float) * 3;
	}

	VkVertexInputAttributeDescription uvInputAttributeDescription = {
		2,							//location;
		0,							//binding;
		VK_FORMAT_R32G32_SFLOAT,	//format;
		sizeof(float)*3				//offset;
	};
	if (flags & FGG_FIXED_STATES_VERTEX_TCOORDS_BIT) {
		*vertexInputAttributeDescriptionCount += 1;
		vertexBindingDescription.stride += sizeof(float) * 2;
	}

	pVertexInputAttributeDescriptions = (VkVertexInputAttributeDescription*)malloc(*vertexInputAttributeDescriptionCount * sizeof(VkVertexInputAttributeDescription));
	if (pVertexInputAttributeDescriptions != NULL) {
		if (flags & FGG_FIXED_STATES_VERTEX_POSITIONS_BIT) {
			pVertexInputAttributeDescriptions[0] = positionInputAttributeDescription;
		}
		if (flags & FGG_FIXED_STATES_VERTEX_NORMALS_BIT) {
			pVertexInputAttributeDescriptions[1] = normalInputAttributeDescription;
		}
		if (flags & FGG_FIXED_STATES_VERTEX_TCOORDS_BIT) {
			pVertexInputAttributeDescriptions[2] = uvInputAttributeDescription;
		}
	}
	else { exit(EXIT_FAILURE); }

	*vertexBindDescription = vertexBindingDescription;
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
		VK_POLYGON_MODE_FILL,										//polygonMode;
		VK_CULL_MODE_BACK_BIT,										//cullMode
		VK_FRONT_FACE_CLOCKWISE,									//frontFace
		VK_FALSE,													//depthBiasEnable 
		0.0f,														//depthBiasConstantFactor;
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

void fggSetFixedStates(const FggVkCore core, FggFixedStateFlags flags, FggVkFixedStates* fStates) {
	
	fggSetVertexInputState(&fStates->vertexBindingDescription, &fStates->vertexInputAttributeDescriptionCount, fStates->pVertexInputAssemblyDescriptions, &fStates->vertexInputStateInfo, flags);
	
	fggCreateInputAssembly(&fStates->inputAssembly, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);
	if (flags & FGG_FIXED_STATES_PRIMITIVE_TOPOLOGY_LINE_LIST) {
		fStates->inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
	}
	if (flags & FGG_FIXED_STATES_PRIMITIVE_TOPOLOGY_POINT_LIST) {
		fStates->inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
	}
	
	fggCreateRasterizer(&fStates->rasterizer);
	if (flags & FGG_FIXED_STATES_POLYGON_MODE_WIREFRAME_BIT) {
		fStates->rasterizer.polygonMode = VK_POLYGON_MODE_LINE;
	}
	if (flags & FGG_FIXED_STATES_POLYGON_MODE_POINTS_BIT) {
		fStates->rasterizer.polygonMode = VK_POLYGON_MODE_POINT;
	}
	

	fggSetMultisampleState(&fStates->multisampleStateInfo);
	fggColorBlendSettings(&fStates->colorBlendAttachment, &fStates->colorBlendState);
	fggSetViewport(core.window, &fStates->viewport, &fStates->scissor, &fStates->viewportState);

}

void fggSetupGraphicsPipeline(const FggVkCore core, const FggVkFixedStates fStates, FggPipelineSetupFlags setupFlags, FggVkPipelineData* pPipeData) {
	
	pPipeData->setupFlags = setupFlags;

	VkPipelineLayoutCreateInfo mainPipelineLayoutCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,	//sType;
		NULL,											//pNext;
		0,												//flags;
		0,												//setLayoutCount;
		NULL,											//pSetLayouts;
		0,												//pushConstantRangeCount;
		NULL,											//pPushConstantRanges;
	};

	if (setupFlags & FGG_PIPELINE_SETUP_PUSH_CONSTANTS_BIT) {
		mainPipelineLayoutCreateInfo.pushConstantRangeCount = 1;
		mainPipelineLayoutCreateInfo.pPushConstantRanges = &pPipeData->pushConstantRange;
	}

	if (setupFlags & FGG_PIPELINE_SETUP_UNIFORM_BUFFER_BIT) {
		mainPipelineLayoutCreateInfo.setLayoutCount = 1;
		mainPipelineLayoutCreateInfo.pSetLayouts = &pPipeData->descriptorSetLayout;
	}											   

	fggCheckVkResult(
		vkCreatePipelineLayout(core.device, &mainPipelineLayoutCreateInfo, NULL, &pPipeData->mainPipelineLayout),
		"error creating main pipeline layout"
	);

	VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {
		VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,	//sType;
		NULL,												//pNext;
		0,													//flags;
		pPipeData->shaderStageCount,						//stageCount;
		pPipeData->pShaderStages,							//pStages;
		&fStates.vertexInputStateInfo,						//pVertexInputState;
		&fStates.inputAssembly,								//pInputAssemblyState;
		NULL,												//pTessellationState;
		&fStates.viewportState,								//pViewportState;
		&fStates.rasterizer,								//pRasterizationState;
		&fStates.multisampleStateInfo,						//pMultisampleState;
		NULL,												//pDepthStencilState;
		&fStates.colorBlendState,							//pColorBlendState;
		NULL,												//pDynamicState;
		pPipeData->mainPipelineLayout,						//layout;
		core.renderPass,									//renderPass;
		0,													//subpass;
		NULL,												//basePipelineHandle;
		0													//basePipelineIndex;
	};

	pPipeData->vertexStride = fStates.vertexInputStateInfo.pVertexBindingDescriptions->stride;

	fggCheckVkResult(
		vkCreateGraphicsPipelines(core.device, NULL, 1, &graphicsPipelineCreateInfo, NULL, &pPipeData->pipeline),
		"error creating graphics pipeline"
	);
}

void fggDestroyPipeline(const FggVkCore core, FggVkPipelineData* pPipeData) {
	fggClearBufferMemory(core.device, pPipeData->uniformBuffer, pPipeData->uniformBufferMemory);
	vkDestroyDescriptorPool(core.device, pPipeData->descriptorPool, NULL);
	vkDestroyDescriptorSetLayout(core.device, pPipeData->descriptorSetLayout, NULL);
	vkDestroyPipelineLayout(core.device, pPipeData->mainPipelineLayout, NULL);
	vkDestroyPipeline(core.device, pPipeData->pipeline, NULL);
	vkDestroyShaderModule(core.device, pPipeData->pShaderModules[0], NULL);
	vkDestroyShaderModule(core.device, pPipeData->pShaderModules[1], NULL);

	free(pPipeData->pShaderStages);

}