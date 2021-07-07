#include "fggWindow.h"

#include <stdio.h>
#include <stdlib.h>

void fggInitGLFW(FggWindow* window) {

	if (!glfwInit()) {
		puts("Error intializing gflfw!");
		exit(-1);
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window->window = glfwCreateWindow(window->width, window->height, window->title, NULL, NULL);
}

int fggIsWindowActive(GLFWwindow *window) {
	return !glfwWindowShouldClose(window);
}

void fggPollEvents() {
	glfwPollEvents();
}

void fggClearWindow(GLFWwindow* window) {
	glfwDestroyWindow(window);
	glfwTerminate();
}

