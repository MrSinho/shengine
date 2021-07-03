#include <Foggy-Engine.h>

#include <stdlib.h>

int main() {

	VkData data = VKDataInitPrerequisites(600, 600, "Foggy-Engine Editor");

	InitVulkan(&data);

	CreateSwapchain(&data);
	GetSwapchainImages(&data);
	CreateSwapchainImageViews(&data);

	float vertices[48] = {
		//positions					//UV		//normals
		1.0f,  1.0f,  0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	   -1.0f,  1.0f,  0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	   -1.0f, -1.0f,  0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f,  1.0f,  0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	   -1.0f,  1.0f,  0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	    1.0f, -1.0f,  0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f
	};

	Mesh triangle = {
		48,				//vertexCount;
		&vertices[0],	//pVertices;
		0,				//indexCount;
		NULL			//pIndices;
	};
	
	InitCommands(&data);

	CreateRenderPass(&data);
	SetFramebuffers(&data);
	SetSyncObjects(&data);

	PipelineData pipeData = PipelineDataInitPrerequisitites();

	//BuildShader("../Shaders/src/Charges.vert", "../Shaders/bin/Charges.vert.spv");
	//BuildShader("../Shaders/src/Charges.frag", "../Shaders/bin/Charges.frag.spv");

	BuildShader("../Shaders/src/Triangle.vert", "../Shaders/bin/Triangle.vert.spv");
	BuildShader("../Shaders/src/Triangle.frag", "../Shaders/bin/Triangle.frag.spv");


	pipeData.shaderStageCount = 2;
	pipeData.pShaderStages = (VkPipelineShaderStageCreateInfo*)calloc(pipeData.shaderStageCount, sizeof(VkPipelineShaderStageCreateInfo));
	if (pipeData.pShaderStages != NULL) {
		CreateShaderStage(data.device, "../Shaders/bin/Charges.vert.spv", &pipeData.pShaderStages[0], VK_SHADER_STAGE_VERTEX_BIT);
		CreateShaderStage(data.device, "../Shaders/bin/Charges.frag.spv", &pipeData.pShaderStages[1], VK_SHADER_STAGE_FRAGMENT_BIT);
		//CreateShaderStage(data.device, "../Shaders/bin/Triangle.vert.spv", &pipeData.pShaderStages[0], VK_SHADER_STAGE_VERTEX_BIT);
		//CreateShaderStage(data.device, "../Shaders/bin/Triangle.frag.spv", &pipeData.pShaderStages[1], VK_SHADER_STAGE_FRAGMENT_BIT);
	}

	VkBuffer vertexBuffer;
	LoadMesh(data, triangle, &vertexBuffer);

	//PIPELINE INITIALIZATION
	SetVertexInputState(&pipeData.vertexBindingDescription, &pipeData.vertexInputAttributeDescriptionCount, pipeData.pVertexInputAssemblyDescriptions, &pipeData.vertexInputStateInfo);
	CreateInputAssembly(&pipeData.inputAssembly, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);
	CreateRasterizer(&pipeData.rasterizer);
	SetMultisampleState(&pipeData.multisampleStateInfo);
	ColorBlendSettings(&pipeData.colorBlendAttachment, &pipeData.colorBlendState);
	SetViewport(data.window, &pipeData);
	SetupGraphicsPipeline(data, &pipeData);

	while (IsWindowActive(data.window.window)) {
		PollEvents();
		Draw(&data, &pipeData, triangle, vertexBuffer);
	}

	Cleanup(&data);

	return 0;
}