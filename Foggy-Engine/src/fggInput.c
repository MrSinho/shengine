#include "fggWindow.h"
#include "fggInput.h"

//FggInput fggInputInitPrerequisites(const FggWindow window) {
//	FggInput input = {0};
//	glfwSetKeyCallback(window.window, fggInputInitPrerequisites);
//	return input;
//}

void fggInputCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		
	}
	else if (action == GLFW_RELEASE) {

	}
}

int fggIsKeyPressed(const FggWindow window, int key)
{
	if (glfwGetKey(window.window, key) == GLFW_PRESS) { return 1; }
	else { return 0; }
}

int fggIsKeyReleased(const FggWindow window, int key)
{
	if (glfwGetKey(window.window, key) == GLFW_RELEASE) { return 1; }
	else { return 0; }
}

int fggIsMouseButtonPressed(const FggWindow window, int button)
{
	if (glfwGetMouseButton(window.window, button) == GLFW_PRESS) { return 1; }
	else { return 0; }
}

int fggIsMouseButtonReleased(const FggWindow window, int button)
{
	if (glfwGetMouseButton(window.window, button) == GLFW_RELEASE) { return 1; }
	else { return 0; }
}
