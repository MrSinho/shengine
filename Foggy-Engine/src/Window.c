#include "Window.h"

#include <stdio.h>

void InitGLFW(Window* window) {

	if (!glfwInit()) {
		puts("Error intializing gflfw!");
		exit(-1);
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window->window = glfwCreateWindow(window->width, window->height, window->title, NULL, NULL);
}

int IsWindowActive(const GLFWwindow *window) {
	return !glfwWindowShouldClose(window);
}

void PollEvents() {
	glfwPollEvents();
}

void ClearWindow(const GLFWwindow* window) {
	glfwDestroyWindow(window);
	glfwTerminate();
}

