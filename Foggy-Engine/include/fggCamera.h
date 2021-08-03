#ifndef FGG_CAMERA_H
#define FGG_CAMERA_H

#include "fggCglmImplementation.h"
#include "fggEcsImplementation.h"

typedef enum FggCameraSetupFlags {
	FGG_CAMERA_SETUP_FREE_FLIGHT_BIT = 0x01,
} FggCameraSetupFlags;

typedef struct FggCamera{

	float	fov;
	float	nc; 
	float	fc;
	mat4	projection;
	mat4	view;

	FggCameraSetupFlags flags;

} FggCamera;

EZ_ECS_MAKE_COMPONENT_DEFINITIONS(FggCamera, 3);


#endif