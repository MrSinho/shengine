#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "shengine/shWindow.h"
#include "shengine/shEngine.h"

#include <stdio.h>
#include <stdlib.h>



uint8_t shWindowSetup(
	const char* title,
	ShWindow*   p_window
) {
	shEngineError(p_window == NULL, "shWindowSetup: invalid window memory", return 0);

	ShWindow window = {
		NULL,
		{ NULL },
		{ GLFW_CURSOR_NORMAL, GLFW_HRESIZE_CURSOR, GLFW_VRESIZE_CURSOR },
		0,
		0,
		title
	};
	*p_window = window;

	shEngineError(glfwInit()            == GLFW_FALSE, "shWindowSetup: failed initializing glfw",     return 0);
	shEngineError(glfwVulkanSupported() == GLFW_FALSE, "shWindowSetup: vulkan not supported by glfw", return 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE,  GLFW_TRUE);

	GLFWmonitor* monitor    = (GLFWmonitor*)glfwGetPrimaryMonitor();
	GLFWvidmode* video_mode = (GLFWvidmode*)glfwGetVideoMode(monitor); 	

	p_window->width  = (uint32_t)((float)video_mode->width  / 1.5f);
	p_window->height = (uint32_t)((float)video_mode->height / 1.5f);

	p_window->window = glfwCreateWindow(p_window->width, p_window->height, p_window->title, NULL, NULL);
	p_window->pp_instance_extensions = glfwGetRequiredInstanceExtensions(&p_window->instance_extension_count);

#ifdef _WIN32
	glfwSetWindowSizeLimits(p_window->window, 400, 400, GLFW_DONT_CARE, GLFW_DONT_CARE);
#else
	glfwSetWindowSizeLimits(p_window->window, p_window->width, p_window->height, p_window->width, p_window->height);//X11 is so problematic
#endif//_WIN32

	p_window->default_cursors[0] = glfwCreateStandardCursor(GLFW_CURSOR_NORMAL);
	p_window->default_cursors[1] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
	p_window->default_cursors[2] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
	p_window->default_cursors[3] = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);

	return 1;
}

uint8_t shClearWindow(
	ShWindow* p_window
) {
	shEngineError(p_window == NULL, "shClearWindow: invalid window memory", return 0);

	glfwDestroyWindow(p_window->window);
	glfwTerminate();

	return 1;
}

uint8_t shWindowCreateSurface(
	ShEngine* p_engine
) {
	shEngineError(p_engine == NULL, "shWindowCreateSurface: invalid engine memory", return 0);

	p_engine->window.width  = p_engine->window.width;
	p_engine->window.height = p_engine->window.height;

	glfwCreateWindowSurface(p_engine->core.instance, p_engine->window.window, NULL, &p_engine->core.surface);

	return 1;
}

uint8_t shUpdateInput(ShWindow* p_window) {
	shEngineError(p_window == NULL, "shUpdateInput: invalid window memory", return 0);

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

	return 1;
}

uint8_t shGetWindowSize(ShWindow* p_window) {
	shEngineError(p_window == NULL, "shGetWindowSize: invalid window memory", return 0);

	int width = (int)p_window->width;
	int height = (int)p_window->height;

	glfwGetFramebufferSize(p_window->window, &width, &height);
	
	if ((uint32_t)width != p_window->width || (uint32_t)height != p_window->height) {
		
		p_window->width = (uint32_t)width;
		p_window->height = (uint32_t)height;

		p_window->surface_resize_pending = 1;
	}

	return 1;
}

uint8_t shUpdateWindow(ShEngine* p_engine) {
	shEngineError(p_engine == NULL, "shUpdateWindow: invalid engine memory", return 0);

	shPollEvents();
	shGetTime(&p_engine->time);
	shGetCursorPosition(&p_engine->window);

	shUpdateInput(&p_engine->window);

	return 1;
}



#ifdef __cplusplus
}
#endif//__cplusplus