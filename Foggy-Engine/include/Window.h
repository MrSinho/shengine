#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <stdint.h>

struct Window {
	
	void InitGLFW(uint32_t _width, uint32_t _height, const char* title);
	bool IsActive();
	void Clear();

	GLFWwindow* window;
	uint32_t width, height;
	const char* title;
};


#endif