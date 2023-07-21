# Template code with comments

## See the [`project guide`](./project-guide.md) first

```c
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shengine/shEngine.h>//most of engine declarations are here
#include <shengine/shExport.h>//required when building a shared library



uint8_t SH_ENGINE_EXPORT_FUNCTION triangle_start(ShEngine* p_engine) {
    // First function to be called chronologically.
    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION triangle_update(ShEngine* p_engine) {
    // This function is called once per frame.
	return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION triangle_main_cmd_buffer(ShEngine* p_engine) {
    // This function is called once per frame while the main engine command buffer is recording
	return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION triangle_main_renderpass(ShEngine* p_engine) {
    // This function is called once per frame inside the main engine renderpass (you can setup your own of course using direcly the Vulkan API or the shvulkan library, by running on the triangle_update function).
    return 1;
}


uint8_t SH_ENGINE_EXPORT_FUNCTION triangle_frame_resize(ShEngine* p_engine) {
    // When the window is resized, this function is called. It's useful to destroy pipelines and reload them with a new viewport.
    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION triangle_close(ShEngine* p_engine) {
    // When the engine is released/restarted, this function is called.
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

    p_engine->application_host.p_start           = (ShApplicationFunc*) &triangle_start;
    p_engine->application_host.p_update          = (ShApplicationFunc*) &triangle_update;
    p_engine->application_host.p_main_cmd_buffer = (ShApplicationFunc*) &triangle_main_cmd_buffer;
    p_engine->application_host.p_main_renderpass = (ShApplicationFunc*) &triangle_main_renderpass;
    p_engine->application_host.p_frame_resize    = (ShApplicationFunc*) &triangle_frame_resize;
    p_engine->application_host.p_close           = (ShApplicationFunc*) &triangle_close;
    p_engine->window.title                       = "triangle";
    
    shEditorMain(p_engine);
    return 0;
}
#endif//SH_APPLICATION_TARGET_TYPE_EXECUTABLE

#ifdef __cplusplus
}
#endif//__cplusplus
```