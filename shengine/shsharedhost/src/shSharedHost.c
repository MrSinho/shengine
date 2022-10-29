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



uint8_t shSharedSceneRun(void* p_engine, ShSimulationFunc* p_func) {
    shSharedHostError(p_engine != NULL, "invalid engine memory");
    if (p_func == NULL) { return 1; }
    return p_func(p_engine);
}

void shLoadSimulation(const char* path, ShSimulationHandle* p_simulation) {
    shSharedHostError(path != NULL && p_simulation != NULL, "invalid arguments");

    char* buffer = (char*)shReadText(path, NULL);
    shSharedHostWarning(buffer != NULL, "missing simulation descriptor");

    json_object* parser = json_tokener_parse(buffer);
    shSharedHostWarning(parser != NULL, "invalid simulation json format");

    json_object* json_run            = json_object_object_get(parser, "run");
    json_object* json_name           = json_object_object_get(parser, "name");
    json_object* json_start          = json_object_object_get(parser, "start");
    json_object* json_thread         = json_object_object_get(parser, "thread");
    json_object* json_update_pending = json_object_object_get(parser, "update_pending");
    json_object* json_after_thread   = json_object_object_get(parser, "after_thread");
    json_object* json_update         = json_object_object_get(parser, "update");
    json_object* json_frame_update   = json_object_object_get(parser, "frame_update");
    json_object* json_frame_resize   = json_object_object_get(parser, "frame_resize");
    json_object* json_close          = json_object_object_get(parser, "close");

    const char* shared_name          = (json_name           == NULL)  ? "name"           : json_object_get_string(json_name);
    p_simulation->run                = (json_run            == NULL)  ? 1                : (uint8_t)json_object_get_int(json_run);
    p_simulation->s_start            = (json_start          == NULL)  ? "start"          : (char*)json_object_get_string(json_start);
    p_simulation->s_thread           = (json_thread         == NULL)  ? "thread"         : (char*)json_object_get_string(json_thread);
    p_simulation->s_update_pending   = (json_update_pending == NULL)  ? "update_pending" : (char*)json_object_get_string(json_update_pending);
    p_simulation->s_after_thread     = (json_after_thread   == NULL)  ? "after_thread"   : (char*)json_object_get_string(json_after_thread);
    p_simulation->s_update           = (json_update         == NULL)  ? "update"         : (char*)json_object_get_string(json_update);
    p_simulation->s_frame_update     = (json_frame_update   == NULL)  ? "frame_update"   : (char*)json_object_get_string(json_frame_update);
    p_simulation->s_frame_resize     = (json_frame_resize   == NULL)  ? "frame_resize"   : (char*)json_object_get_string(json_frame_resize);
    p_simulation->s_close            = (json_close          == NULL)  ? "close"          : (char*)json_object_get_string(json_close);

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
    p_simulation->p_start                               = (ShSimulationFunc*)shSharedLoadSymbol(p_simulation->shared, p_simulation->s_start);
    p_simulation->p_thread                              = (ShSimulationThreadFunc*)shSharedLoadSymbol(p_simulation->shared, p_simulation->s_thread);
    p_simulation->p_update_pending                      = (ShSimulationFunc*)shSharedLoadSymbol(p_simulation->shared, p_simulation->s_update_pending);
    p_simulation->p_after_thread                        = (ShSimulationFunc*)shSharedLoadSymbol(p_simulation->shared, p_simulation->s_after_thread);
    p_simulation->p_update                              = (ShSimulationFunc*)shSharedLoadSymbol(p_simulation->shared, p_simulation->s_update);
    p_simulation->p_frame_update                        = (ShSimulationFunc*)shSharedLoadSymbol(p_simulation->shared, p_simulation->s_frame_update);
    p_simulation->p_frame_resize                        = (ShSimulationFunc*)shSharedLoadSymbol(p_simulation->shared, p_simulation->s_frame_resize);
    p_simulation->p_close                               = (ShSimulationFunc*)shSharedLoadSymbol(p_simulation->shared, p_simulation->s_close);
    shSharedHostWarning(p_simulation->p_start          != NULL, "invalid start function pointer");
    shSharedHostWarning(p_simulation->p_thread         != NULL, "invalid thread function pointer");
    shSharedHostWarning(p_simulation->p_update_pending != NULL, "invalid update pending function pointer");
    shSharedHostWarning(p_simulation->p_after_thread   != NULL, "invalid after thread function pointer");
    shSharedHostWarning(p_simulation->p_update         != NULL, "invalid update function pointer");
    shSharedHostWarning(p_simulation->p_frame_update   != NULL, "invalid frame update function pointer");
    shSharedHostWarning(p_simulation->p_frame_resize   != NULL, "invalid frame resize function pointer");
    shSharedHostWarning(p_simulation->p_close          != NULL, "invalid close function pointer");
}

#ifdef __cplusplus
}
#endif//__cplusplus