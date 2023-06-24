#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shengine/shEngine.h>
#include <shengine/shExport.h>
#include <shengine/shEngineUI.h>

#include <math.h>
#include <stdio.h>
#include <memory.h>



#define CANVAS_VERTEX_COUNT 6


#define NOISE_PARAMETERS_SET             0
#define NOISE_PARAMETERS_SIZE            sizeof(NoiseParameters)             

#define NOISE_PARAMETERS_SET_LAYOUT      0
#define NOISE_PARAMETERS_BINDING         0
                                         
#define NOISE_PARAMETERS_DESCRIPTOR_POOL 0



typedef struct NoiseParameters {
    float s;
    float a;
    float b;
} NoiseParameters;


typedef struct NoiseApp {
    NoiseParameters parameters;
    VkFence         copy_fence;
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

    p_noise->parameters.s = 0.5f;
    p_noise->parameters.a = 2.0f;
    p_noise->parameters.b = 7.0f;
    p_noise->display_ui   = 1;


    p_engine->p_pipeline_pool = shAllocatePipelinePool();
    shApplicationError(p_engine->p_pipeline_pool == NULL, "invalid pipeline pool memory", return 0);
    
    ShVkPipelinePool* p_pool                = p_engine->p_pipeline_pool; 
	ShVkPipeline*     p_pipeline            = &p_pool->pipelines[0];
    uint32_t          swapchain_image_count = p_engine->core.swapchain_image_count;

    VkBuffer          staging_buffer        = p_engine->vulkan_memory_properties.buffers[0];
	VkDeviceMemory    staging_memory        = p_engine->vulkan_memory_properties.buffers_memory[0];
    VkBuffer          dst_buffer            = p_engine->vulkan_memory_properties.buffers[1];

    VkDevice          device                = p_engine->core.device;
    VkCommandBuffer   cmd_buffer            = p_engine->core.graphics_cmd_buffers[0];
	VkQueue           queue                 = p_engine->core.graphics_queue;

    shPipelinePoolSetDescriptorSetBufferInfos(NOISE_PARAMETERS_SET, swapchain_image_count, dst_buffer, 0, sizeof(NoiseParameters), p_pool);
    shPipelinePoolCreateDescriptorSetLayoutBinding(NOISE_PARAMETERS_BINDING, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, p_pool);
    shPipelinePoolCreateDescriptorSetLayout(device, NOISE_PARAMETERS_BINDING, 1, NOISE_PARAMETERS_SET_LAYOUT, p_pool);
    shPipelinePoolCopyDescriptorSetLayout(NOISE_PARAMETERS_SET_LAYOUT, NOISE_PARAMETERS_SET_LAYOUT, swapchain_image_count, p_pool);

    shPipelinePoolCreateDescriptorPool(device, NOISE_PARAMETERS_DESCRIPTOR_POOL, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, swapchain_image_count, p_pool);
    shPipelinePoolAllocateDescriptorSets(device, NOISE_PARAMETERS_BINDING, NOISE_PARAMETERS_DESCRIPTOR_POOL, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, NOISE_PARAMETERS_SET, swapchain_image_count, p_pool);
    shPipelinePoolUpdateDescriptorSets(device, NOISE_PARAMETERS_SET, swapchain_image_count, p_pool);

    shPipelineSetVertexInputState (p_pipeline);
	shPipelineCreateInputAssembly (VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, p_pipeline);

	shPipelineCreateRasterizer    (VK_POLYGON_MODE_FILL, 0, p_pipeline);
	shPipelineSetMultisampleState (p_engine->core.sample_count, 0.0f, p_pipeline);
	shPipelineSetViewport         (0, 0, p_engine->window.width, p_engine->window.height, 
	                               0, 0, p_engine->window.width, p_engine->window.height, p_pipeline);
	shPipelineColorBlendSettings  (p_pipeline);

	uint32_t shader_size = 0;
	char* shader_code = (char*)shReadCode(
		"../../shaders/bin/noise.vert.spv",
		"rb",
		&shader_size
	);
	shApplicationError(shader_code == NULL, "noise_start: invalid shader code memory", return 0);

	shApplicationError(
		shPipelineCreateShaderModule(device, shader_size, shader_code, p_pipeline) == 0,
		"noise_start: failed allocating shader module",
		return 0
	);
	shPipelineCreateShaderStage(VK_SHADER_STAGE_VERTEX_BIT, p_pipeline);
	free(shader_code);
	
	shader_code = (char*)shReadCode(
		"../../shaders/bin/noise.frag.spv",
		"rb",
		&shader_size
	);
	shApplicationError(shader_code == NULL, "invalid shader code memory", return 0);

	shApplicationError(
		shPipelineCreateShaderModule(device, shader_size, shader_code, p_pipeline) == 0,
		"noise_start: failed allocating shader module",
		return 0
	);
	shPipelineCreateShaderStage(VK_SHADER_STAGE_FRAGMENT_BIT, p_pipeline);
	free(shader_code);



	shApplicationError(
		shPipelineCreateLayout(device, NOISE_PARAMETERS_SET, swapchain_image_count, p_engine->p_pipeline_pool, p_pipeline) == 0,
		"noise_start: failed creating pipeline layout",
		return 0
	);
	
	shApplicationError(
		shSetupGraphicsPipeline(device, p_engine->core.renderpass, p_pipeline) == 0,
		"noise_start: failed creating pipeline",
		return 0
	);


	shCreateFences(device, 1, 1, &p_noise->copy_fence);

    return 1;
}

