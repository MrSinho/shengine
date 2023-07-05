#ifndef SH_TIME_H
#define SH_TIME_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus


#define SH_TIME_MAX_TICK_COUNT 1024



typedef uint64_t ShTimeFunc(void*);



typedef struct ShTime {
	double   now;
	double   delta_time;
	double   last_time;
	uint32_t tick_count;
	double   ticks_last_time[SH_TIME_MAX_TICK_COUNT];
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
	double      delta_time,
	uint32_t    tick_idx,
	ShTimeFunc* p_func,
	void*       p_arg,
	uint64_t*   p_return_value
);

#define shOnTick(time, delta_time, tick_idx, expression)\
	if (((time).now - (time).ticks_last_time[tick_idx]) >= (delta_time) && (tick_idx < SH_TIME_MAX_TICK_COUNT)) {\
		expression;\
		(time).ticks_last_time[tick_idx] = (time).now;\
	}\


#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_TIME_H
