#include "shWindow.h"

#include <stdio.h>
#include <stdlib.h>

void shInitGLFW(ShWindow* window) {

	if (!glfwInit()) {
		puts("Error intializing gflfw!");
		exit(-1);
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window->window = glfwCreateWindow(window->width, window->height, window->title, NULL, NULL);
}

int shIsWindowActive(GLFWwindow *window) {
	return !glfwWindowShouldClose(window);
}

void shPollEvents() {
	glfwPollEvents();
}

void shClearWindow(GLFWwindow* window) {
	glfwDestroyWindow(window);
	glfwTerminate();
}

