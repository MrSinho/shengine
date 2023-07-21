#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "shengine/shTime.h"
#include "shengine/shEngine.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // UNIX



uint8_t shProfilingTimerStart(
	ShProfilingTimer*    p_timer,
	ShProfilingTimerType type
) {
	shEngineError(p_timer == NULL, "shProfilingTimerStart: invalid timer memory", return 0);
	
	shEngineError(
		type >= SH_PROFILING_TIMER_MAX_ENUM,
		"shProfilingTimerStart: invalid timer type",
		return 0
	);

	switch (type) {
	case SH_PROFILING_TIMER_MAIN_THREAD:
		p_timer->main_thread_start_s = glfwGetTime();
		break;
	case SH_PROFILING_TIMER_APPLICATION_UPDATE:
		p_timer->application_update_start_s = glfwGetTime();
		break;
	case SH_PROFILING_TIMER_APPLICATION_MAIN_CMD_BUFFER:
		p_timer->application_main_cmd_buffer_start_s = glfwGetTime();
		break;
	case SH_PROFILING_TIMER_APPLICATION_MAIN_RENDERPASS:
		p_timer->application_main_renderpass_start_s = glfwGetTime();
		break;
	}

	return 1;
}

uint8_t shProfilingTimerEnd(
	ShProfilingTimer*    p_timer,
	ShProfilingTimerType type
)  {
	shEngineError(p_timer == NULL, "shProfilingTimerEnd: invalid timer memory", return 0);
	
	shEngineError(
		type >= SH_PROFILING_TIMER_MAX_ENUM,
		"shProfilingTimerEnd: invalid timer type",
		return 0
	);

	switch (type) {
	case SH_PROFILING_TIMER_MAIN_THREAD:
		p_timer->main_thread_end_s    = glfwGetTime();
		p_timer->main_thread_dtime_ms = (p_timer->main_thread_end_s - p_timer->main_thread_start_s) * 1000.0;
		break;
	case SH_PROFILING_TIMER_APPLICATION_UPDATE:
		p_timer->application_update_end_s    = glfwGetTime();
		p_timer->application_update_dtime_ms = (p_timer->application_update_end_s - p_timer->application_update_start_s) * 1000.0;
		break;
	case SH_PROFILING_TIMER_APPLICATION_MAIN_CMD_BUFFER:
		p_timer->application_main_cmd_buffer_end_s    = glfwGetTime();
		p_timer->application_main_cmd_buffer_dtime_ms = (p_timer->application_main_cmd_buffer_end_s - p_timer->application_main_cmd_buffer_start_s) * 1000.0;
		break;
	case SH_PROFILING_TIMER_APPLICATION_MAIN_RENDERPASS:
		p_timer->application_main_renderpass_end_s    = glfwGetTime();
		p_timer->application_main_renderpass_dtime_ms = (p_timer->application_main_renderpass_end_s - p_timer->application_main_renderpass_start_s) * 1000.0;
		break;
	}

	return 1;
}

uint8_t shProfilingTimerStartExt(
	ShProfilingTimer* p_timer,
	SmdVarName        name,
	uint32_t          timer_idx
) {
	shEngineError(p_timer == NULL, "shProfilingTimerStartExt: invalid timer memory", return 0);
	
	shEngineError(
		timer_idx >= SH_PROFILING_TIMERS_MAX_EXT_TIMER_COUNT,
		"shProfilingTimerStartExt: invalid timer idx",
		return 0
	);

	p_timer->ext_start_s[timer_idx] = glfwGetTime();
	strcpy(p_timer->ext_names[timer_idx], name);

	return 1;
}

uint8_t shProfilingTimerEndExt(
	ShProfilingTimer* p_timer,
	uint32_t          timer_idx
)  {
	shEngineError(p_timer == NULL, "shProfilingTimerEndExt: invalid timer memory", return 0);
	
	shEngineError(
		timer_idx >= SH_PROFILING_TIMERS_MAX_EXT_TIMER_COUNT,
		"shProfilingTimerStartExt: invalid timer idx",
		return 0
	);

	p_timer->ext_end_s   [timer_idx] =  glfwGetTime();
	p_timer->ext_dtime_ms[timer_idx] = (p_timer->ext_end_s[timer_idx] - p_timer->ext_start_s[timer_idx]) * 1000.0;

	return 1;
}

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
	double      seconds,
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

	if (p_time->ticks_status[tick_idx] == SH_TICK_UNDEFINED) {
		p_time->ticks_status[tick_idx] =  SH_TICK_INITIALIZED;
	}

	if ((
		(p_time->now - p_time->ticks_last_time[tick_idx]) >= seconds
		) 
		&& 
		(
		p_time->ticks_status[tick_idx] != SH_TICK_UNDEFINED &&
		p_time->ticks_status[tick_idx] != SH_TICK_EXECUTING
		) 
	) {
		p_time->ticks_status[tick_idx] = SH_TICK_EXECUTING;
		if (p_return_value != NULL) {
			(*p_return_value) = p_func(p_arg);
		}
		else {
			p_func(p_arg);
		}
		p_time->ticks_last_time[tick_idx] = p_time->now;
		p_time->ticks_status[tick_idx] = SH_TICK_FINISHED_EXECUTING;
	}
	else {
		p_time->ticks_status[tick_idx] = SH_TICK_WAITING;
	}

	return 1;
}

void shSleep(uint32_t ms) {
#ifdef _WIN32
	Sleep(ms);
#else
	usleep(ms * 1000);
#endif
}



#ifdef __cplusplus
}
#endif//__cplusplus