#ifndef SH_DESCRIPTOR_HANDLE_H
#define SH_DESCRIPTOR_HANDLE_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <sys/types.h>
#include <sys/stat.h>

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

typedef struct ShFd {
	char		path[256];
	ShFileStats stats0;
	ShFileStats stats1;
} ShFd;


typedef struct ShScene ShScene;
typedef struct ShMaterialHost ShMaterialHost;
typedef struct ShVkCore ShVkCore;

extern uint8_t shListenFd(ShFd* descriptor_handle);

#define shGetFileStats(path, stats)\
	stat(path, stats)

#define shInitDescriptor(p_descriptor_handle)\
	shGetFileStats((p_descriptor_handle)->path, &(p_descriptor_handle)->stats0)

extern void shMakeAssetsPath(const char* src_path, char* dst_path);

extern void shLoadMaterials(ShVkCore* p_core, const char* path, uint32_t* p_material_count, ShMaterialHost** pp_materials);

extern void shMaterialsRelease(ShVkCore* p_core, uint32_t* p_mat_info_count, ShMaterialHost** pp_materials);

extern void shLoadScene(const char* path, ShMaterialHost** pp_materials, ShScene* p_scene);

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_DESCRIPTOR_HANDLE_H