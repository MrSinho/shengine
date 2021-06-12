#include <Foggy-Engine.h>

int main() {

	Window window{};
	window.InitGLFW(720, 480, "Foggy-Engine Editor");

	VulkanHandler vkHandler;
	vkHandler.InitVulkan();

	while (window.IsActive()) {
		glfwPollEvents();
	}

	return 0;
}