#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shengine/shEngine.h>
#include <shengine/shExport.h>
#include <shengine/shEngineUI.h>

#include <math.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>

#define CANVAS_VERTEX_COUNT   6
#define NOISE_PARAMETERS_SIZE sizeof(NoiseParameters)             
#define MEMORY_WRITE_COPY_IDX 0


typedef struct NoiseParameters {
    float s;
    float a;
    float b;
} NoiseParameters;


typedef struct NoiseApp {
    NoiseParameters parameters;
    VkFence         copy_fence;
    SmdFileHandle   recovery;
    SmdFileHandle   saved;
    SmdExportHandle export;
    uint8_t         display_ui;
} NoiseApp;



uint8_t SH_ENGINE_EXPORT_FUNCTION noise_start(ShEngine* p_engine) {
    shApplicationError(p_engine == NULL, "noise_start: invalid engine memory", return 0);

    p_engine->p_ext = calloc(1, sizeof(NoiseApp));
    shApplicationError(
        p_engine->p_ext == NULL,
        "noise_start: invalid engine extension memory",
        return 0
    );

    NoiseApp* p_noise = (NoiseApp*)p_engine->p_ext;

    smdReadFile("../../smd/noise-recovery.smd", &p_noise->recovery);
    smdParseMemory(&p_noise->recovery);
    smdAccessVarByName(&p_noise->recovery, "s", NULL, &p_noise->parameters.s);
    smdAccessVarByName(&p_noise->recovery, "a", NULL, &p_noise->parameters.a);
    smdAccessVarByName(&p_noise->recovery, "b", NULL, &p_noise->parameters.b);
    smdFileHandleRelease(&p_noise->recovery);
    

    ShVkPipelinePool* p_pool                = &p_engine->pipeline_pool; 
	ShVkPipeline*     p_pipeline            = &p_pool->pipelines[0];
    uint32_t          swapchain_image_count = p_engine->core.swapchain_image_count;

    VkBuffer          staging_buffer        = p_engine->vulkan_memory_properties.buffers[0];
	VkDeviceMemory    staging_memory        = p_engine->vulkan_memory_properties.buffers_memory[0];
    VkBuffer          dst_buffer            = p_engine->vulkan_memory_properties.buffers[1];

    VkDevice          device                = p_engine->core.device;
    VkCommandBuffer   cmd_buffer            = p_engine->core.graphics_cmd_buffers[0];
	VkQueue           queue                 = p_engine->core.graphics_queue;

    shPipelinePoolSetDescriptorBufferInfos(0, swapchain_image_count, dst_buffer, 0, sizeof(NoiseParameters), p_pool);
    shPipelinePoolCreateDescriptorSetLayoutBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, p_pool);
    shPipelinePoolCreateDescriptorSetLayout(device, 0, 1, 0, p_pool);
    shPipelinePoolCopyDescriptorSetLayout(0, 0, swapchain_image_count, p_pool);

    shPipelinePoolCreateDescriptorPool(device, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, swapchain_image_count, p_pool);
    shPipelinePoolAllocateDescriptorSetUnits(device, 0, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, swapchain_image_count, p_pool);
    shPipelinePoolUpdateDescriptorSetUnits(device, 0, swapchain_image_count, p_pool);

    shPipelineSetVertexInputState (p_pipeline);
	shPipelineCreateInputAssembly (VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, SH_FALSE, p_pipeline);

	shPipelineCreateRasterizer    (VK_POLYGON_MODE_FILL, SH_FALSE, p_pipeline);
	shPipelineSetMultisampleState (p_engine->core.sample_count, 0.0f, p_pipeline);
	shPipelineSetViewport         (0, 0, p_engine->window.width, p_engine->window.height, 
	                               0, 0, p_engine->window.width, p_engine->window.height, p_pipeline);
	shPipelineColorBlendSettings  (SH_FALSE, SH_FALSE, SH_ENGINE_SUBPASS_COLOR_ATTACHMENT_COUNT, p_pipeline);

	uint32_t shader_size = 0;
	char* shader_code = (char*)shReadCode(
		"../../shaders/bin/noise.vert.spv",
		"rb",
		&shader_size
	);
	shApplicationError(shader_code == NULL, "noise_start: invalid shader code memory", return 0);

    shPipelineCreateShaderModule(device, shader_size, shader_code, p_pipeline);
	shPipelineCreateShaderStage(VK_SHADER_STAGE_VERTEX_BIT, p_pipeline);
	free(shader_code);
	
	shader_code = (char*)shReadCode(
		"../../shaders/bin/noise.frag.spv",
		"rb",
		&shader_size
	);
	shApplicationError(shader_code == NULL, "invalid shader code memory", return 0);

    shPipelineCreateShaderModule(device, shader_size, shader_code, p_pipeline);
	shPipelineCreateShaderStage(VK_SHADER_STAGE_FRAGMENT_BIT, p_pipeline);
	free(shader_code);



	shApplicationError(
		shPipelineCreateLayout(device, 0, swapchain_image_count, p_pool, p_pipeline) == 0,
		"noise_start: failed creating pipeline layout",
		return 0
	);
	
	shApplicationError(
		shSetupGraphicsPipeline(device, p_engine->core.renderpass, p_pipeline) == 0,
		"noise_start: failed creating pipeline",
		return 0
	);


	shCreateFences(device, 1, 1, &p_noise->copy_fence);

    shProfilingTimerSetExtCount(&p_engine->profiling_timer, 1);

    shEngineGuiSetup(p_engine);
    p_noise->display_ui = 1;

    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION noise_update(ShEngine* p_engine, const uint32_t entity) {
    shApplicationError(p_engine                  == NULL, "noise_update: invalid engine memory",           return 0);
    shApplicationError(p_engine->p_ext           == NULL, "noise_update: invalid engine extension memory", return 0);

    NoiseApp*         p_noise = (NoiseApp*)p_engine->p_ext;
    float             dtime   = (float)p_engine->time.delta_time;
    float             fps     = 1.0f / (float)p_engine->time.delta_time;

    if (shIsKeyDown(p_engine->window, SH_KEY_LEFT_CONTROL) && shIsKeyPressed(p_engine->window, SH_KEY_E)) {//save interface data
        smdWriteLine(&p_noise->export, 1, "info", SMD_VAR_TYPE_STR1024, "@github.com/mrsinho");
        smdCommentLine(&p_noise->export, "\n");
        smdWriteLine(&p_noise->export, 1, "s",    SMD_VAR_TYPE_FLOAT32, &p_noise->parameters.s);
        smdWriteLine(&p_noise->export, 1, "a",    SMD_VAR_TYPE_FLOAT32, &p_noise->parameters.a);
        smdWriteLine(&p_noise->export, 1, "b",    SMD_VAR_TYPE_FLOAT32, &p_noise->parameters.b);
        smdWriteFile(&p_noise->export, "../../smd/noise-saved.smd");
        smdExportHandleRelease(&p_noise->export);
    }

    if (shIsKeyDown(p_engine->window, SH_KEY_LEFT_CONTROL) && shIsKeyPressed(p_engine->window, SH_KEY_L)) {//load saved data
        smdReadFile("../../smd/noise-saved.smd", &p_noise->saved);
        smdParseMemory(&p_noise->saved);
        smdAccessVarByName(&p_noise->saved, "s", NULL, &p_noise->parameters.s);
        smdAccessVarByName(&p_noise->saved, "a", NULL, &p_noise->parameters.a);
        smdAccessVarByName(&p_noise->saved, "b", NULL, &p_noise->parameters.b);
        smdFileHandleRelease(&p_noise->saved);
    }

         if (shIsKeyDown(p_engine->window, SH_KEY_W)) { p_noise->parameters.s += 1.0f * dtime; }
    else if (shIsKeyDown(p_engine->window, SH_KEY_Q)) { p_noise->parameters.s -= 1.0f * dtime; }

         if (shIsKeyDown(p_engine->window, SH_KEY_S)) { p_noise->parameters.a += 1.0f * dtime; }
    else if (shIsKeyDown(p_engine->window, SH_KEY_A)) { p_noise->parameters.a -= 1.0f * dtime; }

         if (shIsKeyDown(p_engine->window, SH_KEY_X)) { p_noise->parameters.b += 1.0f * dtime; }
    else if (shIsKeyDown(p_engine->window, SH_KEY_Z)) { p_noise->parameters.b -= 1.0f * dtime; }

    VkDevice          device                = p_engine->core.device;
    VkBuffer          staging_buffer        = p_engine->vulkan_memory_properties.buffers[0];
	VkDeviceMemory    staging_memory        = p_engine->vulkan_memory_properties.buffers_memory[0];
    VkBuffer          dst_buffer            = p_engine->vulkan_memory_properties.buffers[1];

    VkCommandBuffer   transfer_cmd_buffer   = p_engine->core.transfer_cmd_buffer;
	VkQueue           transfer_queue        = p_engine->core.transfer_queue;

    shProfilingTimerStartExt(&p_engine->profiling_timer, "MEMORY_WRITE_COPY___ms", MEMORY_WRITE_COPY_IDX);

    shWriteMemory  (device, staging_memory, 0, NOISE_PARAMETERS_SIZE, p_noise);
    shWaitForFences(device, 1, &p_noise->copy_fence, 1, UINT64_MAX);

    shResetFences(device, 1, &p_noise->copy_fence);

    shBeginCommandBuffer(transfer_cmd_buffer);
    shCopyBuffer        (transfer_cmd_buffer, staging_buffer, 0, 0, NOISE_PARAMETERS_SIZE, dst_buffer);
    shEndCommandBuffer  (transfer_cmd_buffer);

    shQueueSubmit(1, &transfer_cmd_buffer, transfer_queue, p_noise->copy_fence, 0, NULL, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, NULL);
    shWaitForFences(device, 1, &p_noise->copy_fence, 1, UINT64_MAX);

    shProfilingTimerEndExt(&p_engine->profiling_timer, MEMORY_WRITE_COPY_IDX);

    if (shIsKeyPressed(p_engine->window, SH_KEY_H)) { p_noise->display_ui = !p_noise->display_ui; }

    if (p_noise->display_ui) {

        ShGui* p_gui = &p_engine->gui;

        shGuiRegion(
            p_gui,
            SH_GUI_VEC2_ZERO,
            (shguivec2){ 200.0f, 105.0f },
            SH_GUI_COLOR_BLACK,
            SH_GUI_COLOR_BLACK,
            SH_GUI_PIXELS | SH_GUI_EDGE_LEFT | SH_GUI_EDGE_TOP
        );

        char s_params[64] = { 0 };
        snprintf(s_params, 64, "fps: %.2f\ns  : %.2f\na  : %.2f\nb  : %.2f", fps, p_noise->parameters.s, p_noise->parameters.a, p_noise->parameters.b);
        shGuiText(
            p_gui,
            (shguivec2) { 20.0f, -20.0f },
            SH_GUI_COLOR_WHITE,
            20.0f,
            s_params,
            SH_GUI_EDGE_LEFT | SH_GUI_EDGE_TOP
        );

        shGuiRegion(
            p_gui,
            SH_GUI_VEC2_ZERO,
            (shguivec2){ 150.0f, 30.0f },
            SH_GUI_COLOR_BLACK,
            SH_GUI_COLOR_BLACK,
            SH_GUI_PIXELS | SH_GUI_CENTER_WIDTH | SH_GUI_EDGE_TOP
        );

        shGuiText(
            p_gui,
            (shguivec2) { 0.0f, -20.0f },
            SH_GUI_COLOR_WHITE,
            10.0f,
            "press H to hide",
            SH_GUI_CENTER_WIDTH | SH_GUI_EDGE_TOP
        );
    }

   

    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION noise_main_cmd_buffer(ShEngine* p_engine) {

	return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION noise_main_renderpass(ShEngine* p_engine) {                              
    shApplicationError(p_engine                  == NULL, "noise_main_renderpass: invalid engine memory",           return 0);
    shApplicationError(p_engine->p_ext           == NULL, "noise_main_renderpass: invalid engine extension memory", return 0);

    uint32_t          swapchain_image_idx = p_engine->core.swapchain_image_idx;
    VkCommandBuffer   cmd_buffer          = p_engine->core.graphics_cmd_buffers[swapchain_image_idx];
    ShVkPipelinePool* p_pool              = &p_engine->pipeline_pool; 
    ShVkPipeline*     p_pipeline          = &p_pool->pipelines[0];
    NoiseApp*         p_noise             = (NoiseApp*)p_engine->p_ext;

    shPipelineBindDescriptorSetUnits(
        cmd_buffer, 
        0, 
        swapchain_image_idx, 
        1, 
        VK_PIPELINE_BIND_POINT_GRAPHICS, 
        0, 
        NULL, 
        p_pool, 
        p_pipeline
    );

	shBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, p_pipeline);

    shDraw(cmd_buffer, CANVAS_VERTEX_COUNT, 0, 1, 0);

    return 1;
}


uint8_t SH_ENGINE_EXPORT_FUNCTION noise_frame_resize(ShEngine* p_engine) {
    shApplicationError(p_engine                  == NULL, "noise_frame_resize: invalid engine memory", return 0);

	ShVkPipeline* p_pipeline = &p_engine->pipeline_pool.pipelines[0];
	VkDevice      device     =  p_engine->core.device;

	shDestroyPipeline(device, p_pipeline->pipeline);
	shPipelineSetViewport(
        0, 0, p_engine->window.width, p_engine->window.height, 
        0, 0, p_engine->window.width, p_engine->window.height, 
        p_pipeline
    );

	shApplicationError(
		shSetupGraphicsPipeline(device, p_engine->core.renderpass, p_pipeline) == 0,
		"noise_frame_resize: failed creating pipeline",
		return 0
	);

    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION noise_close(ShEngine* p_engine) {
    shApplicationError(p_engine                  == NULL, "noise_close: invalid engine memory",           return 0);
    shApplicationError(p_engine->p_ext           == NULL, "noise_close: invalid engine extension memory", return 0);

    
    VkDevice          device      =  p_engine->core.device;
    ShVkPipelinePool* p_pool      = &p_engine->pipeline_pool;
    ShVkPipeline*     p_pipeline  = &p_pool->pipelines[0];
    NoiseApp*         p_noise     = (NoiseApp*)p_engine->p_ext;


    shWaitDeviceIdle(device);
    
    shDestroyFences(device, 1, &p_noise->copy_fence);

    shPipelinePoolDestroyDescriptorPools     (device, 0, 1, p_pool);
    shPipelinePoolDestroyDescriptorSetLayouts(device, 0, 1, p_pool);

	shPipelineDestroyShaderModules(device, 0, 2, p_pipeline);
	shPipelineDestroyLayout       (device, p_pipeline);
	shDestroyPipeline             (device, p_pipeline->pipeline);
	
    
	shClearPipeline(p_pipeline);


    if (p_engine->p_ext != NULL) {
        free(p_engine->p_ext);
    }

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

    p_engine->application_host.p_start           = (ShApplicationFunc*)      &noise_start;
    p_engine->application_host.p_update          = (ShApplicationFunc*)      &noise_update;
    p_engine->application_host.p_main_cmd_buffer = (ShApplicationFunc*)      &noise_main_cmd_buffer;
    p_engine->application_host.p_main_renderpass = (ShApplicationFunc*)      &noise_main_renderpass;
    p_engine->application_host.p_frame_resize    = (ShApplicationFunc*)      &noise_frame_resize;
    p_engine->application_host.p_close           = (ShApplicationFunc*)      &noise_close;
    p_engine->window.title                       = "noise";

    return shEditorMain(p_engine);
}
#endif//SH_APPLICATION_TARGET_TYPE_EXECUTABLE

#ifdef __cplusplus
}
#endif//__cplusplus
