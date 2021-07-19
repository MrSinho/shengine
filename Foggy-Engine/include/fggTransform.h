#ifndef FGG_TRANSFORM_H
#define FGG_TRANSFORM_H

#include "fggCglmImplementation.h"

#include "fggEcsImplementation.h"

#include "fggExport.h"

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

FGG_DEFINE_EXPORT_OFFSET(FggTransform, 0);

FGG_DEFINE_EXPORT_SIZE(FggTransform, sizeof(FggTransform));

#endif