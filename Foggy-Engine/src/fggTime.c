#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "fggTime.h"

void fggGetTime(FggTime* time)
{
	time->now = glfwGetTime();
	time->delta_time = (float)time->now - time->last_time;
	time->last_time = (float)time->now;
}

void fggSetTime(double now, FggTime *time)
{
	time->now = now;
	glfwSetTime(time->now);
}
