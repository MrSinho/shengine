#ifndef FGG_TRANSFORM_H
#define FGG_TRANSFORM_H

#include "fggCglmImplementation.h"

#include "fggEcsImplementation.h"

#define FGG_TRANSFORM_MODEL_SIZE sizeof(mat4)


typedef struct FggTransform {

	mat4 model;
	vec3 position;
	vec3 rotation;
	vec3 scale;

	vec3 front;
	vec3 left;
	vec3 up;

} FggTransform;

EZ_ECS_MAKE_COMPONENT_DEFINITIONS(FggTransform, 0)

#endif