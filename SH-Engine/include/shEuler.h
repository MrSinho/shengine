#ifndef SH_EULER_H
#define SH_EULER_H

#include "shLinearAlgebraTypes.h"

#include <math.h>

#define SH_DEGREES_TO_RADIANS(angle) angle * 3.14159265358f / 180.0f

static void shEulerToVector(shvec3 rotation, shvec3 front) {
	front[0] = (float)(sin(rotation[1]) * cos(rotation[0]));
	front[1] = (float)(sin(rotation[0]));
	front[2] = -(float)(cos(rotation[1]) * cos(rotation[0]));
}


#endif//SH_EULER_H