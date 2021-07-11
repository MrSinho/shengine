#ifndef FGG_PROJECTION_H
#define FGG_PROJECTION_H

#include <cglm/cglm.h>

typedef struct FggWindow FggWindow;

typedef struct FggProjection {

	mat4 projection;

} FggProjection;


extern void fggProjectionUpdate(const FggWindow window, FggProjection* projection);

#endif