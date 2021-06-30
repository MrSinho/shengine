#include <Foggy-Engine.h>

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

	BuildShader("../Shaders/src/Triangle.vert", "../Shaders/bin/Triangle.vert.spv");
	BuildShader("../Shaders/src/Triangle.frag", "../Shaders/bin/Triangle.frag.spv");

	CreateShaderModule(data.device, "../Shaders/bin/Triangle.vert.spv");
	CreateShaderModule(data.device, "../Shaders/bin/Triangle.frag.spv");

	while (IsWindowActive(data.window.window)) {
		PollEvents();
		Draw(&data);
	}

	Cleanup(&data);

	return 0;
}