#ifndef SH_PROJECTION_H
#define SH_PROJECTION_H

#include "shLinearAlgebraTypes.h"

typedef struct ShWindow ShWindow;

extern void shSetProjection(const ShWindow window, float fov, float nc, float fc, shmat4 projection);

#endif