#ifndef SH_PROFILING_H
#define SH_PROFILING_H
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



#include "shengine/shEngineVkCore.h"

#include <smd/smd.h>



#define SH_PROFILING_TIMER_MAX_EXT_COUNT 1024

typedef enum ShProfilingTimerType {
	SH_PROFILING_TIMER_MAIN_THREAD                 = 0,
	SH_PROFILING_TIMER_APPLICATION_UPDATE          = 1,
	SH_PROFILING_TIMER_APPLICATION_MAIN_CMD_BUFFER = 2,
	SH_PROFILING_TIMER_APPLICATION_MAIN_RENDERPASS,
	SH_PROFILING_TIMER_MAIN_CMD_BUFFER_WAIT_0,
	SH_PROFILING_TIMER_MAIN_CMD_BUFFER_WAIT_1,
	SH_PROFILING_TIMER_MAIN_CMD_BUFFER_WAIT_2,
	SH_PROFILING_TIMER_MAIN_CMD_BUFFER_WAIT_3,
	SH_PROFILING_TIMER_MAIN_CMD_BUFFER_WAIT_4,
	SH_PROFILING_TIMER_MAIN_CMD_BUFFER_WAIT_5,
	SH_PROFILING_TIMER_MAIN_CMD_BUFFER_WAIT_6,
	SH_PROFILING_TIMER_EXT,
	SH_PROFILING_TIMER_MAX_ENUM
} ShProfilingTimerType;

typedef struct ShProfilingTimer {
	double          main_profiling_tool_last_time;
			        
	double          main_thread_start_s;
	double          main_thread_end_s;
	double          main_thread_dtime_ms;
		            
	double          main_graphics_queue_start_s;
	double          main_graphics_queue_end_s;
	double          main_graphics_queue_dtime_ms;

	double          application_update_start_s;
	double          application_update_end_s;
	double          application_update_dtime_ms;

	double          application_main_cmd_buffer_start_s;
	double          application_main_cmd_buffer_end_s;
	double          application_main_cmd_buffer_dtime_ms;
	
	double          application_main_renderpass_start_s;
	double          application_main_renderpass_end_s;
	double          application_main_renderpass_dtime_ms;

	double          main_cmd_buffer_wait_start_s[SH_ENGINE_MAX_SWAPCHAIN_IMAGE_COUNT];
	double          main_cmd_buffer_wait_end_s  [SH_ENGINE_MAX_SWAPCHAIN_IMAGE_COUNT];
	double          main_cmd_buffer_wait_dtime_s[SH_ENGINE_MAX_SWAPCHAIN_IMAGE_COUNT];

	uint32_t        ext_count;
	double          ext_start_s [SH_PROFILING_TIMER_MAX_EXT_COUNT];
	double          ext_end_s   [SH_PROFILING_TIMER_MAX_EXT_COUNT];
	double          ext_dtime_ms[SH_PROFILING_TIMER_MAX_EXT_COUNT];
	SmdVarName      ext_names   [SH_PROFILING_TIMER_MAX_EXT_COUNT];

	SmdExportHandle export;
} ShProfilingTimer;



extern uint8_t shProfilingTimerStart(
	ShProfilingTimer*    p_timer,
	ShProfilingTimerType type
);

extern uint8_t shProfilingTimerEnd(
	ShProfilingTimer*    p_timer,
	ShProfilingTimerType type
);

extern uint8_t shProfilingTimerSetExtCount(
	ShProfilingTimer* p_timer,
	uint32_t          ext_count
);

extern uint8_t shProfilingTimerStartExt(
	ShProfilingTimer* p_timer,
	SmdVarName        name,
	uint32_t          timer_idx
);

extern uint8_t shProfilingTimerEndExt(
	ShProfilingTimer*  p_timer,
	uint32_t          timer_idx
);



#ifdef __cplusplus
}
#endif//__cplusplus
#endif//SH_PROFILING_H