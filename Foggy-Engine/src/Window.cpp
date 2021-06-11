#include "Window.h"
#include <stdexcept>

Window::Window(uint32_t _width, uint32_t _height, const char *_title) {
	width = _width; height = _height; title = _title;
	InitGLFW();
}

void Window::InitGLFW() {
	
	if (!glfwInit()) {
		throw std::runtime_error("Error intializing gflfw!");
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(width, height, title, NULL, NULL);
}

bool Window::IsActive() {
	return !glfwWindowShouldClose(window);
}

Window::~Window() {
	glfwDestroyWindow(window);
	glfwTerminate();
}