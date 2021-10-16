#include "fggDrawLoop.h"

#include "fggVkCore.h"
#include "fggVkPipelineData.h"
#include "fggVkMemoryInfo.h"
#include "fggMeshInfo.h"

void fggFrameReset(const FggVkCore core) {
	vkWaitForFences(core.device, 1, &core.render_fence, 1, 1000000000);
	vkResetFences(core.device, 1, &core.render_fence);

	vkResetCommandBuffer(core.p_cmd_buffers[0], 0);
}

void fggFrameBegin(const FggVkCore core, uint32_t* pSwapchainImageIndex) {

	vkAcquireNextImageKHR(core.device, core.swapchain, 1000000000, core.present_semaphore, 0, pSwapchainImageIndex);

	VkCommandBufferBeginInfo cmdBufferBeginInfo = {
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,	//sType;
		NULL,											//pNext;
		VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,	//flags; //submit cmd buffer once, before the next reset
		NULL											//pInheritanceInfo;
	};

	VkClearValue clearColor = { {0.1f, 0.1f, 0.1f} };
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
		&clearColor										//pClearValues;
	};

	vkBeginCommandBuffer(core.p_cmd_buffers[0], &cmdBufferBeginInfo);

	vkCmdBeginRenderPass(core.p_cmd_buffers[0], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void fggBindPipeline(const VkCommandBuffer graphicsCmdBuffer, const FggVkPipelineData pipeData) {

	vkCmdBindPipeline(graphicsCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeData.pipeline);
}

void fggBindVertexBuffers(const FggVkCore core, const FggMesh mesh) {
	const VkDeviceSize offset = 0;
	vkCmdBindVertexBuffers(core.p_cmd_buffers[0], 0, 1, &mesh.vertex_buffer, &offset);
}

void fggBindIndexBuffers(const FggVkCore core, const FggMesh mesh) {
	vkCmdBindIndexBuffer(core.p_cmd_buffers[0], mesh.index_buffer, 0, VK_INDEX_TYPE_UINT32);
}

void fggPushConstants(const VkCommandBuffer graphicsCmdBuffer, const FggVkPipelineData pipeData, const void* pPushConstantsData) {
	if (pipeData.pushConstantRange.size > 0 || pPushConstantsData != NULL) {
		vkCmdPushConstants(graphicsCmdBuffer, pipeData.mainPipelineLayout, pipeData.pushConstantRange.stageFlags, pipeData.pushConstantRange.offset, pipeData.pushConstantRange.size, pPushConstantsData);
	}
}

void fggBindDescriptorSets(const FggVkCore core, FggVkPipelineData pipeData) {
	pipeData.writeDescriptorSet.pBufferInfo = &pipeData.descriptorBufferInfo;
	//if (!pipeData.shared_uniform|| !uniform_updated) {
	vkUpdateDescriptorSets(core.device, 1, &pipeData.writeDescriptorSet, 0, NULL);
	//}
	vkCmdBindDescriptorSets(core.p_cmd_buffers[0], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeData.mainPipelineLayout, 0, 1, &pipeData.descriptorSet, 0, NULL);
}

void fggDraw(const VkCommandBuffer graphicsCmdBuffer, const uint32_t count, const uint32_t stride, const FggMesh mesh) {

	if (mesh.index_buffer_memory == NULL) {
		vkCmdDraw(graphicsCmdBuffer, count / stride, 1, 0, 0);
	}
	else {
		vkCmdDrawIndexed(graphicsCmdBuffer, count, 1, 0, 0, 0);
	}
}

void fggFrameEnd(const FggVkCore core, const uint32_t swapchainImageIndex) {

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

void fggRenderMesh(const FggVkCore core, const FggVkPipelineData pipe_data, const uint32_t push_const_size, void* p_push_const, const uint32_t uniform_size, void* p_uniform, const FggMeshInfo mesh_info, FggMesh* mesh) {

	//Map mesh buffers
	if (mesh_info.flags & FGG_MESH_SETUP_DYNAMIC_MESH) {
		//if (mesh->flags & FGG_MESH_SETUP_RUNTIME_MESH) {
		//	if (mesh->vertex_count >= 0 && mesh->p_vertices != NULL) {
		//		fggAllocateMeshVertexData(core, mesh);
		//	}
		//	if (mesh->index_count >= 0 && mesh->p_indices != NULL) {
		//		fggAllocateMeshIndexData(core, mesh);
		//	}
		//}
		fggMapVertexBufferMemory(core, mesh_info, mesh);
		if (mesh->index_buffer_memory != NULL) {
			fggMapIndexBufferMemory(core, mesh_info, mesh);
		}
	}

	//Bind vertex and index buffers
	if (mesh->vertex_buffer_memory != NULL) {
		fggBindVertexBuffers(core, *mesh);
	}
	if (mesh->index_buffer_memory != NULL) {
		fggBindIndexBuffers(core, *mesh);
	}

	fggBindPipeline(core.p_cmd_buffers[0], pipe_data);

	//push constants
	if (push_const_size != 0 && p_push_const != NULL) {
		fggPushConstants(core.p_cmd_buffers[0], pipe_data, p_push_const);
	}

	// bind uniform memory
	if (uniform_size != 0 && p_uniform != NULL) {
		fggMapMemory(core.device, pipe_data.uniformBufferMemory, uniform_size, p_uniform);
		fggBindDescriptorSets(core, pipe_data);
	}
	
	if (mesh->index_buffer_memory != NULL) { //indexed
		fggDraw(core.p_cmd_buffers[0], mesh_info.index_count, pipe_data.vertexStride / 4, *mesh);
	}
	else { //not indexed
		fggDraw(core.p_cmd_buffers[0], mesh_info.vertex_count, pipe_data.vertexStride / 4, *mesh);
	}

	//if (mesh->flags & FGG_MESH_SETUP_DYNAMIC_MESH & FGG_MESH_SETUP_RUNTIME_MESH) {
	//	if (mesh->vertex_count >= 0 && mesh->p_vertices != NULL) {
	//		fggClearBufferMemory(core.device, mesh->vertex_buffer, mesh->vertex_buffer_memory);
	//	}
	//	if (mesh->index_count >= 0 && mesh->p_indices != NULL) {
	//		fggClearBufferMemory(core.device, mesh->index_buffer, mesh->index_buffer_memory);
	//	}
	//}
	//
}