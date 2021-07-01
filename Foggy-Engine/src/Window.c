#include "Window.h"

#include <stdio.h>
#include <stdlib.h>

void InitGLFW(Window* window) {

	if (!glfwInit()) {
		puts("Error intializing gflfw!");
		exit(-1);
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window->window = glfwCreateWindow(window->width, window->height, window->title, NULL, NULL);
}

int IsWindowActive(GLFWwindow *window) {
	return !glfwWindowShouldClose(window);
}

void PollEvents() {
	glfwPollEvents();
}

void ClearWindow(GLFWwindow* window) {
	glfwDestroyWindow(window);
	glfwTerminate();
}

