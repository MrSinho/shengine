#ifndef WINDOW_H
#define WINDOW_H

#ifdef WIN32
	#define GLFW_EXPOSE_NATIVE_WIN32
#endif

#ifdef __linux__
	#define GLFW_EXPOSE_NATIVE_X11
#endif

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <stdint.h>

typedef struct Window {

	GLFWwindow* window;
	uint32_t width, height;
	const char* title;

} Window;

extern void InitGLFW(Window *window);
extern bool IsWindowActive(const Window &window);
extern void ClearWindow(Window *window);

#endif