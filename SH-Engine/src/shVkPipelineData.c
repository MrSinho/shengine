#include "shVkCore.h"
#include "shVkPipelineData.h"
#include "shProjection.h"
#include "shVkCheck.h"
#include "shFile.h"

#include <stdio.h>
#include <stdlib.h>

#include <shVkMemoryInfo.h>

#ifdef _MSC_VER
#pragma warning (disable: 6386)
#endif//_MSC_VER

void shAllocateUniformBufferData(const ShVkCore core, const uint32_t bufferSize, ShUniformBuffer* p_uniform) {
	p_uniform->uniform_buffer_size = bufferSize;
	shCreateBuffer(core.device, p_uniform->uniform_buffer_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, &p_uniform->uniform_buffer);
	shAllocateMemory(core.device, core.physical_device, p_uniform->uniform_buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &p_uniform->uniform_buffer_memory);
}

void shDescriptorSetLayout(const ShVkCore core, const uint32_t binding, const VkShaderStageFlags shaderStageFlags, ShUniformBuffer* p_uniform) {

	VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {
		binding,							//binding;
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,	//descriptorType;
		1,									//descriptorCount;
		shaderStageFlags,					//stageFlags;
		NULL								//pImmutableSamplers;
	};

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,	//sType;
		NULL,													//pNext;
		0,														//flags;
		1,														//bindingCount;
		&descriptorSetLayoutBinding								//pBindings;
	};
	p_uniform->descriptor_set_layout_binding = descriptorSetLayoutBinding;

	shCheckVkResult(
		vkCreateDescriptorSetLayout(core.device, &descriptorSetLayoutCreateInfo, NULL, &p_uniform->descriptor_set_layout),
		"error creating descriptor set layout"
	);
}

void shCreateDescriptorPool(const ShVkCore core, ShUniformBuffer* p_uniform) {
	VkDescriptorPoolSize descriptorPoolSize = {
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,	//type;
		core.swapchain_image_count			//descriptorCount;
	};

	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,	//sType;
		NULL,											//pNext;
		0,												//flags;
		core.swapchain_image_count,						//maxSets;
		1,												//poolSizeCount;
		&descriptorPoolSize								//pPoolSizes;
	};

	shCheckVkResult(
		vkCreateDescriptorPool(core.device, &descriptorPoolCreateInfo, NULL, &p_uniform->descriptor_pool),
		"error creating descriptor pool"
	);
}

void shAllocateDescriptorSets(const ShVkCore core, const uint32_t uniform_idx, ShVkPipelineData* p_pipe_data) {
	ShUniformBuffer* p_uniform = &p_pipe_data->p_uniform_buffers[uniform_idx];

	VkDescriptorSetAllocateInfo allocateInfo = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,				//sType;
		NULL,														//pNext;
		p_pipe_data->p_uniform_buffers[uniform_idx].descriptor_pool,//descriptorPool;
		1,															//descriptorSetCount;
		&p_uniform->descriptor_set_layout							//pSetLayouts;
	};

	shCheckVkResult(
		vkAllocateDescriptorSets(core.device, &allocateInfo, &p_pipe_data->p_descriptor_sets[uniform_idx]),
		"error allocating descriptor set"
	);

	VkDescriptorBufferInfo descriptorBufferInfo = {
		p_uniform->uniform_buffer,		//buffer;
		0,								//offset;
		p_uniform->uniform_buffer_size,	//range;
	};
	p_uniform->descriptor_buffer_info = descriptorBufferInfo;

	VkWriteDescriptorSet writeDescriptorSet = {		
		VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,					//sType;
		NULL,													//pNext;
		p_pipe_data->p_descriptor_sets[uniform_idx],			//dstSet;
		0,														//dstBinding;
		0,														//dstArrayElement;
		1,														//descriptorCount;
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,						//descriptorType;
		NULL,													//pImageInfo;
		&p_uniform->descriptor_buffer_info,						//pBufferInfo;
		NULL													//pTexelBufferView;
	};
	p_pipe_data->p_write_descriptor_sets[uniform_idx] = writeDescriptorSet;
	p_pipe_data->p_write_descriptor_sets[uniform_idx].pBufferInfo = &p_pipe_data->p_uniform_buffers[uniform_idx].descriptor_buffer_info;
}

void shSetPushConstants(const VkShaderStageFlags shaderStageFlags, const uint32_t offset, const uint32_t size, ShVkPipelineData* pPipeData) {
	pPipeData->pushConstantRange.offset		= offset;
	pPipeData->pushConstantRange.size		= size;
	pPipeData->pushConstantRange.stageFlags = shaderStageFlags;
}

