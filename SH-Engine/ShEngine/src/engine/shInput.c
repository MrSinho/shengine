#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

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

void shGetCursorPosition(ShWindow* p_window) {
	double old_x = p_window->cursor_pos_x;
	double old_y = p_window->cursor_pos_y;
	glfwGetCursorPos(p_window->window, &p_window->cursor_pos_x, &p_window->cursor_pos_y);
	p_window->d_cursor_pos_x = old_x - p_window->cursor_pos_x;
	p_window->d_cursor_pos_y = old_y - p_window->cursor_pos_y;
}

#ifdef __cplusplus
}
#endif//__cplusplus