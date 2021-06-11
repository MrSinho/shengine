#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>
#include <stdint.h>

struct Window {
	Window(uint32_t _width, uint32_t _height, const char* title);
	void InitGLFW();
	bool IsActive();
	~Window();

	GLFWwindow* window;

	uint32_t width, height;
	const char* title;
};


#endif