void shSetupShaders(const ShVkCore core, const char* vertexspv, const char* fragmentspv, ShVkPipelineData* pipeData) {

	pipeData->shaderModuleCount = 2;
	pipeData->shaderStageCount = 2;

	pipeData->pShaderModules = calloc(pipeData->shaderModuleCount, sizeof(VkShaderModule));
	if (pipeData->pShaderModules != NULL) {
		shCreateShaderModule(core.device, vertexspv, &pipeData->pShaderModules[0]);
		shCreateShaderModule(core.device, fragmentspv, &pipeData->pShaderModules[1]);
	
		pipeData->pShaderStages = calloc(pipeData->shaderModuleCount, sizeof(VkPipelineShaderStageCreateInfo));
		if (pipeData->pShaderStages != NULL) {
			shCreateShaderStage(core.device, pipeData->pShaderModules[0], VK_SHADER_STAGE_VERTEX_BIT, &pipeData->pShaderStages[0]);
			shCreateShaderStage(core.device, pipeData->pShaderModules[1], VK_SHADER_STAGE_FRAGMENT_BIT, &pipeData->pShaderStages[1]);
		}
	}

}

void shCreateShaderModule(const VkDevice device, const char* input, VkShaderModule* shaderModule) {
	
	uint32_t codeSize = 0;
	char* shader_code = (char*)shReadBinary(input, &codeSize);

	VkShaderModuleCreateInfo shaderModuleCreateInfo = {
		VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,	//sType;
		NULL,											//pNext;
		0,												//flags;
		codeSize,										//codeSize;
		(const uint32_t*)(shader_code)					//pCode;
	};

#ifndef NDEBUG
	printf("creating shader module using binary at: %s \n", input);
#endif

	shCheckVkResult(
		vkCreateShaderModule(device, &shaderModuleCreateInfo, NULL, shaderModule),
		"error creating shader module"
	);

	free(shader_code);
}

void shCreateShaderStage(const VkDevice device, const VkShaderModule shModule, const VkShaderStageFlagBits stageFlag, VkPipelineShaderStageCreateInfo* pShInfo) {
	
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

void shSetVertexInputState(VkVertexInputBindingDescription* vertexBindDescription, uint32_t *vertex_input_attribute_description_count, VkVertexInputAttributeDescription *pVertexInputAttributeDescriptions, VkPipelineVertexInputStateCreateInfo *vi, ShFixedStateFlags flags) {
	
	*vertex_input_attribute_description_count = 0;
	
	VkVertexInputBindingDescription vertex_binding_description = {
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
	if (flags & SH_FIXED_STATES_VERTEX_POSITIONS_BIT) {
		*vertex_input_attribute_description_count += 1;
		vertex_binding_description.stride += sizeof(float) * 3;
	}
	
	VkVertexInputAttributeDescription normalInputAttributeDescription = {
		1,								//location;
		0,								//binding;
		VK_FORMAT_R32G32B32_SFLOAT,		//format;
		sizeof(float) * 5				//offset;
	};
	if (flags & SH_FIXED_STATES_VERTEX_NORMALS_BIT) {
		*vertex_input_attribute_description_count += 1;
		vertex_binding_description.stride += sizeof(float) * 3;
	}

	VkVertexInputAttributeDescription uvInputAttributeDescription = {
		2,							//location;
		0,							//binding;
		VK_FORMAT_R32G32_SFLOAT,	//format;
		sizeof(float)*3				//offset;
	};
	if (flags & SH_FIXED_STATES_VERTEX_TCOORDS_BIT) {
		*vertex_input_attribute_description_count += 1;
		vertex_binding_description.stride += sizeof(float) * 2;
	}

	pVertexInputAttributeDescriptions = (VkVertexInputAttributeDescription*)malloc(*vertex_input_attribute_description_count * sizeof(VkVertexInputAttributeDescription));
	if (pVertexInputAttributeDescriptions != NULL) {
		if (flags & SH_FIXED_STATES_VERTEX_POSITIONS_BIT) {
			pVertexInputAttributeDescriptions[0] = positionInputAttributeDescription;
		}
		if (flags & SH_FIXED_STATES_VERTEX_NORMALS_BIT) {
			pVertexInputAttributeDescriptions[1] = normalInputAttributeDescription;
		}
		if (flags & SH_FIXED_STATES_VERTEX_TCOORDS_BIT) {
			pVertexInputAttributeDescriptions[2] = uvInputAttributeDescription;
		}
	}
	else { exit(EXIT_FAILURE); }

	*vertexBindDescription = vertex_binding_description;
	VkPipelineVertexInputStateCreateInfo vertexInput = {
		VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,//sType;
		NULL,								  //pNext;
		0,									  //flags;
		1,									  //vertexBindingDescriptionCount;
		vertexBindDescription,				  //pVertexBindingDescriptions;
		*vertex_input_attribute_description_count,//vertexAttributeDescriptionCount;
		pVertexInputAttributeDescriptions,	  //pVertexAttributeDescriptions;
	};
	*vi = vertexInput;
}

void shCreateInputAssembly(VkPipelineInputAssemblyStateCreateInfo* input_assembly, VkPrimitiveTopology primitiveTopology, VkBool32 primitiveRestartEnable) {
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,	//sType;
		NULL,															//pNext;
		0,																//flags;
		primitiveTopology,												//topology;
		primitiveRestartEnable											//primitiveRestartEnable;
	};																	
	*input_assembly = inputAssemblyStateCreateInfo;
}

void shCreateRasterizer(VkPipelineRasterizationStateCreateInfo *rasterizer) {
	VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,	//sType;
		NULL,														//pNext;
		0,															//flags;
		VK_FALSE,													//depthClampEnable;
		VK_FALSE,													//rasterizerDiscardEnable;
		VK_POLYGON_MODE_FILL,										//polygonMode;
		VK_CULL_MODE_BACK_BIT,										//cullMode
		VK_FRONT_FACE_CLOCKWISE,									//frontFace
		VK_FALSE,													//depthBiasEnable 
		0.0f,														//depthBiasConstantFactor;
		0.0f,														//depthBiasClamp;
		0.0f,														//depthBiasSlopeFactor; 
		1.0f														//lineWidth;
	};

	*rasterizer = rasterizationStateCreateInfo;
}

