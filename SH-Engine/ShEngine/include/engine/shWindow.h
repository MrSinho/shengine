#ifndef SH_WINDOW_H
#define SH_WINDOW_H

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdint.h>

typedef struct ShWindow {
	GLFWwindow*		window;
	uint32_t		width;
	uint32_t		height;
	const char*		title;
	double			cursor_pos_x;
	double			cursor_pos_y;
	const char**	pp_instance_extensions;
	uint32_t		instance_extension_count;
} ShWindow;

typedef struct ShEngine ShEngine;

extern void shWindowSetup(const char* title, const uint32_t width, const uint32_t height, ShWindow* p_window);

extern int shIsWindowActive(GLFWwindow* window);

extern void shPollEvents();

extern void shClearWindow(GLFWwindow* window);

extern void shCreateWindowSurface(ShEngine* p_engine);

#endif//SH_WINDOW_H