uint64_t SH_ENGINE_EXPORT_FUNCTION noise_thread(NoiseApp* p_noise) {
    
    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION noise_update_pending(ShEngine* p_engine) {

    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION noise_after_thread(ShEngine* p_engine) {
    
    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION noise_update(ShEngine* p_engine, const uint32_t entity) {
    shApplicationError(p_engine        == NULL, "noise_start: invalid engine memory",           return 0);
    shApplicationError(p_engine->p_ext == NULL, "noise_start: invalid engine extension memory", return 0);
    shApplicationError(p_engine->p_pipeline_pool == NULL, "noise_main_renderpass: invalid pipeline pool memory", return 0);

    ShGui*            p_gui                 = p_engine->p_gui;
    NoiseApp*         p_noise               = (NoiseApp*)p_engine->p_ext;
    float             dtime                 = (float)p_engine->time.delta_time;

    ShVkPipelinePool* p_pool                = p_engine->p_pipeline_pool; 
	ShVkPipeline*     p_pipeline            = &p_pool->pipelines[0];
    uint32_t          swapchain_image_idx   = p_engine->core.swapchain_image_idx;

    VkBuffer          staging_buffer        = p_engine->vulkan_memory_properties.buffers[0];
	VkDeviceMemory    staging_memory        = p_engine->vulkan_memory_properties.buffers_memory[0];
    VkBuffer          dst_buffer            = p_engine->vulkan_memory_properties.buffers[1];

    VkDevice          device                = p_engine->core.device;
    VkCommandBuffer   cmd_buffer            = p_engine->core.graphics_cmd_buffers[swapchain_image_idx];
	VkQueue           queue                 = p_engine->core.graphics_queue;

    if (shIsKeyPressed(p_engine->window, SH_KEY_H)) { p_noise->display_ui = !p_noise->display_ui; }

    if (p_noise->display_ui) {
       //roba
    }

         if (shIsKeyDown(p_engine->window, SH_KEY_W)) { p_noise->parameters.s += 1.0f * dtime; }
    else if (shIsKeyDown(p_engine->window, SH_KEY_Q)) { p_noise->parameters.s -= 1.0f * dtime; }

         if (shIsKeyDown(p_engine->window, SH_KEY_S)) { p_noise->parameters.a += 1.0f * dtime; }
    else if (shIsKeyDown(p_engine->window, SH_KEY_A)) { p_noise->parameters.a -= 1.0f * dtime; }

         if (shIsKeyDown(p_engine->window, SH_KEY_X)) { p_noise->parameters.b += 1.0f * dtime; }
    else if (shIsKeyDown(p_engine->window, SH_KEY_Z)) { p_noise->parameters.b -= 1.0f * dtime; }


    shWriteMemory  (device, staging_memory, 0, NOISE_PARAMETERS_SIZE, p_noise);
    shWaitForFences(device, 1, &p_noise->copy_fence, 1, UINT64_MAX);
	shResetFences  (device, 1, &p_noise->copy_fence);
    
	shBeginCommandBuffer(cmd_buffer);
	shCopyBuffer(cmd_buffer, staging_buffer, 0, 0, NOISE_PARAMETERS_SIZE, dst_buffer);
    shEndCommandBuffer(cmd_buffer);

	shQueueSubmit(1, &cmd_buffer, queue, p_noise->copy_fence, 0, NULL, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, NULL);
	shWaitForFences(device, 1, &p_noise->copy_fence, 1, UINT64_MAX);

    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION noise_main_cmd_buffer(ShEngine* p_engine) {
    
	return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION noise_main_renderpass(ShEngine* p_engine) {                              
    shApplicationError(p_engine                  == NULL, "noise_main_renderpass: invalid engine memory",           return 0);
    shApplicationError(p_engine->p_ext           == NULL, "noise_main_renderpass: invalid engine extension memory", return 0);
    shApplicationError(p_engine->p_pipeline_pool == NULL, "noise_main_renderpass: invalid pipeline pool memory",    return 0);

    uint32_t          swapchain_image_idx = p_engine->core.swapchain_image_idx;
    VkCommandBuffer   cmd_buffer          =  p_engine->core.graphics_cmd_buffers[swapchain_image_idx];
    ShVkPipelinePool* p_pool              = p_engine->p_pipeline_pool; 
    ShVkPipeline*     p_pipeline          = &p_pool->pipelines[0];
    NoiseApp*         p_noise             = (NoiseApp*)p_engine->p_ext;

    shPipelineBindDescriptorSets(cmd_buffer, swapchain_image_idx, 1, VK_PIPELINE_BIND_POINT_GRAPHICS, 0, NULL, p_pool, p_pipeline);

	shBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, p_pipeline);

    shDraw(cmd_buffer, CANVAS_VERTEX_COUNT, 0, 1, 0);

    return 1;
}


uint8_t SH_ENGINE_EXPORT_FUNCTION noise_frame_resize(ShEngine* p_engine) {
    shApplicationError(p_engine                  == NULL, "noise_frame_resize: invalid engine memory", return 0);
    shApplicationError(p_engine->p_pipeline_pool == NULL, "noise_frame_resize: invalid pipeline pool memory", return 0);

	ShVkPipeline* p_pipeline = &p_engine->p_pipeline_pool->pipelines[0];
	VkDevice      device     =  p_engine->core.device;

	shDestroyPipeline(device, p_pipeline->pipeline);
	shPipelineSetViewport(0, 0, p_engine->window.width, p_engine->window.height, 
                          0, 0, p_engine->window.width, p_engine->window.height, p_pipeline);

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
    shApplicationError(p_engine->p_pipeline_pool == NULL, "noise_close: invalid pipeline pool memory", return 0);

    
    VkDevice          device      =  p_engine->core.device;
    ShVkPipelinePool* p_pool      =  p_engine->p_pipeline_pool;
    ShVkPipeline*     p_pipeline  = &p_pool->pipelines[0];
    NoiseApp*         p_noise     = (NoiseApp*)p_engine->p_ext;


    shWaitDeviceIdle(device);
    
    shDestroyFences(device, 1, &p_noise->copy_fence);

    shPipelinePoolDestroyDescriptorPools     (device, NOISE_PARAMETERS_DESCRIPTOR_POOL, 1, p_pool);
    shPipelinePoolDestroyDescriptorSetLayouts(device, NOISE_PARAMETERS_SET_LAYOUT, 1, p_pool);

	shPipelineDestroyShaderModules(device, 0, 2, p_pipeline);
	shPipelineDestroyLayout       (device, p_pipeline);
	shDestroyPipeline             (device, p_pipeline->pipeline);
	
    
	shClearPipeline(p_pipeline);

    shFreePipelinePool(p_pool);


    if (p_engine->p_ext != NULL) {
        free(p_engine->p_ext);
    }
    return 1;
}



#ifdef SH_APPLICATION_TARGET_TYPE_EXECUTABLE

#include <sheditor/shEditor.h>

int main() {
    ShEngine engine = { 0 };
    engine.application_host.p_start           = &noise_start;
    engine.application_host.p_thread          = &noise_thread;
    engine.application_host.p_update_pending  = &noise_update_pending;
    engine.application_host.p_after_thread    = &noise_after_thread;
    engine.application_host.p_update          = &noise_update;
    engine.application_host.p_main_cmd_buffer = &noise_main_cmd_buffer;
    engine.application_host.p_main_renderpass = &noise_main_renderpass;
    engine.application_host.p_frame_resize    = &noise_frame_resize;
    engine.application_host.p_close           = &noise_close;
    engine.window.title                       = "NoiseApp";
    return shEditorMain(&engine);
}
#endif//SH_APPLICATION_TARGET_TYPE_EXECUTABLE

#ifdef __cplusplus
}
#endif//__cplusplus