void shSetMultisampleState(VkPipelineMultisampleStateCreateInfo *multisampleState) {
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

void shColorBlendSettings(VkPipelineColorBlendAttachmentState *color_blend_attachment, VkPipelineColorBlendStateCreateInfo* color_blend_state) {
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
	*color_blend_attachment = colorBlendAttachmentState;

	VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		NULL,
		0,
		VK_FALSE,
		VK_LOGIC_OP_COPY,
		1,
		color_blend_attachment,
		{0.0f, 0.0f, 0.0f}
	};
	*color_blend_state = colorBlendStateCreateInfo;
}

void shSetViewport(const ShWindow window, VkViewport* vprt, VkRect2D* scssr, VkPipelineViewportStateCreateInfo* vprtState) {
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

void shSetFixedStates(const ShVkCore core, ShFixedStateFlags flags, ShVkFixedStates* p_fixed_states) {
	
	shSetVertexInputState(&p_fixed_states->vertex_binding_description, &p_fixed_states->vertex_input_attribute_description_count, p_fixed_states->p_vertex_input_assembly_descriptions, &p_fixed_states->vertex_input_state_info, flags);
	
	shCreateInputAssembly(&p_fixed_states->input_assembly, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);
	if (flags & SH_FIXED_STATES_PRIMITIVE_TOPOLOGY_LINE_LIST) {
		p_fixed_states->input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
	}
	if (flags & SH_FIXED_STATES_PRIMITIVE_TOPOLOGY_POINT_LIST) {
		p_fixed_states->input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
	}

	shCreateRasterizer(&p_fixed_states->rasterizer);
	if (flags & SH_FIXED_STATES_POLYGON_MODE_WIREFRAME_BIT) {
		p_fixed_states->rasterizer.polygonMode = VK_POLYGON_MODE_LINE;
	}
	if (flags & SH_FIXED_STATES_POLYGON_MODE_POINTS_BIT) {
		p_fixed_states->rasterizer.polygonMode = VK_POLYGON_MODE_POINT;
	}
	

	shSetMultisampleState(&p_fixed_states->multisample_state_info);
	shColorBlendSettings(&p_fixed_states->color_blend_attachment, &p_fixed_states->color_blend_state);
	shSetViewport(core.window, &p_fixed_states->viewport, &p_fixed_states->scissor, &p_fixed_states->viewport_state);

}

void shSetupGraphicsPipeline(const ShVkCore core, const ShVkFixedStates fStates, ShPipelineSetupFlags setupFlags, ShVkPipelineData* pPipeData) {
	
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

	if (pPipeData->pushConstantRange.size != 0) {
		mainPipelineLayoutCreateInfo.pushConstantRangeCount = 1;
		mainPipelineLayoutCreateInfo.pPushConstantRanges = &pPipeData->pushConstantRange;
	}
	VkDescriptorSetLayout* p_descriptor_set_layouts = calloc(pPipeData->uniform_buffer_count, sizeof(VkDescriptorSetLayout));
	if (p_descriptor_set_layouts != NULL) {
		mainPipelineLayoutCreateInfo.setLayoutCount = pPipeData->uniform_buffer_count;
		for (uint32_t i = 0; i < pPipeData->uniform_buffer_count; i++) {
			p_descriptor_set_layouts[i] = pPipeData->p_uniform_buffers[i].descriptor_set_layout;
		}
		mainPipelineLayoutCreateInfo.pSetLayouts = p_descriptor_set_layouts;
	}											   

	VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,	//sType;
		NULL,														//pNext;
		0,															//flags;
		VK_TRUE,													//depthTestEnable;
		VK_TRUE,													//depthWriteEnable;
		VK_COMPARE_OP_LESS,											//depthCompareOp;
		VK_FALSE,													//depthBoundsTestEnable;
		VK_FALSE,													//stencilTestEnable;
		0, 															//front;
		0,															//back;
		0.0f,														//minDepthBounds;
		1.0f														//maxDepthBounds;
	};


	shCheckVkResult(
		vkCreatePipelineLayout(core.device, &mainPipelineLayoutCreateInfo, NULL, &pPipeData->mainPipelineLayout),
		"error creating main pipeline layout"
	);

	VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {
		VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,	//sType;
		NULL,												//pNext;
		0,													//flags;
		pPipeData->shaderStageCount,						//stageCount;
		pPipeData->pShaderStages,							//pStages;
		&fStates.vertex_input_state_info,					//pVertexInputState;
		&fStates.input_assembly,							//pInputAssemblyState;
		NULL,												//pTessellationState;
		&fStates.viewport_state,							//pViewportState;
		&fStates.rasterizer,								//pRasterizationState;
		&fStates.multisample_state_info,					//pMultisampleState;
		&depthStencilStateCreateInfo ,						//pDepthStencilState;
		&fStates.color_blend_state,							//pColorBlendState;
		NULL,												//pDynamicState;
		pPipeData->mainPipelineLayout,						//layout;
		core.render_pass,									//renderPass;
		0,													//subpass;
		0,													//basePipelineHandle;
		0													//basePipelineIndex;
	};

	pPipeData->vertexStride = fStates.vertex_input_state_info.pVertexBindingDescriptions->stride;

	shCheckVkResult(
		vkCreateGraphicsPipelines(core.device, 0, 1, &graphicsPipelineCreateInfo, NULL, &pPipeData->pipeline),
		"error creating graphics pipeline"
	);

	if (p_descriptor_set_layouts != NULL) { free(p_descriptor_set_layouts); }
}

