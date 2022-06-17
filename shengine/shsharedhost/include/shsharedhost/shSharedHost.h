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
typedef void (ShSimulationFunc) (void*);



typedef struct ShSimulationHandle {
	ShSharedHandle		shared;
	uint8_t				run;
	char*				s_start;
	char*				s_update;
	char*				s_close;
	ShSimulationFunc*	p_start;
	ShSimulationFunc*	p_update;
	ShSimulationFunc*	p_close;
} ShSimulationHandle;



#define shSharedHostWarning(condition, msg)\
	if (!(int)(condition)) { printf("shsharedhost warning: %s\n", msg); }

#define shSharedHostError(condition, msg)\
	if (!(int)(condition)) { printf("shsharedhost error: %s\n", msg); perror("aborting"); }


extern void shSharedSceneRun(ShSimulationFunc* p_func, void* p_engine, const uint32_t entity_count);

extern void shLoadSimulation(const char* path, void* p_engine, ShSimulationHandle* p_simulation);

extern void shSimulationLoadSymbols(ShSimulationHandle* p_simulation);

#define shSimulationStart(p_engine)\
	if ((p_engine)->simulation_host.p_start != NULL) { shSharedSceneRun((p_engine)->simulation_host.p_start, p_engine, (p_engine)->scene.entity_count); }

#define shSimulationUpdate(p_engine)\
	if ((p_engine)->simulation_host.p_update != NULL) { shSharedSceneRun((p_engine)->simulation_host.p_update, p_engine, (p_engine)->scene.entity_count); }

#define shSimulationClose(p_engine)\
	if ((p_engine)->simulation_host.p_close != NULL) { shSharedSceneRun((p_engine)->simulation_host.p_close, p_engine, (p_engine)->scene.entity_count); }



#define shSharedRelease(p_shared)\
	if (p_shared != NULL) { if (*p_shared != NULL) { shSharedFree(*(p_shared)); *(p_shared) = NULL; } }

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_SHARED_HOST_H
