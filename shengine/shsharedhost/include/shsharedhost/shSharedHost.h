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
	ShSharedHandle           shared;
	uint8_t                  run;
	char*                    s_start;
	char*                    s_thread;
	char*                    s_update_pending;
	char*                    s_after_thread;
	char*                    s_update;
	char*                    s_frame_update;
	char*                    s_frame_resize;
	char*                    s_close;
	ShApplicationFunc*       p_start;
	ShApplicationThreadFunc* p_thread;
	ShApplicationFunc*       p_update_pending;
	ShApplicationFunc*       p_after_thread;
	ShApplicationFunc*       p_update;
	ShApplicationFunc*       p_frame_update;
	ShApplicationFunc*       p_frame_resize;
	ShApplicationFunc*		 p_close;
	uint8_t                  after_thread_called;
} ShApplicationHandle;



#define shSharedHostWarning(condition, msg)\
	if (!(int)(condition)) { printf("shsharedhost warning: %s\n", msg); }

#define shSharedHostError(condition, msg)\
	if (!(int)(condition)) { printf("shsharedhost error: %s\n", msg); perror("aborting"); }


extern uint8_t shSharedSceneRun(void* p_engine, ShApplicationFunc* p_func);

extern void shLoadSimulation(const char* path, ShApplicationHandle* p_application);

extern void shSimulationLoadSymbols(ShApplicationHandle* p_application);

#define shSharedRelease(p_shared)\
        if (p_shared != NULL) { if (*p_shared != NULL) { shSharedFree(*(p_shared)); *(p_shared) = NULL; } }

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_SHARED_HOST_H
