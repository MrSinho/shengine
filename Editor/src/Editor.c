#include <Foggy-Engine.h>

#include <stdlib.h>

int main() {

	VkData data = VKDataInitPrerequisites(720, 480, "Foggy-Engine Editor");

	InitVulkan(&data);
	
	CreateSwapchain(&data);
	GetSwapchainImages(&data);
	CreateSwapchainImageViews(&data);

	InitCommands(&data);

	CreateRenderPass(&data);
	SetFramebuffers(&data);
	SetSyncObjects(&data);

	PipelineData pipeData = PipelineDataInitPrerequisitites();

	BuildShader("../Shaders/src/Triangle.vert", "../Shaders/bin/Triangle.vert.spv");
	BuildShader("../Shaders/src/Triangle.frag", "../Shaders/bin/Triangle.frag.spv");
	
	pipeData.shaderStageCount = 2;
	pipeData.pShaderStages = (VkPipelineShaderStageCreateInfo*)calloc(pipeData.shaderStageCount, sizeof(VkPipelineShaderStageCreateInfo));
	if (pipeData.pShaderStages != NULL) {
		CreateShaderStage(data.device, "../Shaders/bin/Triangle.vert.spv", &pipeData.pShaderStages[0], VK_SHADER_STAGE_VERTEX_BIT);
		CreateShaderStage(data.device, "../Shaders/bin/Triangle.frag.spv", &pipeData.pShaderStages[1], VK_SHADER_STAGE_FRAGMENT_BIT);
	}

	CreateInputAssembly(&pipeData.inputAssembly, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);

	CreateRasterizer(&pipeData.rasterizer);

	SetMultisampleState(&pipeData.multisampleStateInfo);

	ColorBlendSettings(&pipeData.colorBlendAttachment);

	SetViewport(data.window, &pipeData.viewportState);

	SetupGraphicsPipeline(data.device, &pipeData);

	while (IsWindowActive(data.window.window)) {
		PollEvents();
		Draw(&data);
	}

	Cleanup(&data);

	return 0;
}