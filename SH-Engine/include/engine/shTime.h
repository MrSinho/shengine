#ifndef SH_TIME_H
#define SH_TIME_H

typedef struct ShTime {
	double now;
	double delta_time;
	double last_time;
} ShTime;


extern void shGetTime(ShTime* time);

extern void shSetTime(double now, ShTime* time);

#endif//SH_TIME_H
