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



uint8_t shSharedSceneRun(void* p_engine, ShApplicationFunc* p_func) {
    shSharedHostError(p_engine != NULL, "invalid engine memory");
    if (p_func == NULL) { return 1; }
    return p_func(p_engine);
}

void shLoadApplication(const char* path, ShApplicationHandle* p_application) {
    shSharedHostError(path != NULL && p_application != NULL, "invalid arguments");

    char* buffer = (char*)shReadText(path, NULL);
    shSharedHostWarning(buffer != NULL, "missing application descriptor");

    json_object* parser = json_tokener_parse(buffer);
    shSharedHostWarning(parser != NULL, "invalid application json format");

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
    p_application->run                = (json_run            == NULL)  ? 1                : (uint8_t)json_object_get_int(json_run);
    p_application->s_start            = (json_start          == NULL)  ? "start"          : (char*)json_object_get_string(json_start);
    p_application->s_thread           = (json_thread         == NULL)  ? "thread"         : (char*)json_object_get_string(json_thread);
    p_application->s_update_pending   = (json_update_pending == NULL)  ? "update_pending" : (char*)json_object_get_string(json_update_pending);
    p_application->s_after_thread     = (json_after_thread   == NULL)  ? "after_thread"   : (char*)json_object_get_string(json_after_thread);
    p_application->s_update           = (json_update         == NULL)  ? "update"         : (char*)json_object_get_string(json_update);
    p_application->s_frame_update     = (json_frame_update   == NULL)  ? "frame_update"   : (char*)json_object_get_string(json_frame_update);
    p_application->s_frame_resize     = (json_frame_resize   == NULL)  ? "frame_resize"   : (char*)json_object_get_string(json_frame_resize);
    p_application->s_close            = (json_close          == NULL)  ? "close"          : (char*)json_object_get_string(json_close);

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

    p_application->shared = shLoadShared(shared_path);
    if (p_application->run) {
        shSharedHostWarning(p_application->shared != NULL, "invalid shared library");
    }
}

void shApplicationLoadSymbols(ShApplicationHandle* p_application) {
    shSharedHostError(p_application != NULL, "invalid application pointer");
    p_application->p_start                               = (ShApplicationFunc*)shSharedLoadSymbol(p_application->shared, p_application->s_start);
    p_application->p_thread                              = (ShApplicationThreadFunc*)shSharedLoadSymbol(p_application->shared, p_application->s_thread);
    p_application->p_update_pending                      = (ShApplicationFunc*)shSharedLoadSymbol(p_application->shared, p_application->s_update_pending);
    p_application->p_after_thread                        = (ShApplicationFunc*)shSharedLoadSymbol(p_application->shared, p_application->s_after_thread);
    p_application->p_update                              = (ShApplicationFunc*)shSharedLoadSymbol(p_application->shared, p_application->s_update);
    p_application->p_frame_update                        = (ShApplicationFunc*)shSharedLoadSymbol(p_application->shared, p_application->s_frame_update);
    p_application->p_frame_resize                        = (ShApplicationFunc*)shSharedLoadSymbol(p_application->shared, p_application->s_frame_resize);
    p_application->p_close                               = (ShApplicationFunc*)shSharedLoadSymbol(p_application->shared, p_application->s_close);
    shSharedHostWarning(p_application->p_start          != NULL, "invalid start function pointer");
    shSharedHostWarning(p_application->p_thread         != NULL, "invalid thread function pointer");
    shSharedHostWarning(p_application->p_update_pending != NULL, "invalid update pending function pointer");
    shSharedHostWarning(p_application->p_after_thread   != NULL, "invalid after thread function pointer");
    shSharedHostWarning(p_application->p_update         != NULL, "invalid update function pointer");
    shSharedHostWarning(p_application->p_frame_update   != NULL, "invalid frame update function pointer");
    shSharedHostWarning(p_application->p_frame_resize   != NULL, "invalid frame resize function pointer");
    shSharedHostWarning(p_application->p_close          != NULL, "invalid close function pointer");
}

#ifdef __cplusplus
}
#endif//__cplusplus