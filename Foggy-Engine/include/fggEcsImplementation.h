#ifndef FGG_ECS_IMPLEMENTATION_H
#define FGG_ECS_IMPLEMENTATION_H

#define EZ_ECS_MAX_ENTITIES		256
#define EZ_ECS_MAX_COMPONENTS	10



#include <EZ_ECS.h>

#include "fggVkCore.h"

#include "fggProjection.h"


typedef struct FggVkFixedStates FggVkFixedStates;


extern void fggSceneInit(const FggVkCore core, const FggVkFixedStates fixedStates, const ezecsScene scene);

extern void fggSceneUpdate(const FggVkCore core, const ezecsScene scene);


#endif