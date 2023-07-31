#ifndef SH_TIME_H
#define SH_TIME_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



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

#define shOnTick(time, seconds_d, tick_idx, expression)\
	if ((time).ticks_status[tick_idx] == SH_TICK_UNDEFINED) {\
		(time).ticks_status[tick_idx] = SH_TICK_INITIALIZED;\
	}\
	if (\
		(\
			((time).now - (time).ticks_last_time[tick_idx]) >= (double)(seconds_d)\
		) &&\
		(tick_idx < SH_TIME_MAX_TICK_COUNT) &&\
		(\
			(time).ticks_status[tick_idx] != SH_TICK_UNDEFINED &&\
			(time).ticks_status[tick_idx] != SH_TICK_EXECUTING\
		)\
	) {\
		(time).ticks_status[tick_idx] = SH_TICK_EXECUTING;\
		expression;\
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
