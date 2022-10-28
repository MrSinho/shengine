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



typedef struct ShLoaderIni {
	char simulation_name[256];
	char assets_path[256];
} ShLoaderIni;



typedef struct ShFd {
	char dir[256];
	char filename[256];
	char path[256];
} ShFd;



extern uint8_t shGetIniProperties(const char* loader_ini_dir, ShLoaderIni* p_ini);

extern uint8_t shAppendAssetsPath(const char* engine_assets_path, const char* extension_dir, const char* extension_filename, ShFd* p_fd);



typedef struct ShScene ShScene;
typedef struct ShMaterialHost ShMaterialHost;
typedef struct ShVkCore ShVkCore;



#define shGetFileStats(path, stats)\
	stat(path, stats)

#define shInitDescriptor(p_descriptor_handle)\
	shGetFileStats((p_descriptor_handle)->path, &(p_descriptor_handle)->stats0)

#define shFdError(condition, msg, failure_expression)\
	if ((int)(condition)) { printf("shfd error: %s.\n", msg); failure_expression; }



extern uint8_t shLoadMaterials(ShVkCore* p_core, const char* dir, const char* filename, uint32_t* p_material_count, ShMaterialHost** pp_materials);

extern void shMaterialsRelease(ShVkCore* p_core, uint32_t* p_mat_info_count, ShMaterialHost** pp_materials);

extern uint8_t shLoadScene(const char* dir, const char* filename, const uint32_t material_count, ShMaterialHost** pp_materials, ShScene* p_scene);

extern uint32_t shStringFlagToInt(const char* s_flag);

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_DESCRIPTOR_HANDLE_H