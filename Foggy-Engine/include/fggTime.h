#ifndef FGG_TIME_H
#define FGG_TIME_H

typedef struct FggTime
{
	double now;

	float deltaTime;

	float lastTime;

} FggTime;

#endif


extern void fggGetTime(FggTime* time);

extern void fggSetTime(float now, FggTime* time);

