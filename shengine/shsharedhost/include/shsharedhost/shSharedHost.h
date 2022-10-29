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
typedef uint8_t(ShSimulationFunc) (void*);
typedef uint64_t (ShSimulationThreadFunc) (void*);



typedef struct ShSimulationHandle {
	ShSharedHandle          shared;
	uint8_t                 run;
	char*                   s_start;
	char*                   s_thread;
	char*                   s_update_pending;
	char*                   s_after_thread;
	char*                   s_update;
	char*                   s_frame_update;
	char*                   s_frame_resize;
	char*                   s_close;
	ShSimulationFunc*       p_start;
	ShSimulationThreadFunc* p_thread;
	ShSimulationFunc*       p_update_pending;
	ShSimulationFunc*       p_after_thread;
	ShSimulationFunc*       p_update;
	ShSimulationFunc*       p_frame_update;
	ShSimulationFunc*       p_frame_resize;
	ShSimulationFunc*		p_close;
	uint8_t                 after_thread_called;
} ShSimulationHandle;



#define shSharedHostWarning(condition, msg)\
	if (!(int)(condition)) { printf("shsharedhost warning: %s\n", msg); }

#define shSharedHostError(condition, msg)\
	if (!(int)(condition)) { printf("shsharedhost error: %s\n", msg); perror("aborting"); }


extern uint8_t shSharedSceneRun(void* p_engine, ShSimulationFunc* p_func);

extern void shLoadSimulation(const char* path, ShSimulationHandle* p_simulation);

extern void shSimulationLoadSymbols(ShSimulationHandle* p_simulation);

#define shSharedRelease(p_shared)\
        if (p_shared != NULL) { if (*p_shared != NULL) { shSharedFree(*(p_shared)); *(p_shared) = NULL; } }

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_SHARED_HOST_H
