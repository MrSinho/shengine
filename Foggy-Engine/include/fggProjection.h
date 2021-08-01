#ifndef FGG_PROJECTION_H
#define FGG_PROJECTION_H

#include "fggCglmImplementation.h"



typedef struct FggWindow FggWindow;


extern void fggSetProjection(const FggWindow window, float fov, float nc, float fc, mat4 projection);

#endif