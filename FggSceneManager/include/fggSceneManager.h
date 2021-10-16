#ifndef FGG_SCENE_MANAGER_H
#define FGG_SCENE_MANAGER_H

#include <sys/types.h>
#include <sys/stat.h>

#include "fggEcsImplementation.h"
#include "fggComponents.h"

#ifdef _UNIX
#define stat _stat
#include <unistd.h>
#endif // _UNIX

#ifdef _WIN32
#define stat _stat
#endif // _WIN32

#include <stdio.h>

typedef struct stat FggFileStats;



typedef struct FggSceneDescriptorHandle {

	const char* path;
	FggFileStats stats0;
	FggFileStats stats1;

} FggSceneDescriptorHandle;



extern void fggGetFileStats(const char* path, FggFileStats* stats);

extern void fggInitSceneDescriptor(FggSceneDescriptorHandle* descriptor_handle);

extern void fggLoadScene(const char* path, FggScene* p_scene);

extern void fggListenSceneDescriptor(FggSceneDescriptorHandle* descriptor_handle, FggScene* p_scene);

extern void fggUpdateScene(FggScene scene);


#endif // FGG_SCENE_MANAGER_H