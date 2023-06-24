#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



#include <shsharedhost/shSharedHost.h>
#include <string.h>
#include <stdio.h>


#ifdef _MSC_VER
#pragma warning (disable: 4996 6011)
#endif//_MSC_VER



uint64_t shSharedSceneRun(
    void*              p_engine,
    ShApplicationFunc* p_func
) {
    shSharedHostError(p_engine == NULL, "shSharedSceneRun: invalid engine memory", return 0);
    if (p_func == NULL) { return 1; }
    return p_func(p_engine);//all application functions return a uint64_t
}

uint8_t shLoadApplication(
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
) {
    shSharedHostError(shared_path       == NULL, "shLoadApplication: invalid shared path memory",       return 0);
    shSharedHostError(s_start           == NULL, "shLoadApplication: invalid s_start memory",           return 0);
    shSharedHostError(s_thread          == NULL, "shLoadApplication: invalid s_thread memory",          return 0);
    shSharedHostError(s_update_pending  == NULL, "shLoadApplication: invalid s_update_pending memory",  return 0);
    shSharedHostError(s_after_thread    == NULL, "shLoadApplication: invalid s_after_thread memory",    return 0);
    shSharedHostError(s_update          == NULL, "shLoadApplication: invalid s_update memory",          return 0);
    shSharedHostError(s_main_cmd_buffer == NULL, "shLoadApplication: invalid s_main_cmd_buffer memory", return 0);
    shSharedHostError(s_main_renderpass == NULL, "shLoadApplication: invalid s_main_renderpass memory", return 0);
    shSharedHostError(s_frame_resize    == NULL, "shLoadApplication: invalid s_frame_resize memory",    return 0);
    shSharedHostError(s_close           == NULL, "shLoadApplication: invalid s_close memory",           return 0);
    shSharedHostError(p_application     == NULL, "shLoadApplication: invalid application memory",       return 0);

    p_application->run    = run;

    char shared_path_ext[64];
#ifndef _MSC_VER
    strcpy(shared_path_ext, "lib");
    strcat(shared_path_ext, shared_path);
#else
    strcpy(shared_path_ext, shared_path);

#endif//_MSC_VER
#ifdef _MSC_VER
    strcat(shared_path_ext, ".dll");
#else
    strcat(shared_path_ext, ".so");
#endif//_MSC_VER

    if (!p_application->run) {
        return 1;
    }

    p_application->shared = shLoadShared(shared_path_ext);

    shSharedHostError(p_application->shared == NULL, "shLoadApplication: invalid shared library", return 0);

    p_application->p_start           = (ShApplicationFunc*)       shSharedLoadSymbol(p_application->shared, s_start);
    p_application->p_thread          = (ShApplicationThreadFunc*) shSharedLoadSymbol(p_application->shared, s_thread);
    p_application->p_update_pending  = (ShApplicationFunc*)       shSharedLoadSymbol(p_application->shared, s_update_pending);
    p_application->p_after_thread    = (ShApplicationFunc*)       shSharedLoadSymbol(p_application->shared, s_after_thread);
    p_application->p_update          = (ShApplicationFunc*)       shSharedLoadSymbol(p_application->shared, s_update);
    p_application->p_main_cmd_buffer = (ShApplicationFunc*)       shSharedLoadSymbol(p_application->shared, s_main_cmd_buffer);
    p_application->p_main_renderpass = (ShApplicationFunc*)       shSharedLoadSymbol(p_application->shared, s_main_renderpass);
    p_application->p_frame_resize    = (ShApplicationFunc*)       shSharedLoadSymbol(p_application->shared, s_frame_resize);
    p_application->p_close           = (ShApplicationFunc*)       shSharedLoadSymbol(p_application->shared, s_close);

    shSharedHostError(p_application->p_start           == NULL, "shLoadApplication: invalid start function memory",           return 0);
    shSharedHostError(p_application->p_thread          == NULL, "shLoadApplication: invalid thread function memory",          return 0);
    shSharedHostError(p_application->p_update_pending  == NULL, "shLoadApplication: invalid update pending function memory",  return 0);
    shSharedHostError(p_application->p_after_thread    == NULL, "shLoadApplication: invalid after thread function memory",    return 0);
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