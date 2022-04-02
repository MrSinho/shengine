#ifndef SH_TIME_H
#define SH_TIME_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

typedef struct ShTime {
	double now;
	double delta_time;
	double last_time;
} ShTime;

extern void shGetTime(ShTime* time);

extern void shSetTime(double now, ShTime* time);

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_TIME_H
