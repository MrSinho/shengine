#ifndef SH_LINEAR_ALGEBRA_H
#define SH_LINEAR_ALGEBRA_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



#define _CRT_SECURE_NO_WARNINGS
#pragma warning (disable: 26812 4996)
#include <cglm/cglm.h>

#include <math.h>



#define SH_DEGREES_TO_RADIANS(angle) (angle) * 3.14159265358f / 180.0f
#define SH_RADIANS_TO_DEGREES(rad)   (rad) / 3.14159265358f * 180.0f

typedef struct ShWindow ShWindow;



static void shEulerToVector(float* rotation, float* front) {
	front[0] = (float)(sin(rotation[1]) * cos(rotation[0]));
	front[1] = (float)(sin(rotation[0]));
	front[2] = -(float)(cos(rotation[1]) * cos(rotation[0]));
}


extern uint8_t shSetProjection(
	ShWindow window,
	float    fov,
	float    nc,
	float    fc,
	float    projection[4][4]
);

extern uint8_t shSetView(
	float* p_position,
	float* p_front,
	float* p_up,
	float  view[4][4]
);



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_LINEAR_ALGEBRA_H