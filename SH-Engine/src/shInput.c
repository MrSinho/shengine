#include "shWindow.h"
#include "shInput.h"

int shIsKeyPressed(const ShWindow window, int key) {
	if (glfwGetKey(window.window, key) == GLFW_PRESS) { return 1; }
	else { return 0; }
}

int shIsKeyReleased(const ShWindow window, int key) {
	if (glfwGetKey(window.window, key) == GLFW_RELEASE) { return 1; }
	else { return 0; }
}

int shIsMouseButtonPressed(const ShWindow window, int button) {
	if (glfwGetMouseButton(window.window, button) == GLFW_PRESS) { return 1; }
	else { return 0; }
}

int shIsMouseButtonReleased(const ShWindow window, int button) {
	if (glfwGetMouseButton(window.window, button) == GLFW_RELEASE) { return 1; }
	else { return 0; }
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