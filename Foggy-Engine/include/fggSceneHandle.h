#ifndef FGG_SCENE_HANDLE_H
#define FGG_SCENE_HANDLE_H


#include <fggEcsImplementation.h>

#include "fggVkCore.h"
#include "fggTime.h"



extern void fggSceneInit(const FggVkCore core, const FggScene scene);

extern void fggSceneUpdate(const FggVkCore core, const FggTime time, const FggScene scene);

extern void fggSceneRelease(const FggVkCore core, const FggScene scene);

#endif