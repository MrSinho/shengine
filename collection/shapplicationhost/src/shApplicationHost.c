#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



#include <shapplicationhost/shApplicationHost.h>
#include <string.h>
#include <stdio.h>


#ifdef _MSC_VER
#pragma warning (disable: 4996 6011)
#endif//_MSC_VER



uint64_t shApplicationRun(
    void*              p_engine,
    ShApplicationFunc* p_func
) {
    shSharedHostError(p_engine == NULL, "shApplicationRun: invalid engine memory", return 0);
    if (p_func == NULL) { return 1; }
    return p_func(p_engine);//all application functions return a uint64_t
}

uint8_t shGetSharedApplication(
    const char*          shared_name,
    const char*          s_start,
    const char*          s_update,
    const char*          s_main_cmd_buffer,
    const char*          s_main_renderpass,
    const char*          s_frame_resize,
    const char*          s_close,
    ShApplicationHost*   p_application
) {
    shSharedHostError(shared_name       == NULL, "shLoadApplication: invalid shared path memory",       return 0);
    shSharedHostError(s_start           == NULL, "shLoadApplication: invalid s_start memory",           return 0);
    shSharedHostError(s_update          == NULL, "shLoadApplication: invalid s_update memory",          return 0);
    shSharedHostError(s_main_cmd_buffer == NULL, "shLoadApplication: invalid s_main_cmd_buffer memory", return 0);
    shSharedHostError(s_main_renderpass == NULL, "shLoadApplication: invalid s_main_renderpass memory", return 0);
    shSharedHostError(s_frame_resize    == NULL, "shLoadApplication: invalid s_frame_resize memory",    return 0);
    shSharedHostError(s_close           == NULL, "shLoadApplication: invalid s_close memory",           return 0);
    shSharedHostError(p_application     == NULL, "shLoadApplication: invalid application memory",       return 0);

    char shared_path[64];
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

    shSharedHostError(p_application->shared == NULL, "shLoadApplication: invalid shared library", return 0);

    p_application->p_start           = (ShApplicationFunc*)       shSharedLoadSymbol(p_application->shared, s_start);
    p_application->p_update          = (ShApplicationFunc*)       shSharedLoadSymbol(p_application->shared, s_update);
    p_application->p_main_cmd_buffer = (ShApplicationFunc*)       shSharedLoadSymbol(p_application->shared, s_main_cmd_buffer);
    p_application->p_main_renderpass = (ShApplicationFunc*)       shSharedLoadSymbol(p_application->shared, s_main_renderpass);
    p_application->p_frame_resize    = (ShApplicationFunc*)       shSharedLoadSymbol(p_application->shared, s_frame_resize);
    p_application->p_close           = (ShApplicationFunc*)       shSharedLoadSymbol(p_application->shared, s_close);

    shSharedHostError(p_application->p_start           == NULL, "shLoadApplication: invalid start function memory",           return 0);
    shSharedHostError(p_application->p_update          == NULL, "shLoadApplication: invalid update function memory",          return 0);
    shSharedHostError(p_application->p_main_cmd_buffer == NULL, "shLoadApplication: invalid main cmd buffer function memory", return 0);
    shSharedHostError(p_application->p_main_renderpass == NULL, "shLoadApplication: invalid main renderpass function memory", return 0);
    shSharedHostError(p_application->p_frame_resize    == NULL, "shLoadApplication: invalid frame resize function memory",    return 0);
    shSharedHostError(p_application->p_close           == NULL, "shLoadApplication: invalid close function memory",           return 0);

    return 1;
}



#ifdef __cplusplus
}
#endif//__cplusplus