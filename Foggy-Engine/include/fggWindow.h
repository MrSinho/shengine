#ifndef FGG_WINDOW_H
#define FGG_WINDOW_H

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <stdint.h>

typedef struct FggWindow {

	GLFWwindow* window;
	uint32_t width, height;
	const char* title;

} FggWindow;



extern void fggInitGLFW(FggWindow *window);

extern int fggIsWindowActive(GLFWwindow* window);

extern void fggPollEvents();

extern void fggClearWindow(GLFWwindow* window);


#endif