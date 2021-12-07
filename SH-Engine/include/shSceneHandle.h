#ifndef SH_SCENE_HANDLE_H
#define SH_SCENE_HANDLE_H


#include <shEcsImplementation.h>

#include "shVkCore.h"
#include "shTime.h"



extern void shSceneInit(const ShVkCore core, ShScene* scene);

extern void shSceneUpdate(const ShVkCore core, const ShTime time, ShScene* scene);

extern void shSceneRelease(const ShVkCore core, ShScene* scene);

#endif