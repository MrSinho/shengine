#include "shDrawLoop.h"

#include "shVkCore.h"
#include "shVkPipelineData.h"
#include "shVkMemoryInfo.h"
#include "shMeshInfo.h"

void shFrameReset(const ShVkCore core) {
	vkWaitForFences(core.device, 1, &core.render_fence, 1, 1000000000);
	vkResetFences(core.device, 1, &core.render_fence);

	vkResetCommandBuffer(core.p_cmd_buffers[0], 0);
}

void shFrameBegin(const ShVkCore core, uint32_t* pSwapchainImageIndex) {

	vkAcquireNextImageKHR(core.device, core.swapchain, 1000000000, core.present_semaphore, 0, pSwapchainImageIndex);

	VkCommandBufferBeginInfo cmdBufferBeginInfo = {
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,	//sType;
		NULL,											//pNext;
		VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,	//flags; //submit cmd buffer once, before the next reset
		NULL											//pInheritanceInfo;
	};

	VkClearValue clear_values[2];
	clear_values[0].color = (VkClearColorValue){ 0.1f, 0.1f, 0.1f };
	clear_values[1].depthStencil = (VkClearDepthStencilValue){ 1.0f, 0 };
	VkRenderPassBeginInfo renderPassBeginInfo = {
		VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,		//sType;
		NULL,											//pNext;
		core.render_pass,								//renderPass;
		core.p_frame_buffers[*pSwapchainImageIndex],		//framebuffer;
		{												//
			{0, 0},										//
			{core.window.width, core.window.height}	//
		},												//renderArea;
		1,												//clearValueCount;
		clear_values									//pClearValues;
	};
	if (core.depth_image_view != VK_NULL_HANDLE) {
		renderPassBeginInfo.clearValueCount = 2;
	}

	vkBeginCommandBuffer(core.p_cmd_buffers[0], &cmdBufferBeginInfo);

	vkCmdBeginRenderPass(core.p_cmd_buffers[0], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void shBindPipeline(const VkCommandBuffer graphicsCmdBuffer, const ShVkPipelineData pipeData) {

	vkCmdBindPipeline(graphicsCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeData.pipeline);
}

void shBindVertexBuffers(const ShVkCore core, const ShMesh mesh) {
	const VkDeviceSize offset = 0;
	vkCmdBindVertexBuffers(core.p_cmd_buffers[0], 0, 1, &mesh.vertex_buffer, &offset);
}

void shBindIndexBuffers(const ShVkCore core, const ShMesh mesh) {
	vkCmdBindIndexBuffer(core.p_cmd_buffers[0], mesh.index_buffer, 0, VK_INDEX_TYPE_UINT32);
}

void shPushConstants(const VkCommandBuffer graphicsCmdBuffer, const ShVkPipelineData pipeData, const void* pPushConstantsData) {
	if (pipeData.pushConstantRange.size > 0 || pPushConstantsData != NULL) {
		vkCmdPushConstants(graphicsCmdBuffer, pipeData.mainPipelineLayout, pipeData.pushConstantRange.stageFlags, pipeData.pushConstantRange.offset, pipeData.pushConstantRange.size, pPushConstantsData);
	}
}

void shBindDescriptorSets(const ShVkCore core, ShVkPipelineData pipeData) {
	vkUpdateDescriptorSets(core.device, 
		pipeData.uniform_buffer_count, pipeData.p_write_descriptor_sets, 
		0, NULL);
	vkCmdBindDescriptorSets(core.p_cmd_buffers[0], VK_PIPELINE_BIND_POINT_GRAPHICS,
		pipeData.mainPipelineLayout, 
		0, pipeData.uniform_buffer_count, pipeData.p_descriptor_sets, 
		0, NULL);
}

void shDraw(const VkCommandBuffer graphicsCmdBuffer, const uint32_t count, const uint32_t stride, const ShMesh mesh) {

	if (mesh.index_buffer_memory == NULL) {
		vkCmdDraw(graphicsCmdBuffer, count / stride, 1, 0, 0);
	}
	else {
		vkCmdDrawIndexed(graphicsCmdBuffer, count, 1, 0, 0, 0);
	}
}

void shFrameEnd(const ShVkCore core, const uint32_t swapchainImageIndex) {

	vkCmdEndRenderPass(core.p_cmd_buffers[0]);
	vkEndCommandBuffer(core.p_cmd_buffers[0]);

	VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	//submit to GPU queue
	// wait for the presentsemaphore, signal to the render semaphore
	VkSubmitInfo submitInfo = {
		VK_STRUCTURE_TYPE_SUBMIT_INFO,	//sType;
		NULL,							//pNext;
		1,								//waitSemaphoreCount;
		&core.present_semaphore,		//pWaitSemaphores;
		&waitStage,						//pWaitDstStageMask;
		1,								//commandBufferCount;
		&core.p_cmd_buffers[0],			//pCommandBuffers;
		1,								//signalSemaphoreCount;
		&core.render_semaphore,			//pSignalSemaphores;
	};

	vkQueueSubmit(core.graphics_queue, 1, &submitInfo, core.render_fence);

	// Present the ready image
	VkPresentInfoKHR presentInfo = {
		VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,	//sType;
		NULL,								//pNext;
		1,									//waitSemaphoreCount;
		&core.render_semaphore, 			//pWaitSemaphores;
		1,									//swapchainCount;
		&core.swapchain,					//pSwapchains;
		&swapchainImageIndex,				//pImageIndices;
		NULL								//pResults;
	};

	vkQueuePresentKHR(core.graphics_queue, &presentInfo);
}

void shRenderMesh(const ShVkCore core, const ShVkPipelineData pipe_data, const uint32_t push_const_size, void* p_push_const, const uint32_t uniforms_size, void* p_uniforms, ShMeshInfo* p_mesh_info, ShMesh* mesh) {

	//Map mesh buffers
	if (p_mesh_info->flags & SH_MESH_SETUP_DYNAMIC_MESH) {
		//if (mesh->flags & SH_MESH_SETUP_RUNTIME_MESH) {
		//	if (mesh->vertex_count >= 0 && mesh->p_vertices != NULL) {
		//		shAllocateMeshVertexData(core, mesh);
		//	}
		//	if (mesh->index_count >= 0 && mesh->p_indices != NULL) {
		//		shAllocateMeshIndexData(core, mesh);
		//	}
		//}
		shMapVertexBufferMemory(core, p_mesh_info, mesh);
		if (mesh->index_buffer_memory != NULL) {
			shMapIndexBufferMemory(core, p_mesh_info, mesh);
		}
	}

	//Bind vertex and index buffers
	if (mesh->vertex_buffer_memory != NULL) {
		shBindVertexBuffers(core, *mesh);
	}
	if (mesh->index_buffer_memory != NULL) {
		shBindIndexBuffers(core, *mesh);
	}

	shBindPipeline(core.p_cmd_buffers[0], pipe_data);

	//push constants
	if (push_const_size != 0 && p_push_const != NULL) {
		shPushConstants(core.p_cmd_buffers[0], pipe_data, p_push_const);
	}

	// bind uniform memory
	uint32_t uniform_offset = 0;
	for (uint32_t i = 0; i < pipe_data.uniform_buffer_count; i++) {
			shMapMemory(core.device, 
				pipe_data.p_uniform_buffers[i].uniform_buffer_memory, 
				pipe_data.p_uniform_buffers[i].uniform_buffer_size, 
				&((char*)p_uniforms)[uniform_offset]);
			shBindDescriptorSets(core, pipe_data);
			uniform_offset += pipe_data.p_uniform_buffers[i].uniform_buffer_size;
	}
	
	
	if (mesh->index_buffer_memory != NULL) { //indexed
		shDraw(core.p_cmd_buffers[0], p_mesh_info->index_count, pipe_data.vertexStride / 4, *mesh);
	}
	else { //not indexed
		shDraw(core.p_cmd_buffers[0], p_mesh_info->vertex_count, pipe_data.vertexStride / 4, *mesh);
	}

	//if (mesh->flags & SH_MESH_SETUP_DYNAMIC_MESH & SH_MESH_SETUP_RUNTIME_MESH) {
	//	if (mesh->vertex_count >= 0 && mesh->p_vertices != NULL) {
	//		shClearBufferMemory(core.device, mesh->vertex_buffer, mesh->vertex_buffer_memory);
	//	}
	//	if (mesh->index_count >= 0 && mesh->p_indices != NULL) {
	//		shClearBufferMemory(core.device, mesh->index_buffer, mesh->index_buffer_memory);
	//	}
	//}
	//
}