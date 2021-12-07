#ifndef SH_PROJECTION_H
#define SH_PROJECTION_H

#include "shCglmImplementation.h"



typedef struct ShWindow ShWindow;


extern void shSetProjection(const ShWindow window, float fov, float nc, float fc, mat4 projection);

#endif