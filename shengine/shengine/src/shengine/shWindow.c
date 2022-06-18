#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "shengine/shWindow.h"
#include "shengine/shEngine.h"

#include <stdio.h>
#include <stdlib.h>

#include <shvulkan/shVkCheck.h>

#include <shengine/shInput.h>

#include <assert.h>

void shWindowSetup(const char* title, const uint32_t width, const uint32_t height, ShWindow* p_window) {
	assert(p_window != NULL);

	ShWindow window = {
		NULL,
		width,
		height,
		title
	};
	*p_window = window;

	shVkError(!glfwInit(), "error initializing glfw");
	shVkError(glfwVulkanSupported() == GLFW_FALSE, "vulkan not supported by glfw");
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	p_window->window = glfwCreateWindow(p_window->width, p_window->height, p_window->title, NULL, NULL);
	p_window->pp_instance_extensions = glfwGetRequiredInstanceExtensions(&p_window->instance_extension_count);
}

void shWindowCreateSurface(ShEngine* p_engine) {
	assert(p_engine != NULL);

	p_engine->core.surface.width = p_engine->window.width;
	p_engine->core.surface.height = p_engine->window.height;
	glfwCreateWindowSurface(p_engine->core.instance, p_engine->window.window, NULL, &p_engine->core.surface.surface);
}

void shUpdateInput(ShWindow* p_window) {
	assert(p_window != NULL);
	for (uint32_t i = 0; i < (uint32_t)(SH_KEY_LAST + 1); i++) {
		p_window->input.key_actions[i] = glfwGetKey(p_window->window, i);
	}
}

void shUpdateWindow(ShEngine* p_engine) {
	assert(p_engine != NULL);

	shPollEvents();
	shGetTime(&p_engine->time);
	shGetCursorPosition(&p_engine->window);

	shUpdateInput(&p_engine->window);
}

#ifdef __cplusplus
}
#endif//__cplusplus