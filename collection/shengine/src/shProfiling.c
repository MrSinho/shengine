#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "shengine/shEngine.h"
#include "shengine/shProfiling.h"

#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
#pragma warning (disable: 6385 6386)
#endif//_MSC_VER



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

	if (type >= SH_PROFILING_TIMER_MAIN_CMD_BUFFER_WAIT_0 && ((type - SH_PROFILING_TIMER_MAIN_CMD_BUFFER_WAIT_0) < SH_ENGINE_MAX_SWAPCHAIN_IMAGE_COUNT)) {
		p_timer->main_cmd_buffer_wait_start_s[type - SH_PROFILING_TIMER_MAIN_CMD_BUFFER_WAIT_0] = glfwGetTime();
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

	if (type >= SH_PROFILING_TIMER_MAIN_CMD_BUFFER_WAIT_0 && ((type - SH_PROFILING_TIMER_MAIN_CMD_BUFFER_WAIT_0) < SH_ENGINE_MAX_SWAPCHAIN_IMAGE_COUNT)) {
		uint32_t submission = type - SH_PROFILING_TIMER_MAIN_CMD_BUFFER_WAIT_0;
		p_timer->main_cmd_buffer_wait_end_s  [submission] = glfwGetTime();
		p_timer->main_cmd_buffer_wait_dtime_s[submission] = (p_timer->main_cmd_buffer_wait_end_s[submission] - p_timer->main_cmd_buffer_wait_start_s[submission]) * 1000;
	}

	return 1;
}

uint8_t shProfilingTimerSetExtCount(
	ShProfilingTimer* p_timer,
	uint32_t          ext_count
) {
	shEngineError(p_timer == NULL, "shProfilingTimerSetExtCount: invalid timer memory", return 0);

	shEngineError(
		ext_count > SH_PROFILING_TIMER_MAX_EXT_COUNT,
		"shProfilingTimerSetExtCount: invalid ext count",
		return 0
	);

	p_timer->ext_count = ext_count;

	return 1;
}

uint8_t shProfilingTimerStartExt(
	ShProfilingTimer* p_timer,
	SmdVarName        name,
	uint32_t          timer_idx
) {
	shEngineError(p_timer == NULL, "shProfilingTimerStartExt: invalid timer memory", return 0);
	
	shEngineError(
		timer_idx >= SH_PROFILING_TIMER_MAX_EXT_COUNT,
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
) {
	shEngineError(p_timer == NULL, "shProfilingTimerEndExt: invalid timer memory", return 0);

	shEngineError(
		timer_idx >= SH_PROFILING_TIMER_MAX_EXT_COUNT,
		"shProfilingTimerStartExt: invalid timer idx",
		return 0
	);

	p_timer->ext_end_s   [timer_idx] =  glfwGetTime();
	p_timer->ext_dtime_ms[timer_idx] = (p_timer->ext_end_s[timer_idx] - p_timer->ext_start_s[timer_idx]) * 1000.0;

	return 1;
}



#ifdef __cplusplus
}
#endif//__cplusplus