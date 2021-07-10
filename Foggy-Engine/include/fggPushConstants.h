#ifndef FGG_PUSH_CONSTANTS_H
#define FGG_PUSH_CONSTANTS_H

#include <cglm/cglm.h>

typedef struct FggWindow FggWindow;

typedef struct FggMeshPushConstants {

	mat4 projection;

} FggMeshPushConstants;



extern void fggPushConstantsUpdate(const FggWindow window, FggMeshPushConstants* meshPushConstants);

#endif