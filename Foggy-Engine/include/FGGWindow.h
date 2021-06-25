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

struct FGGWindow {

	GLFWwindow* window;
	uint32_t width, height;
	const char* title;

};

extern void InitGLFW(FGGWindow *window);
extern bool IsWindowActive(const FGGWindow &window);
extern void ClearWindow(FGGWindow *window);

#endif