#ifndef SH_EULER_H
#define SH_EULER_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <math.h>

#define SH_DEGREES_TO_RADIANS(angle) (angle) * 3.14159265358f / 180.0f
#define SH_RADIANS_TO_DEGREES(rad)   (rad) / 3.14159265358f * 180.0f

static void shEulerToVector(float* rotation, float* front) {
	front[0] = (float)(sin(rotation[1]) * cos(rotation[0]));
	front[1] = (float)(sin(rotation[0]));
	front[2] = -(float)(cos(rotation[1]) * cos(rotation[0]));
}

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_EULER_H