void shDestroyPipeline(const ShVkCore core, ShVkPipelineData* pPipeData) {
	for (uint32_t i = 0; i < pPipeData->uniform_buffer_count; i++) {
		shClearBufferMemory(core.device, pPipeData->p_uniform_buffers[i].uniform_buffer, pPipeData->p_uniform_buffers[i].uniform_buffer_memory);
	}
	for (uint32_t i = 0; i < pPipeData->uniform_buffer_count; i++) {
		vkDestroyDescriptorPool(core.device, pPipeData->p_uniform_buffers[i].descriptor_pool, NULL);
		vkDestroyDescriptorSetLayout(core.device, pPipeData->p_uniform_buffers[i].descriptor_set_layout, NULL);
	}
	
	vkDestroyPipelineLayout(core.device, pPipeData->mainPipelineLayout, NULL);
	vkDestroyPipeline(core.device, pPipeData->pipeline, NULL);
	vkDestroyShaderModule(core.device, pPipeData->pShaderModules[0], NULL);
	vkDestroyShaderModule(core.device, pPipeData->pShaderModules[1], NULL);
	
	if (pPipeData->p_uniform_buffers		!= NULL) { free(pPipeData->p_uniform_buffers);		 }
	if (pPipeData->p_descriptor_sets		!= NULL) { free(pPipeData->p_descriptor_sets);		 }
	if (pPipeData->p_write_descriptor_sets	!= NULL) { free(pPipeData->p_write_descriptor_sets); }
	if (pPipeData->pShaderStages			!= NULL) { free(pPipeData->pShaderStages);			 }
	if (pPipeData->pShaderModules			!= NULL) { free(pPipeData->pShaderModules);			 }
}																