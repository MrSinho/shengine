#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



#include <shsharedhost/shSharedHost.h>

#include <shfd/shFile.h>
#include <json.h>

#include <string.h>
#include <stdio.h>


#ifdef _MSC_VER
#pragma warning (disable: 4996 6011)
#endif//_MSC_VER



void shSharedSceneRun(ShSimulationFunc* p_func, void* p_engine, const uint32_t entity_count) {
    shSharedHostError(p_engine != NULL, "invalid arguments");
    p_func(p_engine);
}

void shLoadSimulation(const char* path, void* p_engine, ShSimulationHandle* p_simulation) {
    shSharedHostError(path != NULL && p_engine != NULL && p_simulation != NULL, "invalid arguments");

    char* buffer = (char*)shReadText(path, NULL);
    shSharedHostWarning(buffer != NULL, "missing simulation descriptor");

    json_object* parser = json_tokener_parse(buffer);
    shSharedHostWarning(parser != NULL, "invalid simulation json format");

    json_object* json_run = json_object_object_get(parser, "run");
    json_object* json_name = json_object_object_get(parser, "name");
    json_object* json_start = json_object_object_get(parser, "start");
    json_object* json_update = json_object_object_get(parser, "update");
    json_object* json_close = json_object_object_get(parser, "close");

    const char* shared_name = (json_name == NULL)   ? "name"    : json_object_get_string(json_name);
    p_simulation->run       = (json_run == NULL)    ? 1         : (uint8_t)json_object_get_int(json_run);
    p_simulation->s_start   = (json_start == NULL)  ? "start"   : (char*)json_object_get_string(json_start);
    p_simulation->s_update  = (json_update == NULL) ? "update"  : (char*)json_object_get_string(json_update);
    p_simulation->s_close   = (json_close == NULL)  ? "close"   : (char*)json_object_get_string(json_close);

    free(buffer);

    char shared_path[256];
#ifndef _MSC_VER
    strcpy(shared_path, "lib");
    strcat(shared_path, shared_name);
#else
    strcpy(shared_path, shared_name);

#endif//_MSC_VER
#ifdef _MSC_VER
    strcat(shared_path, ".dll");
#else
    strcat(shared_path, ".so");
#endif//_MSC_VER

    p_simulation->shared = shLoadShared(shared_path);
    if (p_simulation->run) {
        shSharedHostWarning(p_simulation->shared != NULL, "invalid shared library");
    }
}

void shSimulationLoadSymbols(ShSimulationHandle* p_simulation) {
    shSharedHostError(p_simulation != NULL, "invalid simulation pointer");
    p_simulation->p_start   = shSharedLoadSymbol(p_simulation->shared, p_simulation->s_start);
    p_simulation->p_update  = shSharedLoadSymbol(p_simulation->shared, p_simulation->s_update);
    p_simulation->p_close   = shSharedLoadSymbol(p_simulation->shared, p_simulation->s_close);
    shSharedHostWarning(p_simulation->p_start != NULL, "invalid start function pointer");
    shSharedHostWarning(p_simulation->p_update != NULL, "invalid update function pointer");
    shSharedHostWarning(p_simulation->p_close != NULL, "invalid close function pointer");
}

#ifdef __cplusplus
}
#endif//__cplusplus