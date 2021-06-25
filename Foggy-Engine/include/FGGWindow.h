#ifndef WINDOW_H
#define WINDOW_H

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
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