#ifndef FGG_DEFAULT_CAMERA_H
#define FGG_DEFAULT_CAMERA_H

#include <cglm/cglm.h>

typedef struct FggDefaultCamera {

	mat4 view;

} FggDefaultCamera;



extern void fggDefaultCameraUpdate(FggDefaultCamera *camera);

#endif