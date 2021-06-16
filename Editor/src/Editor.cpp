#include <Foggy-Engine.h>

int main() {

	VulkanHandler vulkanHandler{};
	vulkanHandler.InitVulkan(720, 480, "Foggy-Engine Editor");

	while (vulkanHandler.window.IsActive()) {
		glfwPollEvents();
	}

	return 0;
}