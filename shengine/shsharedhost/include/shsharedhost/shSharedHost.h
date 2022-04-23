#ifndef SH_SHARED_HOST_H
#define SH_SHARED_HOST_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#ifdef _MSC_VER
#pragma warning (disable: 4005 4996)
#endif//_MSC_VER

#ifdef _WIN32
#include <windows.h>
#include <libloaderapi.h>
#define shLoadShared(path) LoadLibrary(path)
#define shSharedLoadSymbol GetProcAddress
#define shSharedRelease FreeLibrary
#else
#include <dlfcn.h>
#define shLoadShared(path) dlopen(path, RTLD_LAZY) 
#define shSharedLoadSymbol dlsym
#define shSharedRelease dlclose
#endif//_WIN32

#include <stdint.h>

#include <assert.h>
#include <stddef.h>
	
typedef void* ShSharedHandle;
typedef void (ShEntityFunc) (void*, uint32_t);

static void shSharedSceneRun(ShEntityFunc* p_func, void* p_engine, const uint32_t entity_count) {
	assert(p_engine != NULL);
	if (p_func != NULL) {
		for (uint32_t i = 0; i < entity_count; i++) {
			p_func(p_engine, i);
		}
	}
}

typedef struct ShSimulationHandle {
	ShSharedHandle	shared;
	uint8_t			run;
	char*			s_start;
	char*			s_update;
	char*			s_close;
	ShEntityFunc*	p_start;
	ShEntityFunc*	p_update;
	ShEntityFunc*	p_close;
} ShSimulationHandle;

extern void shLoadSimulation(const char* path, void* p_engine, ShSimulationHandle* p_simulation);

#ifdef _MSC_VER
#pragma warning (disable: 4113)
#endif//_MSC_VER

static void shSimulationLoadSymbols(ShSimulationHandle* p_simulation) {
	assert(p_simulation != NULL);
	p_simulation->p_start	= shSharedLoadSymbol(p_simulation->shared, p_simulation->s_start);
	p_simulation->p_update	= shSharedLoadSymbol(p_simulation->shared, p_simulation->s_update);
	p_simulation->p_close	= shSharedLoadSymbol(p_simulation->shared, p_simulation->s_close);
}

#define shSimulationStart(p_engine)\
	shSharedSceneRun((p_engine)->simulation_host.p_start, p_engine, (p_engine)->scene.entity_count)

#define shSimulationUpdate(p_engine)\
	shSharedSceneRun((p_engine)->simulation_host.p_update, p_engine, (p_engine)->scene.entity_count)

#define shSimulationClose(p_engine)\
	shSharedSceneRun((p_engine)->simulation_host.p_close, p_engine, (p_engine)->scene.entity_count)

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_SHARED_HOST_H
