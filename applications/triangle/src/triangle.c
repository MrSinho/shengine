#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shengine/shEngine.h>
#include <shengine/shExport.h>

#include <shengine/shEngine.h>



#define VERTEX_LOCATION       0
#define PER_VERTEX_BINDING    0
#define TRIANGLE_VERTEX_COUNT 3
#define VERTEX_STRIDE         (2 * sizeof(float))
#define TRIANGLE_SIZE         (VERTEX_STRIDE * TRIANGLE_VERTEX_COUNT)



uint8_t SH_ENGINE_EXPORT_FUNCTION triangle_start(
	ShEngine* p_engine
) {
	shApplicationError(p_engine == NULL, "triangle_start: invalid engine memory", return 0);

	VkDevice device = p_engine->core.device;

    p_engine->p_pipeline_pool = shAllocatePipelinePool();
	shApplicationError(
		p_engine->p_pipeline_pool == NULL, 
		"triangle_start: invalid pipeline pool memory", 
		return 0
	);

	ShVkPipelinePool* p_pool     = p_engine->p_pipeline_pool; 
	ShVkPipeline*     p_pipeline = &p_pool->pipelines[0];

	shPipelineSetVertexBinding    (PER_VERTEX_BINDING, VERTEX_STRIDE, VK_VERTEX_INPUT_RATE_VERTEX, p_pipeline);
	shPipelineSetVertexAttribute  (VERTEX_LOCATION, PER_VERTEX_BINDING, VK_FORMAT_R32G32_SFLOAT, 0, p_pipeline);
	shPipelineSetVertexInputState (p_pipeline);
	shPipelineCreateInputAssembly (VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, SH_FALSE, p_pipeline);
								  
	shPipelineCreateRasterizer    (VK_POLYGON_MODE_FILL, SH_FALSE, p_pipeline);
	shPipelineSetMultisampleState (p_engine->core.sample_count, 0.0f, p_pipeline);
	shPipelineSetViewport         (0, 0, p_engine->window.width, p_engine->window.height, 
	                               0, 0, p_engine->window.width, p_engine->window.height, p_pipeline);
	shPipelineColorBlendSettings  (SH_FALSE, SH_FALSE, SH_ENGINE_SUBASS_COLOR_ATTACHMENT_COUNT, p_pipeline);

	uint32_t shader_size = 0;
	char* shader_code = (char*)shReadCode(
		"../../shaders/bin/mesh.vert.spv",
		"rb",
		&shader_size
	);
	shApplicationError(shader_code == NULL, "triangle_start: invalid shader code memory", return 0);

	shPipelineCreateShaderModule(device, shader_size, shader_code, p_pipeline);
	shPipelineCreateShaderStage(VK_SHADER_STAGE_VERTEX_BIT, p_pipeline);
	free(shader_code);
	
	shader_code = (char*)shReadCode(
		"../../shaders/bin/mesh.frag.spv",
		"rb",
		&shader_size
	);
	shPipelineCreateShaderModule(device, shader_size, shader_code, p_pipeline);
	shPipelineCreateShaderStage(VK_SHADER_STAGE_FRAGMENT_BIT, p_pipeline);
	free(shader_code);

	shApplicationError(
		shPipelineCreateLayout(device, 0, 0, p_engine->p_pipeline_pool, p_pipeline) == 0,
		"triangle_start: failed creating pipeline layout",
		return 0
	);
	
	shApplicationError(
		shSetupGraphicsPipeline(device, p_engine->core.renderpass, p_pipeline) == 0,
		"triangle_start: failed creating pipeline",
		return 0
	);



	void*          p_triangle     = p_engine->host_memory_properties.p_buffers_memory[0];
	
	VkBuffer       staging_buffer = p_engine->vulkan_memory_properties.buffers[0];//see vulkan-memory.smd
	VkDeviceMemory staging_memory = p_engine->vulkan_memory_properties.buffers_memory[0];
	
	VkBuffer       dst_buffer     = p_engine->vulkan_memory_properties.buffers[1];

	shWriteMemory(device, staging_memory, 0, TRIANGLE_SIZE, p_triangle);



	VkCommandBuffer cmd_buffer = p_engine->core.graphics_cmd_buffers[0];
	VkFence         copy_fence = VK_NULL_HANDLE;
	VkQueue         queue      = p_engine->core.graphics_queue;

	shCreateFences (device, 1, 1, &copy_fence);
	shWaitForFences(device, 1, &copy_fence, 1, UINT64_MAX);
	shResetFences  (device, 1, &copy_fence);

	shBeginCommandBuffer(cmd_buffer);

	shCopyBuffer(cmd_buffer, staging_buffer, 0, 0, TRIANGLE_SIZE, dst_buffer);

	shEndCommandBuffer(cmd_buffer);
	
	shQueueSubmit(1, &cmd_buffer, queue, copy_fence, 0, NULL, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, NULL);
	shWaitForFences(device, 1, &copy_fence, 1, UINT64_MAX);

	shDestroyFences(device, 1, &copy_fence);

    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION triangle_update(ShEngine* p_engine) {

	return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION triangle_main_cmd_buffer(ShEngine* p_engine) {

	return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION triangle_main_renderpass(ShEngine* p_engine) {
	shApplicationError(p_engine                  == NULL, "triangle_main_renderpass: invalid engine memory", return 0);
	shApplicationError(p_engine->p_pipeline_pool == NULL, "triangle_main_renderpass: invalid pipeline pool memory", return 0);

	VkCommandBuffer cmd_buffer   =  p_engine->core.graphics_cmd_buffers[p_engine->core.swapchain_image_idx];
	ShVkPipeline*   p_pipeline   = &p_engine->p_pipeline_pool->pipelines[0];

	VkBuffer       vertex_buffer = p_engine->vulkan_memory_properties.buffers[1];
	VkDeviceSize   vertex_offset = 0;

	shBindVertexBuffers(cmd_buffer, PER_VERTEX_BINDING, 1, &vertex_buffer, &vertex_offset);

	shBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, p_pipeline);

	shDraw(cmd_buffer, TRIANGLE_VERTEX_COUNT, 0, 1, 0);

    return 1;
}


uint8_t SH_ENGINE_EXPORT_FUNCTION triangle_frame_resize(ShEngine* p_engine) {
	shApplicationError(p_engine->p_pipeline_pool == NULL, "triangle_frame_resize: invalid pipeline pool memory", return 0);

	ShVkPipeline* p_pipeline = &p_engine->p_pipeline_pool->pipelines[0];
	VkDevice      device     =  p_engine->core.device;

	shDestroyPipeline(device, p_pipeline->pipeline);
	shPipelineSetViewport(0, 0, p_engine->window.width, p_engine->window.height, 
                          0, 0, p_engine->window.width, p_engine->window.height, p_pipeline);

	shApplicationError(
		shSetupGraphicsPipeline(device, p_engine->core.renderpass, p_pipeline) == 0,
		"triangle_frame_resize: failed creating pipeline",
		return 0
	);

    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION triangle_close(ShEngine* p_engine) {
	shApplicationError(p_engine == NULL, "triangle_close: invalid engine memory", return 0);

	VkDevice          device          = p_engine->core.device;
					   			      
	VkBuffer          staging_buffer  = p_engine->vulkan_memory_properties.buffers[0];//see vulkan-memory.smd
	VkBuffer          dst_buffer      = p_engine->vulkan_memory_properties.buffers[1];
								      
	VkDeviceMemory    staging_memory  = p_engine->vulkan_memory_properties.buffers_memory[0];
	VkDeviceMemory    dst_memory      = p_engine->vulkan_memory_properties.buffers_memory[1];

	ShVkPipelinePool* p_pipeline_pool =  p_engine->p_pipeline_pool;
	ShVkPipeline*     p_pipeline      = &p_engine->p_pipeline_pool->pipelines[0];

	shEngineError(p_pipeline == NULL, "triangle_close: invalid pipeline memory", return 0);

	shWaitDeviceIdle(device);

	shPipelineDestroyShaderModules(device, 0, 2, p_pipeline);
	shPipelineDestroyLayout       (device, p_pipeline);
	shDestroyPipeline             (device, p_pipeline->pipeline);
	
	shClearPipeline(p_pipeline);

	shFreePipelinePool(p_pipeline_pool);

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

	p_engine->application_host.p_start           = (ShApplicationFunc*)      &triangle_start;
    p_engine->application_host.p_update          = (ShApplicationFunc*)      &triangle_update;
    p_engine->application_host.p_main_cmd_buffer = (ShApplicationFunc*)      &triangle_main_cmd_buffer;
    p_engine->application_host.p_main_renderpass = (ShApplicationFunc*)      &triangle_main_renderpass;
    p_engine->application_host.p_frame_resize    = (ShApplicationFunc*)      &triangle_frame_resize;
    p_engine->application_host.p_close           = (ShApplicationFunc*)      &triangle_close;
    p_engine->window.title                       = "triangle";
    
    shEditorMain(p_engine);
    return 0;
}
#endif//SH_APPLICATION_TARGET_TYPE_EXECUTABLE

#ifdef __cplusplus
}
#endif//__cplusplus
