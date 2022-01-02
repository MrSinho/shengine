#ifndef SH_DESCRIPTOR_HANDLE_H
#define SH_DESCRIPTOR_HANDLE_H

#include <sys/types.h>
#include <sys/stat.h>

#include "shEcsImplementation.h"
#include "shComponents.h"

#ifdef _UNIX
#define stat _stat
#include <unistd.h>
#endif // _UNIX

#ifdef _WIN32
#define stat _stat
#endif // _WIN32

#include <stdio.h>
#include <stdint.h>

typedef struct stat ShFileStats;

typedef struct ShDescriptorHandle {

	const char* path;
	ShFileStats stats0;
	ShFileStats stats1;

} ShDescriptorHandle;


extern uint8_t shListenDescriptor(ShDescriptorHandle* descriptor_handle);

extern void shGetFileStats(const char* path, ShFileStats* stats);

extern void shInitDescriptor(ShDescriptorHandle* descriptor_handle);

extern void shLoadMaterialInfos(const char* path, uint32_t* p_mat_info_count, ShMaterialInfo** pp_mat_infos);

extern void shLoadScene(const char* path, const ShMaterialInfo* p_mat_infos, ShScene* p_scene);

extern void shLoadPhysicsWorld(const char* path, ShPhysicsHost* p_host);

#endif//SH_DESCRIPTOR_HANDLE_H