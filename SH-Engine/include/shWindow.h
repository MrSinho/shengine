#ifndef SH_WINDOW_H
#define SH_WINDOW_H

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <stdint.h>

typedef struct ShWindow {

	GLFWwindow* window;
	uint32_t width, height;
	const char* title;

	double cursor_pos_x;
	double cursor_pos_y;

} ShWindow;



extern void shInitGLFW(ShWindow *window);

extern int shIsWindowActive(GLFWwindow* window);

extern void shPollEvents();

extern void shClearWindow(GLFWwindow* window);


#endif