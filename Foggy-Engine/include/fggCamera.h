#ifndef FGG_CAMERA_H
#define FGG_CAMERA_H

#include "fggCglmImplementation.h"
#include "fggEcsImplementation.h"

typedef struct FggCamera{

	float	fov;
	float	nc; 
	float	fc;
	mat4	projection;
	mat4	view;

} FggCamera;

EZ_ECS_MAKE_COMPONENT_DEFINITIONS(FggCamera, 3);


#endif