#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shengine/shEngine.h>
#include <shengine/shExport.h>



uint8_t SH_ENGINE_EXPORT_FUNCTION triangle_start(ShEngine* p_engine) {

    return 1;
}

uint64_t SH_ENGINE_EXPORT_FUNCTION triangle_thread(void* p_ext) {

    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION triangle_update_pending(ShEngine* p_engine) {

    return 1;
}


uint8_t SH_ENGINE_EXPORT_FUNCTION triangle_after_thread(ShEngine* p_engine) {
    
    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION triangle_update(ShEngine* p_engine) {
   
	return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION triangle_main_renderpass(ShEngine* p_engine) {

    return 1;
}


uint8_t SH_ENGINE_EXPORT_FUNCTION triangle_frame_resize(ShEngine* p_engine) {
    
    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION triangle_close(ShEngine* p_engine) {

    return 1;
}



#ifdef SH_APPLICATION_TARGET_TYPE_EXECUTABLE

#include <sheditor/shEditor.h>

int main() {
    ShEngine* p_engine = shAllocateEngine();
    shEngineError(
        p_engine == NULL,
        "main: invalid engine memory",
        return -1
    );
    p_engine->application_host.p_start           = &triangle_start;
    p_engine->application_host.p_thread          = &triangle_thread;
    p_engine->application_host.p_update_pending  = &triangle_update_pending;
    p_engine->application_host.p_after_thread    = &triangle_after_thread;
    p_engine->application_host.p_update          = &triangle_update;
    p_engine->application_host.p_main_renderpass = &triangle_main_renderpass;
    p_engine->application_host.p_frame_resize    = &triangle_frame_resize;
    p_engine->application_host.p_close           = &triangle_close;
    p_engine->window.title                       = "triangle";
    shEditorMain(p_engine);
    return 0;
}
#endif//SH_APPLICATION_TARGET_TYPE_EXECUTABLE

#ifdef __cplusplus
}
#endif//__cplusplus
