#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "shengine/shWindow.h"
#include "shengine/shInput.h"

void shGetCursorPosition(ShWindow* p_window) {
	double old_x = p_window->input.cursor_pos_x;
	double old_y = p_window->input.cursor_pos_y;
	glfwGetCursorPos(p_window->window, &p_window->input.cursor_pos_x, &p_window->input.cursor_pos_y);
	p_window->input.d_cursor_pos_x = old_x - p_window->input.cursor_pos_x;
	p_window->input.d_cursor_pos_y = old_y - p_window->input.cursor_pos_y;
}

#ifdef __cplusplus
}
#endif//__cplusplus