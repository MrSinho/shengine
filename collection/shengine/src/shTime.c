#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "shengine/shTime.h"
#include "shengine/shEngine.h"


uint8_t shGetTime(
	ShTime* p_time
) {
	shEngineError(p_time == NULL, "shGetTime: invalid time memory", return 0);

	p_time->now        = glfwGetTime();
	p_time->delta_time = p_time->now - p_time->last_time;
	p_time->last_time  = p_time->now;

	return 1;
}

uint8_t shSetTime(
	double  now,
	ShTime* p_time
) {
	shEngineError(p_time == NULL, "shSetTime: invalid time memory", return 0);

	p_time->delta_time = 0.0;
	p_time->last_time  = now;
	p_time->now        = now;

	glfwSetTime(p_time->now);

	return 1;
}



#ifdef __cplusplus
}
#endif//__cplusplus