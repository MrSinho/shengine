#ifndef SH_SHARED_HOST_H
#define SH_SHARED_HOST_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#ifdef _WIN32
#include <windows.h>
#include <libloaderapi.h>
#define shLoadShared(path) LoadLibrary(path)
#define shSharedLoadSymbol GetProcAddress
#define shSharedRelease FreeLibrary
#else
#define shLoadShared(path) dlopen(path, RTLD_LAZY) 
#define shSharedLoadSymbol dlsym
#define shSharedRelease dlclose
#endif//_WIN32

#include <stdint.h>
#include <shengine/shEngine.h>

typedef void* ShSharedHandle;
typedef void (ShEntityFunc) (ShEngine*, uint32_t);

static void shSharedSceneRun(ShEntityFunc* p_func, ShEngine* p_engine) {
	assert(p_engine != NULL);
	if (p_func != NULL) {
		for (uint32_t i = 0; i < p_engine->scene.entity_count; i++) {
			p_func(p_engine, i);
		}
	}
}

typedef struct ShSimulationHandle {
	ShSharedHandle	shared;
	uint8_t			run;
	char*			s_start;
	char*			s_update;
	ShEntityFunc*	p_start;
	ShEntityFunc*	p_update;
} ShSimulationHandle;

extern void shLoadSimulation(const char* path, ShEngine* p_engine, ShSimulationHandle* p_simulation);

static void shSimulationLoadSymbols(ShSimulationHandle* p_simulation) {
	assert(p_simulation != NULL);
	p_simulation->p_start	= shSharedLoadSymbol(p_simulation->shared, p_simulation->s_start);
	p_simulation->p_update	= shSharedLoadSymbol(p_simulation->shared, p_simulation->s_update);
}

#define shSimulationStart(p_simulation, p_engine)\
	shSharedSceneRun((p_simulation)->p_start, p_engine)

#define shSimulationUpdate(p_simulation, p_engine)\
	shSharedSceneRun((p_simulation)->p_update, p_engine)

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_SHARED_HOST_H