#ifndef SH_TIME_H
#define SH_TIME_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <smd/smd.h>

#define SH_PROFILING_TIMERS_MAX_EXT_TIMER_COUNT 1024

typedef enum ShProfilingTimerType {
	SH_PROFILING_TIMER_MAIN_THREAD                 = 0,
	SH_PROFILING_TIMER_APPLICATION_UPDATE          = 1,
	SH_PROFILING_TIMER_APPLICATION_MAIN_CMD_BUFFER = 2,
	SH_PROFILING_TIMER_APPLICATION_MAIN_RENDERPASS = 3,
	SH_PROFILING_TIMER_EXT                         = 4,
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

	uint32_t        ext_count;
	double          ext_start_s [SH_PROFILING_TIMERS_MAX_EXT_TIMER_COUNT];
	double          ext_end_s   [SH_PROFILING_TIMERS_MAX_EXT_TIMER_COUNT];
	double          ext_dtime_ms[SH_PROFILING_TIMERS_MAX_EXT_TIMER_COUNT];
	SmdVarName      ext_names   [SH_PROFILING_TIMERS_MAX_EXT_TIMER_COUNT];

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

extern uint8_t shProfilingTimerStartExt(
	ShProfilingTimer* p_timer,
	SmdVarName        name,
	uint32_t          timer_idx
);

extern uint8_t shProfilingTimerEndExt(
	ShProfilingTimer*  p_timer,
	uint32_t          timer_idx
);



#define SH_TIME_MAX_TICK_COUNT       1024

typedef uint64_t ShTimeFunc(void*);

typedef enum ShTickStatus {
	SH_TICK_UNDEFINED          = 0,
	SH_TICK_INITIALIZED        = 1,
	SH_TICK_WAITING            = 2,
	SH_TICK_EXECUTING          = 3,
	SH_TICK_FINISHED_EXECUTING = 4,
	SH_TICK_STATUS_MAX_ENUM
} ShTickStatus;

typedef struct ShTime {
	double           now;
	double           delta_time;
	double           last_time;
	
	uint32_t         tick_count;
	double           ticks_last_time [SH_TIME_MAX_TICK_COUNT];
	ShTickStatus     ticks_status    [SH_TIME_MAX_TICK_COUNT];
	ShTimeFunc*      p_ticks_funcs   [SH_TIME_MAX_TICK_COUNT];
} ShTime;



extern uint8_t shGetTime(
	ShTime* p_time
);

extern uint8_t shSetTime(
	double  now,
	ShTime* p_time
);

extern uint8_t shCallOnTick(
	ShTime*     p_time,
	double      seconds,
	uint32_t    tick_idx,
	ShTimeFunc* p_func,
	void*       p_arg,
	uint64_t*   p_return_value
);

#define shOnTick(time, seconds, tick_idx, expression)\
	if ((time).ticks_status[tick_idx] == SH_TICK_UNDEFINED) {\
		(time).ticks_status[tick_idx] = SH_TICK_INITIALIZED;\
	}\
	if (\
		(\
			((time).now - (time).ticks_last_time[tick_idx]) >= (double)(seconds)\
		) &&\
		(tick_idx < SH_TIME_MAX_TICK_COUNT) &&\
		(\
			(time).ticks_status[tick_idx] != SH_TICK_UNDEFINED &&\
			(time).ticks_status[tick_idx] != SH_TICK_EXECUTING\
		)\
	) {\
		(time).ticks_status[tick_idx] = SH_TICK_EXECUTING;\
		(time).ticks_last_time[tick_idx] = (time).now;\
		(time).ticks_status[tick_idx] = SH_TICK_FINISHED_EXECUTING;\
	}\
	else {\
		(time).ticks_status[tick_idx] = SH_TICK_WAITING;\
	}

extern void shSleep(
	uint32_t ms
);



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_TIME_H
