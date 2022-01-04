#include "engine/shWindow.h"
#include "engine/shInput.h"

int shIsKeyPressed(const ShWindow window, const uint32_t key) {
	return glfwGetKey(window.window, key) == GLFW_PRESS;
}

int shIsKeyReleased(const ShWindow window, const uint32_t key) {
	return glfwGetKey(window.window, key) == GLFW_RELEASE;
}

int shIsMouseButtonPressed(const ShWindow window, const uint32_t button) {
	return glfwGetMouseButton(window.window, button) == GLFW_PRESS;
}

int shIsMouseButtonReleased(const ShWindow window, const uint32_t button) {
	return glfwGetMouseButton(window.window, button) == GLFW_RELEASE;
}

void shGetCursorPosition(const ShWindow window, double* pX, double* pY) {
	static double x, y;
	glfwGetCursorPos(window.window, &x, &y);
	if (pX != NULL && pY != NULL) {
		*pX = x;
		*pY = y;
	}
}

void shMouseOffset(const ShWindow window, double* pdx, double* pdy) {
	double x, y;
	glfwGetCursorPos(window.window, &x, &y);

	*pdx = window.cursor_pos_x - x;
	*pdy = window.cursor_pos_y - y;
}