#include "fggWindow.h"
#include "fggInput.h"

int fggIsKeyPressed(const FggWindow window, int key) {
	if (glfwGetKey(window.window, key) == GLFW_PRESS) { return 1; }
	else { return 0; }
}

int fggIsKeyReleased(const FggWindow window, int key) {
	if (glfwGetKey(window.window, key) == GLFW_RELEASE) { return 1; }
	else { return 0; }
}

int fggIsMouseButtonPressed(const FggWindow window, int button) {
	if (glfwGetMouseButton(window.window, button) == GLFW_PRESS) { return 1; }
	else { return 0; }
}

int fggIsMouseButtonReleased(const FggWindow window, int button) {
	if (glfwGetMouseButton(window.window, button) == GLFW_RELEASE) { return 1; }
	else { return 0; }
}

void fggGetCursorPosition(const FggWindow window, double* pX, double* pY) {
	static double x, y;
	glfwGetCursorPos(window.window, &x, &y);
	if (pX != NULL && pY != NULL) {
		*pX = x;
		*pY = y;
	}
}

void fggMouseOffset(const FggWindow window, double* pdx, double* pdy) {
	double x, y;
	glfwGetCursorPos(window.window, &x, &y);

	*pdx = window.cursorPosX - x;
	*pdy = window.cursorPosY - y;
}