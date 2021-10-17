#ifndef FGG_TRANSFORM_H
#define FGG_TRANSFORM_H


#include "fggEcsImplementation.h"

#define FGG_TRANSFORM_MODEL_SIZE sizeof(mat4)


typedef struct FggTransform {

	float model[4][4];
	float position[3];
	float rotation[3];
	float scale[3];

	float front[3];
	float left[3];
	float up[3];

} FggTransform;

FGG_ECS_MAKE_COMPONENT_DEFINITIONS(FggTransform, 0)

#endif