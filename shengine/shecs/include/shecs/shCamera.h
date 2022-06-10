#ifndef SH_CAMERA_H
#define SH_CAMERA_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "shEcsImplementation.h"

typedef enum ShCameraSetupFlags {
	SH_CAMERA_SETUP_FREE_FLIGHT		= 0b01,
	SH_CAMERA_SETUP_STATIC			= 0b10
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
SH_ECS_MAKE_COMPONENT_DEFINITIONS(ShCamera, 2);

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_CAMERA_H