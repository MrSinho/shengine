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
	double				cursor_pos_x;
	double				cursor_pos_y;
	double				d_cursor_pos_x;
	double				d_cursor_pos_y;
	shMouseEvents		mouse_events;
	shKeyEvents			key_events;
} ShInput;

typedef struct ShWindow {
	GLFWwindow*		window;
	uint32_t		width;
	uint32_t		height;
	const char*		title;
	const char**	pp_instance_extensions;
	uint32_t		instance_extension_count;
	ShInput			input;
} ShWindow;

typedef struct ShEngine ShEngine;

extern void shWindowSetup(const char* title, const uint32_t width, const uint32_t height, ShWindow* p_window);

extern int shIsWindowActive(GLFWwindow* window);

#define shPollEvents glfwPollEvents

#define shWaitEvents glfwWaitEvents

static void shClearWindow(GLFWwindow* window) {
	glfwDestroyWindow(window);
	glfwTerminate();
}

#define shIsWindowActive !glfwWindowShouldClose

extern void shClearWindow(GLFWwindow* window);

extern void	shWindowCreateSurface(ShEngine* p_engine);

extern void shUpdateInput(ShWindow* p_window);

extern void shUpdateWindow(ShEngine* p_engine);

static int shIsKeyPressed(const ShWindow window, const uint32_t key) {
	return window.input.key_events[key] == GLFW_PRESS;
}

static int shIsKeyReleased(const ShWindow window, const uint32_t key) {
	return window.input.key_events[key] == GLFW_RELEASE;
}

static int shIsKeyRepeated(const ShWindow window, const uint32_t key) {
	return window.input.key_events[key] == GLFW_REPEAT;
}

static int shIsMouseButtonPressed(const ShWindow window, const uint32_t button) {
	return window.input.mouse_events[button] == GLFW_PRESS;
}

static int shIsMouseButtonReleased(const ShWindow window, const uint32_t button) {
	return window.input.mouse_events[button] == GLFW_RELEASE;
}

static int shIsMouseButtonRepeated(const ShWindow window, const uint32_t button) {
	return window.input.mouse_events[button] == GLFW_REPEAT;
}



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_WINDOW_H