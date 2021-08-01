#include "FggEuler.h"

#include <math.h>

void fggDegreesToVector(vec3 rotation) {
	rotation[0] = (float)(cos(rotation[1]) * cos(rotation[0]));
	rotation[1] = (float)(sin(rotation[0]));
	rotation[2] = (float)(sin(rotation[1]) * cos(rotation[0]));
}

float fggDegreesToRadians(float rot) {
	return rot * 3.14159265358f / 180.0f;
}
