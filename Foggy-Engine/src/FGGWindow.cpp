#include "FGGWindow.h"
#include <stdexcept>

void InitGLFW(FGGWindow *window) {

	if (!glfwInit()) {
		throw std::runtime_error("Error intializing gflfw!");
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window->window = glfwCreateWindow(window->width, window->height, window->title, NULL, NULL);
}

bool IsWindowActive(const FGGWindow& window) {
	return !glfwWindowShouldClose(window.window);
}

void ClearWindow(FGGWindow *window) {
	glfwDestroyWindow(window->window);
	glfwTerminate();
}

