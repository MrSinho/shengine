#ifndef SH_WINDOW_H
#define SH_WINDOW_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdint.h>

#include <shengine/shInput.h>

typedef struct ShInput {
	float				cursor_pos_x;
	float				cursor_pos_y;
	float				d_cursor_pos_x;
	float				d_cursor_pos_y;
	shMouseEvents		mouse_events;
	shMouseEvents		d_mouse_events;
	shKeyEvents			key_events;
	shKeyEvents			d_key_events;
} ShInput;

typedef struct ShWindow {
	GLFWwindow*		window;
	GLFWcursor*		default_cursors[4];
	int32_t			default_cursor_icons[4];
	uint32_t		width;
	uint32_t		height;
	const char*		title;
	const char**	pp_instance_extensions;
	uint32_t		instance_extension_count;
	ShInput			input;
	uint8_t         surface_resize_pending;
} ShWindow;

typedef struct ShEngine ShEngine;

extern void shWindowSetup(const char* title, const uint32_t width, const uint32_t height, ShWindow* p_window);

extern int shIsWindowActive(GLFWwindow* window);

#define shSetCursor(glfw_window, glfw_cursor)\
	glfwSetCursor(glfw_window, glfw_cursor)

#define shPollEvents glfwPollEvents

#define shWaitEvents glfwWaitEvents

static void shClearWindow(ShWindow* p_window) {
	glfwDestroyWindow(p_window->window);
	glfwTerminate();
}

#define shIsWindowActive(_window)\
	(!glfwWindowShouldClose((_window).window))

extern void shClearWindow(ShWindow* window);

extern void	shWindowCreateSurface(ShEngine* p_engine);

extern void shUpdateInput(ShWindow* p_window);

extern void shGetWindowSize(ShWindow* p_window);

extern void shUpdateWindow(ShEngine* p_engine);


#define shSurfaceResizePending(_window)\
	((_window).surface_resize_pending)

static uint8_t shIsKeyPressed(const ShWindow window, const uint32_t key) {
	return window.input.d_key_events[key] == GLFW_PRESS;
}

static uint8_t shIsKeyDown(const ShWindow window, const uint32_t key) {
	return window.input.key_events[key] == GLFW_PRESS;
}

static uint8_t shIsKeyReleased(const ShWindow window, const uint32_t key) {
	return window.input.key_events[key] == GLFW_RELEASE;
}

static uint8_t shIsKeyRepeated(const ShWindow window, const uint32_t key) {
	return window.input.key_events[key] == GLFW_REPEAT;
}

static uint8_t shIsMouseButtonPressed(const ShWindow window, const uint32_t button) {
	return window.input.d_mouse_events[button] == GLFW_PRESS;
}

static uint8_t shIsMouseButtonDown(const ShWindow window, const uint32_t button) {
	return window.input.mouse_events[button] == GLFW_PRESS;
}

static uint8_t shIsMouseButtonReleased(const ShWindow window, const uint32_t button) {
	return window.input.mouse_events[button] == GLFW_RELEASE;
}

static uint8_t shIsMouseButtonRepeated(const ShWindow window, const uint32_t button) {
	return window.input.mouse_events[button] == GLFW_REPEAT;
}



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_WINDOW_H