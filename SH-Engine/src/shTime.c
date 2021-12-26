#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "shTime.h"

void shGetTime(ShTime* time) {
	time->now = glfwGetTime();
	time->delta_time = time->now - time->last_time;
	time->last_time = time->now;
}

void shSetTime(double now, ShTime *time) {
	time->delta_time = 0.0;
	time->last_time = now;
	time->now = now;
	glfwSetTime(time->now);
}
