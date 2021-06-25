#include <Foggy-Engine.h>

int main() {

	VulkanHandler vulkanHandler{};
	vulkanHandler.window.width = 720;
	vulkanHandler.window.height = 480;
	vulkanHandler.window.title = "Foggy-Engine Editor";

	InitVulkan(&vulkanHandler);

	while (IsWindowActive(vulkanHandler.window)) {
		glfwPollEvents();
	}

	return 0;
}