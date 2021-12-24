#ifndef SH_CAMERA_H
#define SH_CAMERA_H

#include "shEcsImplementation.h"

typedef enum ShCameraSetupFlags {
	SH_CAMERA_SETUP_FREE_FLIGHT_BIT	= 0x01,
	SH_CAMERA_SETUP_STATIC_BIT			= 0x02,
} ShCameraSetupFlags;

typedef struct ShCamera {

	float	fov;
	float	nc;
	float	fc;
	float 	speed;
	float	projection[4][4];
	float	view[4][4];

	ShCameraSetupFlags flags;

} ShCamera;

SH_ECS_MAKE_COMPONENT_DEFINITIONS(ShCamera, 3);


#endif