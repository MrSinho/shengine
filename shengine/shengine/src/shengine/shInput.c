#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "shengine/shWindow.h"
#include "shengine/shInput.h"



void shGetCursorPosition(ShWindow* p_window) {
	float old_x = p_window->input.cursor_pos_x;
	float old_y = p_window->input.cursor_pos_y;


	double double_x = 0;
	double double_y = 0;
	glfwGetCursorPos(p_window->window, &double_x, &double_y);
	
	p_window->input.cursor_pos_x = (float)double_x - (float)p_window->width / 2.0f;
	p_window->input.cursor_pos_y = (float)double_y - (float)p_window->height / 2.0f;
	
	p_window->input.d_cursor_pos_x = old_x - p_window->input.cursor_pos_x;
	p_window->input.d_cursor_pos_y = old_y - p_window->input.cursor_pos_y;
}		



#ifdef __cplusplus
}
#endif//__cplusplus