/*==================================================================*\
  CommandList.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/CommandList.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	CommandList::CommandList(CommandList&& list) :
		CommandList() {
		Swap(*this, list);
	}

	// ---------------------------------------------------

	CommandList::CommandList() :
		_pool(nullptr),
		_commandBuffer(nullptr) {}

	// ---------------------------------------------------

	CommandList::~CommandList() {
		ET_ASSERT(_pool == nullptr, "Leaking Vulkan command pool!");
	}

	// ---------------------------------------------------

	VkResult CommandList::ResetPool(Gpu& gpu, VkCommandPoolResetFlags flags) {
		return vkResetCommandPool(gpu, _pool, flags);
	}

	// ---------------------------------------------------

	VkResult CommandList::BindResources(Gpu& gpu, QueueConcept queue, VkCommandPoolCreateFlags flags) {
		using ::Eldritch2::Swap;

		const VkCommandPoolCreateInfo poolInfo{
			VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			/*pNext =*/nullptr,
			flags,
			gpu.GetQueueFamilyByConcept(queue)
		};

		VkCommandPool   pool(nullptr);
		VkCommandBuffer commandBuffer(nullptr);

		ET_FAIL_UNLESS(vkCreateCommandPool(gpu, &poolInfo, gpu.GetAllocationCallbacks(), &pool));
		ET_AT_SCOPE_EXIT(vkDestroyCommandPool(gpu, pool, gpu.GetAllocationCallbacks()));

		const VkCommandBufferAllocateInfo commandBufferInfo{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			/*pNext =*/nullptr,
			pool,
			VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			/*commandBufferCount =*/1u
		};

		ET_FAIL_UNLESS(vkAllocateCommandBuffers(gpu, &commandBufferInfo, &commandBuffer));
		ET_AT_SCOPE_EXIT(if (commandBuffer != nullptr) vkFreeCommandBuffers(gpu, pool, commandBufferInfo.commandBufferCount, &commandBuffer););

		Swap(_pool, pool);
		Swap(_commandBuffer, commandBuffer);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void CommandList::FreeResources(Gpu& gpu) {
		if (VkCommandBuffer commandBuffer = eastl::exchange(_commandBuffer, nullptr)) {
			vkFreeCommandBuffers(gpu, _pool, 1u, &commandBuffer);
		}

		vkDestroyCommandPool(gpu, eastl::exchange(_pool, nullptr), gpu.GetAllocationCallbacks());
	}

	// ---------------------------------------------------

	VkResult CommandList::BeginRecording(VkCommandBufferUsageFlags flags) {
		const VkCommandBufferBeginInfo beginInfo{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			/*pNext =*/nullptr,
			flags,
			/*pInheritanceInfo =*/nullptr
		};

		return vkBeginCommandBuffer(_commandBuffer, &beginInfo);
	}

	// ---------------------------------------------------

	VkResult CommandList::FinishRecording() {
		return vkEndCommandBuffer(_commandBuffer);
	}

	// ---------------------------------------------------

	void CommandList::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {
		vkCmdDraw(_commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
	}

	// ---------------------------------------------------

	void CommandList::DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) {
		vkCmdDrawIndexed(_commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
	}

	// ---------------------------------------------------

	void CommandList::DrawIndirect(VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t drawStrideInBytes) {
		vkCmdDrawIndirect(_commandBuffer, buffer, offset, drawCount, drawStrideInBytes);
	}

	// ---------------------------------------------------

	void CommandList::DrawIndexedIndirect(VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t drawStrideInBytes) {
		vkCmdDrawIndexedIndirect(_commandBuffer, buffer, offset, drawCount, drawStrideInBytes);
	}

	// ---------------------------------------------------

	void CommandList::Dispatch(VkEvent halfComplete, VkEvent fullComplete, uint32_t x, uint32_t y, uint32_t z) {
		const bool shouldSplitDispatch(halfComplete != VK_NULL_HANDLE);

		if (shouldSplitDispatch) {
			//	We can increase GPU utilization % by optionally kicking off two smaller dispatches in place of the single large operation requested by the user.
			vkCmdDispatch(_commandBuffer, x / 2, y, z);
			vkCmdSetEvent(_commandBuffer, halfComplete, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);
			vkCmdWaitEvents(_commandBuffer, 1u, &halfComplete, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0u, nullptr, 0u, nullptr, 0u, nullptr);
			vkCmdDispatch(_commandBuffer, x / 2, y, z);
		} else {
			vkCmdDispatch(_commandBuffer, x, y, z);
		}

		if (fullComplete != VK_NULL_HANDLE) {
			vkCmdSetEvent(_commandBuffer, fullComplete, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);
		}
	}

	// ---------------------------------------------------

	void CommandList::SetPipeline(VkPipelineBindPoint target, VkPipeline pipeline, VkPipelineLayout layout, uint32_t setCount, const VkDescriptorSet* descriptorSets, uint32_t offsetCount, const uint32_t* offsets) {
		vkCmdBindPipeline(_commandBuffer, target, pipeline);
		vkCmdBindDescriptorSets(_commandBuffer, target, layout, 0u, setCount, descriptorSets, offsetCount, offsets);
	}

	// ---------------------------------------------------

	void CommandList::BindBuffers(uint32_t count, const VkBuffer* buffers, const VkDeviceSize* offsets, VkBuffer indexBuffer, VkIndexType type) {
		vkCmdBindVertexBuffers(_commandBuffer, 0u, count, buffers, offsets);
		vkCmdBindIndexBuffer(_commandBuffer, indexBuffer, 0u, type);
	}

	// ---------------------------------------------------

	void CommandList::BindBuffers(uint32_t count, const VkBuffer* buffers, const VkDeviceSize* offsets) {
		vkCmdBindVertexBuffers(_commandBuffer, 0u, count, buffers, offsets);
	}

	// ---------------------------------------------------

	void CommandList::SetViewports(uint32_t firstViewport, uint32_t count, const VkViewport* viewports) {
		vkCmdSetViewport(_commandBuffer, firstViewport, count, viewports);
	}

	// ---------------------------------------------------

	void CommandList::SetScissors(uint32_t firstScissor, uint32_t count, const VkRect2D* scissors) {
		vkCmdSetScissor(_commandBuffer, firstScissor, count, scissors);
	}

	// ---------------------------------------------------

	void CommandList::SetLineWidth(float width) {
		vkCmdSetLineWidth(_commandBuffer, width);
	}

	// ---------------------------------------------------

	void CommandList::SetBlendConstants(const float (&constants)[4]) {
		vkCmdSetBlendConstants(_commandBuffer, constants);
	}

	// ---------------------------------------------------

	void CommandList::SetDepthBias(float constantFactor, float clamp, float slopeFactor) {
		vkCmdSetDepthBias(_commandBuffer, constantFactor, clamp, slopeFactor);
	}

	// ---------------------------------------------------

	void CommandList::SetDepthBounds(float min, float max) {
		vkCmdSetDepthBounds(_commandBuffer, min, max);
	}

	// ---------------------------------------------------

	void CommandList::SetStencilReadMask(VkStencilFaceFlags face, uint32_t mask) {
		vkCmdSetStencilCompareMask(_commandBuffer, face, mask);
	}

	// ---------------------------------------------------

	void CommandList::SetStencilWriteMask(VkStencilFaceFlags face, uint32_t mask) {
		vkCmdSetStencilWriteMask(_commandBuffer, face, mask);
	}

	// ---------------------------------------------------

	void CommandList::SetStencilReference(VkStencilFaceFlags face, uint32_t reference) {
		vkCmdSetStencilReference(_commandBuffer, face, reference);
	}

	// ---------------------------------------------------

	void CommandList::Copy(VkImage target, VkImageLayout targetLayout, VkImage source, VkImageLayout sourceLayout, uint32_t regionCount, const VkImageBlit* regions, VkFilter filter) {
		vkCmdBlitImage(_commandBuffer, source, sourceLayout, target, targetLayout, regionCount, regions, filter);
	}

	// ---------------------------------------------------

	void CommandList::Copy(VkImage target, VkImageLayout targetLayout, VkImage source, VkImageLayout sourceLayout, uint32_t regionCount, const VkImageCopy* regions) {
		vkCmdCopyImage(_commandBuffer, source, sourceLayout, target, targetLayout, regionCount, regions);
	}

	// ---------------------------------------------------

	void CommandList::Copy(VkImage target, VkImageLayout targetLayout, VkBuffer source, uint32_t regionCount, const VkBufferImageCopy* regions) {
		vkCmdCopyBufferToImage(_commandBuffer, source, target, targetLayout, regionCount, regions);
	}

	// ---------------------------------------------------

	void CommandList::Copy(VkBuffer target, VkImage source, VkImageLayout sourceLayout, uint32_t regionCount, const VkBufferImageCopy* regions) {
		vkCmdCopyImageToBuffer(_commandBuffer, source, sourceLayout, target, regionCount, regions);
	}

	// ---------------------------------------------------

	void CommandList::Copy(VkBuffer target, VkBuffer source, uint32_t regionCount, const VkBufferCopy* regions) {
		vkCmdCopyBuffer(_commandBuffer, source, target, regionCount, regions);
	}

	// ---------------------------------------------------

	void CommandList::Resolve(VkImage target, VkImageLayout targetLayout, VkImage source, VkImageLayout sourceLayout, uint32_t resolveCount, const VkImageResolve* resolves) {
		vkCmdResolveImage(_commandBuffer, source, sourceLayout, target, targetLayout, resolveCount, resolves);
	}

	// ---------------------------------------------------

	void CommandList::UpdateBuffer(VkBuffer target, VkDeviceSize offsetInBytes, const void* sourceData, VkDeviceSize lengthInBytes) {
		vkCmdUpdateBuffer(_commandBuffer, target, offsetInBytes, lengthInBytes, sourceData);
	}

	// ---------------------------------------------------

	void CommandList::FillBuffer(VkBuffer target, VkDeviceSize offsetInBytes, uint32_t pattern, VkDeviceSize lengthInBytes) {
		vkCmdFillBuffer(_commandBuffer, target, offsetInBytes, lengthInBytes, pattern);
	}

	// ---------------------------------------------------

	void CommandList::Clear(VkImage target, VkImageLayout layout, const VkClearDepthStencilValue& value, uint32_t rangeCount, const VkImageSubresourceRange* ranges) {
		vkCmdClearDepthStencilImage(_commandBuffer, target, layout, &value, rangeCount, ranges);
	}

	// ---------------------------------------------------

	void CommandList::Clear(VkImage target, VkImageLayout layout, const VkClearColorValue& value, uint32_t clearCount, const VkImageSubresourceRange* ranges) {
		vkCmdClearColorImage(_commandBuffer, target, layout, &value, clearCount, ranges);
	}

	// ---------------------------------------------------

	void CommandList::Clear(uint32_t targetCount, const VkClearAttachment* targets, uint32_t regionCount, const VkClearRect* regions) {
		vkCmdClearAttachments(_commandBuffer, targetCount, targets, regionCount, regions);
	}

	// ---------------------------------------------------

	void CommandList::SetEvent(VkEvent event, VkPipelineStageFlags mask) {
		vkCmdSetEvent(_commandBuffer, event, mask);
	}

	// ---------------------------------------------------

	void CommandList::ResetEvent(VkEvent event, VkPipelineStageFlags mask) {
		vkCmdResetEvent(_commandBuffer, event, mask);
	}

	// ---------------------------------------------------

	void CommandList::AwaitEvents(
		uint32_t waitCount, const VkEvent* waitEvents,
		VkPipelineStageFlags sourceStageMask,
		VkPipelineStageFlags targetStageMask,
		uint32_t barrierCount, const VkMemoryBarrier* memoryBarriers,
		uint32_t bufferBarrierCount, const VkBufferMemoryBarrier* bufferBarriers,
		uint32_t imageBarrierCount, const VkImageMemoryBarrier* imageBarriers) {
		vkCmdWaitEvents(
			_commandBuffer,
			waitCount,
			waitEvents,
			sourceStageMask,
			targetStageMask,
			barrierCount,
			memoryBarriers,
			bufferBarrierCount,
			bufferBarriers,
			imageBarrierCount,
			imageBarriers);
	}

	// ---------------------------------------------------

	void CommandList::PipelineBarrier(
		VkDependencyFlags            dependencyFlags,
		VkPipelineStageFlags         sourceStageMask,
		VkPipelineStageFlags         targetStageMask,
		uint32_t                     barrierCount,
		const VkMemoryBarrier*       memoryBarriers,
		uint32_t                     bufferBarrierCount,
		const VkBufferMemoryBarrier* bufferBarriers,
		uint32_t                     imageBarrierCount,
		const VkImageMemoryBarrier*  imageBarriers) {
		vkCmdPipelineBarrier(
			_commandBuffer,
			sourceStageMask,
			targetStageMask,
			dependencyFlags,
			barrierCount,
			memoryBarriers,
			bufferBarrierCount,
			bufferBarriers,
			imageBarrierCount,
			imageBarriers);
	}

	// ---------------------------------------------------

	void CommandList::BeginQuery(VkQueryPool pool, uint32_t query, VkQueryControlFlags flags) {
		vkCmdBeginQuery(_commandBuffer, pool, query, flags);
	}

	// ---------------------------------------------------

	void CommandList::EndQuery(VkQueryPool pool, uint32_t query) {
		vkCmdEndQuery(_commandBuffer, pool, query);
	}

	// ---------------------------------------------------

	void CommandList::CopyQueryPoolResults(
		VkQueryPool        pool,
		uint32_t           firstQuery,
		uint32_t           count,
		VkBuffer           target,
		VkDeviceSize       offsetIntoTargetInBytes,
		VkDeviceSize       targetStrideInBytes,
		VkQueryResultFlags flags) {
		vkCmdCopyQueryPoolResults(_commandBuffer, pool, firstQuery, count, target, offsetIntoTargetInBytes, targetStrideInBytes, flags);
	}

	// ---------------------------------------------------

	void CommandList::ResetQueryPool(VkQueryPool pool, uint32_t firstQuery, uint32_t count) {
		vkCmdResetQueryPool(_commandBuffer, pool, firstQuery, count);
	}

	// ---------------------------------------------------

	void CommandList::WriteTimestamp(VkPipelineStageFlagBits stage, VkQueryPool pool, uint32_t query) {
		vkCmdWriteTimestamp(_commandBuffer, stage, pool, query);
	}

	// ---------------------------------------------------

	void CommandList::PushConstants(VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t sizeInBytes, const void* values) {
		vkCmdPushConstants(_commandBuffer, layout, stageFlags, offset, sizeInBytes, values);
	}

	// ---------------------------------------------------

	void CommandList::Execute(uint32_t bufferCount, const VkCommandBuffer* buffers) {
		vkCmdExecuteCommands(_commandBuffer, bufferCount, buffers);
	}

	// ---------------------------------------------------

	void CommandList::BeginRendering(VkSubpassContents contents, const VkRenderPassBeginInfo& beginInfo) {
		vkCmdBeginRenderPass(_commandBuffer, &beginInfo, contents);
	}

	// ---------------------------------------------------

	void CommandList::AdvanceSubpass(VkSubpassContents contents) {
		vkCmdNextSubpass(_commandBuffer, contents);
	}

	// ---------------------------------------------------

	void CommandList::FinishRendering() {
		vkCmdEndRenderPass(_commandBuffer);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
