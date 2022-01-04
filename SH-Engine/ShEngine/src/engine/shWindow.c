#include "engine/shWindow.h"
#include "engine/shEngine.h"

#include <stdio.h>
#include <stdlib.h>

#include <shVkCheck.h>

void shWindowSetup(const char* title, const uint32_t width, const uint32_t height, ShWindow* p_window) {

	ShWindow window = {
		NULL,
		width,
		height,
		title
	};
	*p_window = window;

	if (!glfwInit()) {
		puts("Error intializing gflfw!");
		exit(-1);
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	p_window->window = glfwCreateWindow(p_window->width, p_window->height, p_window->title, NULL, NULL);
	p_window->pp_instance_extensions = glfwGetRequiredInstanceExtensions(&p_window->instance_extension_count);
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

void shCreateWindowSurface(ShEngine* p_engine) {
	p_engine->core.surface.width = p_engine->window.width;
	p_engine->core.surface.height = p_engine->window.height;
	shCheckVkResult(
		glfwCreateWindowSurface(p_engine->core.instance, p_engine->window.window, NULL, &p_engine->core.surface.surface),
		"error creating window surface"
	);
}
