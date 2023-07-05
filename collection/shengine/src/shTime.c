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

uint8_t shCallOnTick(
	ShTime*     p_time,
	double      delta_time,
	uint32_t    tick_idx,
	ShTimeFunc* p_func,
	void*       p_arg,
	uint64_t*   p_return_value
) {
	shEngineError(p_time == NULL, "shCallOnTick: invalid time memory",     return 0);
	shEngineError(p_func == NULL, "shCallOnTick: invalid function memory", return 0);
	
	shEngineError(
		tick_idx >= SH_TIME_MAX_TICK_COUNT, 
		"shCallOnTick: invalid tick idx", 
		return 0
	);

	if ((p_time->now - p_time->ticks_last_time[tick_idx]) >= delta_time) {
		if (p_return_value != NULL) {
			(*p_return_value) = p_func(p_arg);
		}
		else {
			p_func(p_arg);
		}
		p_time->ticks_last_time[tick_idx] = p_time->now;
	}

	return 1;
}


#ifdef __cplusplus
}
#endif//__cplusplus