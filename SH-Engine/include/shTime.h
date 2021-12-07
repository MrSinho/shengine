#ifndef SH_TIME_H
#define SH_TIME_H

typedef struct ShTime
{
	double now;

	float delta_time;

	float last_time;

} ShTime;

#endif


extern void shGetTime(ShTime* time);

extern void shSetTime(double now, ShTime* time);

