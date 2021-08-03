#ifndef FGG_ECS_IMPLEMENTATION_H
#define FGG_ECS_IMPLEMENTATION_H

#define EZ_ECS_MAX_ENTITIES		5
#define EZ_ECS_MAX_COMPONENTS	4



#include <EZ_ECS.h>

#include "fggVkCore.h"
#include "fggTime.h"
#include "fggProjection.h"


typedef struct FggVkFixedStates FggVkFixedStates;


extern void fggSceneInit(const FggVkCore core, const ezecsScene scene);

extern void fggSceneUpdate(const FggVkCore core, const FggTime time, const ezecsScene scene);

extern void fggSceneRelease(const FggVkCore core, const ezecsScene scene);

#endif