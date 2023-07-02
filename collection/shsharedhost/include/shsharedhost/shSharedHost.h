#ifndef SH_SHARED_HOST_H
#define SH_SHARED_HOST_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



#ifdef _MSC_VER
#pragma warning (disable: 4005 4996 4113)
#endif//_MSC_VER



#ifdef _WIN32
#include <windows.h>
#define shLoadShared(path) LoadLibrary(path)
#define shSharedLoadSymbol GetProcAddress
#define shSharedFree FreeLibrary
#else
#include <dlfcn.h>
#define shLoadShared(path) dlopen(path, RTLD_LAZY) 
#define shSharedLoadSymbol dlsym
#define shSharedFree dlclose
#endif//_WIN32


#include <stdint.h>
#include <stddef.h>
	


typedef void* ShSharedHandle;
typedef uint8_t(ShApplicationFunc) (void*);
typedef uint64_t (ShApplicationThreadFunc) (void*);



typedef struct ShApplicationHandle {
	uint8_t                  run;
	ShSharedHandle           shared;
	ShApplicationFunc*       p_start;
	ShApplicationThreadFunc* p_thread;
	ShApplicationFunc*       p_update_pending;
	ShApplicationFunc*       p_after_thread;
	ShApplicationFunc*       p_update;
	ShApplicationFunc*       p_main_cmd_buffer;
	ShApplicationFunc*       p_main_renderpass;
	ShApplicationFunc*       p_frame_resize;
	ShApplicationFunc*		 p_close;
	uint8_t                  after_thread_called;
} ShApplicationHandle;



#define shSharedHostError(condition, msg, failure_expression)\
	if ((int)(condition)) { printf("shsharedhost error: %s\n", msg); failure_expression; }



extern uint64_t shSharedSceneRun(
	void*              p_engine,
	ShApplicationFunc* p_func
);

extern uint8_t shLoadApplication(
	uint8_t              run,
	const char*          shared_path,
	const char*          s_start,
	const char*          s_thread,
	const char*          s_update_pending,
	const char*          s_after_thread,
	const char*          s_update,
	const char*          s_main_cmd_buffer,
	const char*          s_main_renderpass,
	const char*          s_frame_resize,
	const char*          s_close,
	ShApplicationHandle* p_application
);



#define shSharedRelease(p_shared)\
        if ((p_shared) != NULL) { if (*(p_shared) != NULL) { shSharedFree(*(p_shared)); *(p_shared) = NULL; } }



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_SHARED_HOST_H
