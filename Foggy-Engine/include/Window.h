#ifndef WINDOW_H
#define WINDOW_H

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <stdint.h>

typedef struct Window {

	GLFWwindow* window;
	uint32_t width, height;
	const char* title;

} Window;

extern void InitGLFW(Window *window);
extern int IsWindowActive(GLFWwindow* window);
extern void PollEvents();
extern void ClearWindow(GLFWwindow* window);

#endif