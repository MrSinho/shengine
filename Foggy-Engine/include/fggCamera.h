#ifndef FGG_DEFAULT_CAMERA_H
#define FGG_DEFAULT_CAMERA_H

#include "fggCglmImplementation.h"

#include "fggEcsImplementation.h"

typedef struct FggCamera {

	mat4 view;

	vec3 position;
	vec3 rotation;
	vec3 scale;

} FggCamera;

#endif