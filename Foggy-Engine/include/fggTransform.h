#ifndef FGG_TRANSFORM_H
#define FGG_TRANSFORM_H

#include <cglm/cglm.h>

#include "fggEcsImplementation.h"

typedef struct FggTransform {

	mat4 model;
	vec3 position;
	vec3 rotation;
	vec3 scale;

} FggTransform;

EZ_ECS_MAKE_COMPONENT_DEFINITIONS(FggTransform, 0)


#endif