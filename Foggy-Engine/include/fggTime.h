#ifndef FGG_TIME_H
#define FGG_TIME_H

typedef struct FggTime
{
	double now;

	float delta_time;

	float last_time;

} FggTime;

#endif


extern void fggGetTime(FggTime* time);

extern void fggSetTime(double now, FggTime* time);

