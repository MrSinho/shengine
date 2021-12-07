#include "shEuler.h"

#include <math.h>

void shDegreesToVector(vec3 rotation, vec3 front) {
	front[0] =  (float)(sin(rotation[1]) * cos(rotation[0]));
	front[1] =  (float)(sin(rotation[0]));
	front[2] = -(float)(cos(rotation[1]) * cos(rotation[0]));
}

float shDegreesToRadians(float rot) {
	return rot * 3.14159265358f / 180.0f;
}
