#include "Window.h"
#include <stdexcept>

void InitGLFW(Window *window) {

	if (!glfwInit()) {
		throw std::runtime_error("Error intializing gflfw!");
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window->window = glfwCreateWindow(window->width, window->height, window->title, NULL, NULL);
}

bool IsWindowActive(const Window& window) {
	return !glfwWindowShouldClose(window.window);
}

void ClearWindow(Window *window) {
	glfwDestroyWindow(window->window);
	glfwTerminate();
}

