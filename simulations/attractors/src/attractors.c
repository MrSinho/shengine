#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shvulkan/shVkDrawLoop.h>

#include <shengine/shEngine.h>
#include <shengine/shExport.h>

#include <shegui/shEgui.h>

#include <shecs/shCamera.h>

#include <stdio.h>
#include <memory.h>



typedef struct Attractors {
    float* p_positions;
    uint32_t vertex_count;//each vertex is 12 bytes

    VkBuffer       vertex_staging_buffer;
    VkDeviceMemory vertex_staging_memory;
    VkBuffer       vertex_buffer;
    VkDeviceMemory vertex_memory;

} Attractors;



uint8_t SH_ENGINE_EXPORT_FUNCTION attractors_start(ShEngine* p_engine) {
    p_engine->p_ext = calloc(1, sizeof(Attractors));

    VkDevice device = p_engine->core.device;
    VkRenderPass render_pass = p_engine->core.render_pass;

    ShMaterialHost* p_material = &p_engine->p_materials[0];

    return shEngineGuiSetup(p_engine, 256, SH_GUI_THEME_EXTRA_DARK);
}

uint64_t SH_ENGINE_EXPORT_FUNCTION attractors_thread(Attractors* p_attractors) {//void* ShEngine::p_engine_extension = NULL
    shEngineError(p_attractors == NULL, "invalid attractors memory", return 0);

    p_attractors->vertex_count = (uint32_t)3E3;
    p_attractors->p_positions = calloc(p_attractors->vertex_count, 4);
    shEngineError(p_attractors->p_positions == NULL, "invalid attractors vertex positions memory", return 0);

    for (uint32_t vertex_idx = 3; vertex_idx < p_attractors->vertex_count; vertex_idx+=3) {
        float* pos = &p_attractors->p_positions[vertex_idx];
        if (vertex_idx % 2 == 0) {
            memcpy(pos, &p_attractors->p_positions[vertex_idx - 3], 12);
        }
        else {
            float f = (float)(vertex_idx / 6) / 1000.0f;
            pos[0] = f;
            pos[1] =-f * f;
            pos[2] = 1.0f;
            printf("%f\n", f);
        }
    }

    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION attractors_update_pending(ShEngine* p_engine) {

    return 1;
}


uint8_t SH_ENGINE_EXPORT_FUNCTION attractors_after_thread(ShEngine* p_engine) {

    VkDevice device = p_engine->core.device;
    VkPhysicalDevice physical_device = p_engine->core.physical_device;
    VkCommandBuffer cmd_buffer = p_engine->core.p_graphics_commands[0].cmd_buffer;

    Attractors* p_attractors = p_engine->p_ext;

    {//STAGING BUFFER
        shCreateBuffer(
            device,
            p_attractors->vertex_count * 4,
            VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            &p_attractors->vertex_staging_buffer
        );

        shAllocateMemory(
            device,
            physical_device,
            p_attractors->vertex_staging_buffer,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            &p_attractors->vertex_staging_memory
        );

        shBindMemory(
            device,
            p_attractors->vertex_staging_buffer,
            0,
            p_attractors->vertex_staging_memory
        );
    }//STAGING BUFFER

    {//VERTEX BUFFER
        shCreateBuffer(
            device,
            p_attractors->vertex_count * 4,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            &p_attractors->vertex_buffer
        );

        shAllocateMemory(
            device,
            physical_device,
            p_attractors->vertex_buffer,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            &p_attractors->vertex_memory
        );

        shBindMemory(
            device,
            p_attractors->vertex_buffer,
            0,
            p_attractors->vertex_memory
        );
    }//VERTEX BUFFER
    
    {//WRITE MEMORY
        shWriteMemory(
            device,
            p_attractors->vertex_staging_memory,
            0,
            p_attractors->vertex_count * 4,
            p_attractors->p_positions
        );
    }//WRITE MEMORY

    {//COPY BUFFERS
        VkFence* p_fence = &p_engine->core.p_graphics_commands[0].fence;
        //record graphics cmd buffer
        shWaitForFence(device, p_fence);
        shResetFence(device, p_fence);

        shBeginCommandBuffer(cmd_buffer);

        shCopyBuffer(
            cmd_buffer,
            p_attractors->vertex_staging_buffer,
            0,
            0,
            p_attractors->vertex_count * 4,
            p_attractors->vertex_buffer
        );

        shEndCommandBuffer(cmd_buffer);
        
        shQueueSubmit(
            1, 
            &p_engine->core.p_graphics_commands[0].cmd_buffer, 
            p_engine->core.graphics_queue.queue, 
            *p_fence
        );

        shWaitForFence(device, p_fence);

    }//COPY BUFFERS

    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION attractors_update(ShEngine* p_engine) {
    //before recording cmd buffer
    ShGui* p_gui = p_engine->p_gui;

    if (p_gui == NULL) { return 0; }
    shGuiWindow(
        p_gui,
        250.0f,
        200.0f,
        -500.0f,
        200.0f,
        "Attractors demo",
        SH_GUI_MOVABLE | SH_GUI_RESIZABLE | SH_GUI_PIXELS
    );
    shGuiWindowText(
        p_gui,
        SH_GUI_WINDOW_TEXT_SIZE,
        "Hey, this is a demo!",
        SH_GUI_CENTER_WIDTH
    );
    shGuiWindowSeparator(p_gui);
    if (shGuiWindowButton(p_gui, SH_GUI_WINDOW_TEXT_SIZE, "Reset", SH_GUI_CENTER_WIDTH)) {
        puts("Reset");
        shResetEngineState(p_engine, 0);
        return 1;
    }
    shGuiWindowSeparator(p_gui);
    if (shGuiWindowButton(p_gui, SH_GUI_WINDOW_TEXT_SIZE, "Quit", SH_GUI_CENTER_WIDTH)) {
        shEngineShutdown(p_engine);
    }
	return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION attractors_frame_update(ShEngine* p_engine) {

    VkDevice device = p_engine->core.device;
    VkCommandBuffer cmd_buffer = p_engine->core.p_graphics_commands[0].cmd_buffer;
    ShVkPipeline* p_pipeline = &p_engine->p_materials[0].pipeline;

    Attractors* p_attractors = (Attractors*)p_engine->p_ext;

    if (p_pipeline->pipeline == NULL) {
        return 1;
    }

    shBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, p_pipeline);

    {//PROJECTION AND VIEW MATRICES
        ShCamera* p_camera = shGetShCamera(&p_engine->scene, 0);
        shEngineError(p_camera == NULL, "invalid camera memory", return 0);

        shPipelinePushConstants(cmd_buffer, p_camera->projection, p_pipeline);
    }//PROJECTION AND VIEW MATRICES

    shBindVertexBuffer(
        cmd_buffer,
        0,
        0,
        &p_attractors->vertex_buffer
    );

    shDraw(
        cmd_buffer,
        p_attractors->vertex_count
    );

    shEndPipeline(p_pipeline);

    return 1;
}


uint8_t SH_ENGINE_EXPORT_FUNCTION attractors_frame_resize(ShEngine* p_engine) {
    
    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION attractors_close(ShEngine* p_engine, const uint32_t entity) {
    Attractors* p_attractors = p_engine->p_ext;
    shEngineError(p_attractors == NULL, "invalid attractors memory", return 0);
    shEngineError(p_attractors->p_positions == NULL, "invalid attractors vertex positions memory", return 0);

    VkDevice device = p_engine->core.device;

    shClearBufferMemory(
        device,
        p_attractors->vertex_staging_buffer,
        p_attractors->vertex_staging_memory
    );

    shClearBufferMemory(
        device,
        p_attractors->vertex_buffer,
        p_attractors->vertex_memory
    );

    free(p_attractors->p_positions);
    free(p_attractors);

    return 1;
}

#ifdef __cplusplus
}
#endif//__cplusplus
