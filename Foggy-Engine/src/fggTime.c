#include <GLFW/glfw3.h>

#include "fggTime.h"

void fggGetTime(FggTime* time)
{
	time->now = glfwGetTime();
	time->deltaTime = (float)time->now - time->lastTime;
	time->lastTime = (float)time->now;
}

void fggSetTime(double now, FggTime *time)
{
	time->now = now;
	glfwSetTime(time->now);
}
