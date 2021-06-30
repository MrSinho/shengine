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

	while (IsWindowActive(data.window.window)) {
		PollEvents();
		Draw(&data);
	}

	return 0;
}