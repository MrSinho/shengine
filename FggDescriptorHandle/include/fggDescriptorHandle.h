#ifndef FGG_DESCRIPTOR_HANDLE_H
#define FGG_DESCRIPTOR_HANDLE_H

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



typedef struct FggDescriptorHandle {

	const char* path;
	FggFileStats stats0;
	FggFileStats stats1;

} FggDescriptorHandle;



extern void fggGetFileStats(const char* path, FggFileStats* stats);

extern void fggInitDescriptor(FggDescriptorHandle* descriptor_handle);

extern void fggLoadMaterialInfos(const char* path, uint32_t* p_mat_info_count, FggMaterialInfo** pp_mat_infos);

extern void fggLoadScene(const char* path, const FggMaterialInfo* p_mat_infos, FggScene* p_scene);

extern int fggListenDescriptor(FggDescriptorHandle* descriptor_handle);

extern void fggUpdateScene(FggScene scene);


#endif // FGG_DESCRIPTOR_HANDLE_H