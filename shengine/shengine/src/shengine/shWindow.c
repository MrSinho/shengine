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
	shEngineError(p_window == NULL, "invalid window memory");

	ShWindow window = {
		NULL,
		{ NULL },
		{ GLFW_CURSOR_NORMAL, GLFW_HRESIZE_CURSOR, GLFW_VRESIZE_CURSOR },
		width,
		height,
		title
	};
	*p_window = window;

	shVkError(!glfwInit(), "error initializing glfw", return);
	shVkError(glfwVulkanSupported() == GLFW_FALSE, "vulkan not supported by glfw", return);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	p_window->window = glfwCreateWindow(p_window->width, p_window->height, p_window->title, NULL, NULL);
	p_window->pp_instance_extensions = glfwGetRequiredInstanceExtensions(&p_window->instance_extension_count);

	p_window->default_cursors[0] = glfwCreateStandardCursor(GLFW_CURSOR_NORMAL);
	p_window->default_cursors[1] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
	p_window->default_cursors[2] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
	p_window->default_cursors[3] = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
}

void shWindowCreateSurface(ShEngine* p_engine) {
	shEngineError(p_engine == NULL, "invalid engine memory");

	p_engine->core.surface.width = p_engine->window.width;
	p_engine->core.surface.height = p_engine->window.height;
	glfwCreateWindowSurface(p_engine->core.instance, p_engine->window.window, NULL, &p_engine->core.surface.surface);
}

void shUpdateInput(ShWindow* p_window) {
	shEngineError(p_window == NULL, "invalid window memory");

	for (uint32_t i = 0; i < (uint32_t)(SH_KEY_LAST + 1); i++) {
		uint32_t key = glfwGetKey(p_window->window, i);
		p_window->input.d_key_events[i] = p_window->input.key_events[i] != key ? key : 0;
		p_window->input.key_events[i] = key;
	}
	for (uint32_t i = 0; i < (uint32_t)(SH_MOUSE_BUTTON_8 + 1); i++) {
		uint32_t click = glfwGetMouseButton(p_window->window, i);
		p_window->input.d_mouse_events[i] = p_window->input.mouse_events[i] != click ? click : 0;
		p_window->input.mouse_events[i] = click;
	}
}

void shGetWindowSize(ShWindow* p_window) {
	shEngineError(p_window == NULL, "invalid window memory");
	glfwGetWindowSize(p_window->window, (int*)&p_window->width, (int*)&p_window->height);
}

void shUpdateWindow(ShEngine* p_engine) {
	shEngineError(p_engine == NULL, "invalid engine memory");

	shPollEvents();
	shGetTime(&p_engine->time);
	shGetCursorPosition(&p_engine->window);

	shUpdateInput(&p_engine->window);
}

#ifdef __cplusplus
}
#endif//__cplusplus