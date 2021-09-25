#ifndef FGG_ECS_IMPLEMENTATION_H
#define FGG_ECS_IMPLEMENTATION_H

#define FGG_ECS_MAX_ENTITIES	15
#define FGG_ECS_MAX_COMPONENTS	4



#include <fggECS.h>

#include "fggVkCore.h"
#include "fggTime.h"
#include "fggProjection.h"


typedef struct FggVkFixedStates FggVkFixedStates;


extern void fggSceneInit(const FggVkCore core, const FggScene scene);

extern void fggSceneUpdate(const FggVkCore core, const FggTime time, const FggScene scene);

extern void fggSceneRelease(const FggVkCore core, const FggScene scene);

#endif