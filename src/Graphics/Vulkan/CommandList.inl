/*==================================================================*\
  CommandList.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	ETInlineHint VkCommandBuffer CommandList::Get() {
		return _commandBuffer;
	}

	// ---------------------------------------------------

	ETInlineHint CommandList::operator VkCommandBuffer() {
		return _commandBuffer;
	}

	// ---------------------------------------------------

	template <uint32_t count>
	ETInlineHint void CommandList::BindBuffers(const VkBuffer (&buffers)[count], const VkDeviceSize (&offsets)[count], VkBuffer indexBuffer, VkIndexType type) {
		this->BindBuffers(count, buffers, offsets, indexBuffer, type);
	}

	// ---------------------------------------------------

	template <uint32_t count>
	ETInlineHint void CommandList::BindBuffers(const VkBuffer (&buffers)[count], VkBuffer indexBuffer, VkIndexType type) {
		const VkDeviceSize offsets[count] = { 0u };

		this->BindBuffers(count, buffers, offsets, indexBuffer, type);
	}

	// ---------------------------------------------------

	template <uint32_t count>
	ETInlineHint void CommandList::BindBuffers(const VkBuffer (&buffers)[count], const VkDeviceSize (&offsets)[count]) {
		this->BindBuffers(count, buffers, offsets);
	}

	// ---------------------------------------------------

	template <uint32_t count>
	ETInlineHint void CommandList::BindBuffers(const VkBuffer (&buffers)[count]) {
		const VkDeviceSize offsets[count] = { 0u };

		this->BindBuffers(count, buffers, offsets);
	}

	// ---------------------------------------------------

	template <uint32_t viewportCount>
	ETInlineHint void CommandList::SetViewports(uint32_t firstViewport, const VkViewport (&viewports)[viewportCount]) {
		this->SetViewports(firstViewport, viewportCount, viewports);
	}

	// ---------------------------------------------------

	template <uint32_t scissorCount>
	ETInlineHint void CommandList::SetScissors(uint32_t firstScissor, const VkRect2D (&scissors)[scissorCount]) {
		this->SetScissors(firstScissor, scissorCount, scissors);
	}

	// ---------------------------------------------------

	template <uint32_t count>
	ETInlineHint void CommandList::Copy(VkImage target, VkImageLayout targetLayout, VkImage source, VkImageLayout sourceLayout, const VkImageBlit (&regions)[count], VkFilter filter) {
		this->Copy(target, targetLayout, source, sourceLayout, count, regions, filter);
	}

	// ---------------------------------------------------

	template <uint32_t count>
	ETInlineHint void CommandList::Copy(VkImage target, VkImageLayout targetLayout, VkImage source, VkImageLayout sourceLayout, const VkImageCopy (&regions)[count]) {
		this->Copy(target, targetLayout, source, sourceLayout, count, regions);
	}

	// ---------------------------------------------------

	template <uint32_t count>
	ETInlineHint void CommandList::Copy(VkImage target, VkImageLayout targetLayout, VkBuffer source, const VkBufferImageCopy (&regions)[count]) {
		this->Copy(target, targetLayout, source, count, regions);
	}

	// ---------------------------------------------------

	template <uint32_t count>
	ETInlineHint void CommandList::Copy(VkBuffer target, VkImage source, VkImageLayout sourceLayout, const VkBufferImageCopy (&regions)[count]) {
		this->Copy(target, source, sourceLayout, count, regions);
	}

	// ---------------------------------------------------

	template <uint32_t count>
	ETInlineHint void CommandList::Copy(VkBuffer target, VkBuffer source, const VkBufferCopy (&regions)[count]) {
		this->Copy(target, source, count, regions);
	}

	// ---------------------------------------------------

	template <uint32_t count>
	ETInlineHint void CommandList::Resolve(VkImage target, VkImageLayout targetLayout, VkImage source, VkImageLayout sourceLayout, const VkImageResolve (&regions)[count]) {
		this->Resolve(target, targetLayout, source, sourceLayout, count, regions);
	}

	// ---------------------------------------------------

	template <uint32_t count>
	ETInlineHint void CommandList::Clear(VkImage target, VkImageLayout layout, const VkClearDepthStencilValue& value, const VkImageSubresourceRange (&ranges)[count]) {
		this->Clear(target, layout, value, count, ranges);
	}

	// ---------------------------------------------------

	template <uint32_t count>
	ETInlineHint void CommandList::Clear(VkImage target, VkImageLayout layout, const VkClearColorValue& value, const VkImageSubresourceRange (&ranges)[count]) {
		this->Clear(target, layout, value, count, ranges);
	}

	// ---------------------------------------------------

	template <uint32_t count, uint32_t regionCount>
	ETInlineHint void CommandList::Clear(const VkClearAttachment (&targets)[count], const VkClearRect (&regions)[regionCount]) {
		this->Clear(count, targets, regionCount, regions);
	}

	// ---------------------------------------------------

	template <uint32_t count, uint32_t barrierCount, uint32_t bufferBarrierCount, uint32_t imageBarrierCount>
	ETInlineHint void CommandList::AwaitEvents(
		const VkEvent (&waitEvents)[count],
		VkPipelineStageFlags sourceStageMask,
		VkPipelineStageFlags targetStageMask,
		const VkMemoryBarrier (&barriers)[barrierCount],
		const VkBufferMemoryBarrier (&bufferBarriers)[bufferBarrierCount],
		const VkImageMemoryBarrier (&imageBarriers)[imageBarrierCount]) {
		this->AwaitEvents(count, waitEvents, sourceStageMask, targetStageMask, barrierCount, barriers, bufferBarrierCount, bufferBarriers, imageBarrierCount, imageBarriers);
	}

	// ---------------------------------------------------

	ETInlineHint void CommandList::AwaitEvents(uint32_t count, const VkEvent* events, VkPipelineStageFlags sourceStageMask, VkPipelineStageFlags targetStageMask) {
		this->AwaitEvents(count, events, sourceStageMask, targetStageMask, 0u, nullptr, 0u, nullptr, 0u, nullptr);
	}

	// ---------------------------------------------------

	template <uint32_t count>
	ETInlineHint void CommandList::AwaitEvents(const VkEvent (&events)[count], VkPipelineStageFlags sourceStageMask, VkPipelineStageFlags targetStageMask) {
		this->AwaitEvents(count, events, sourceStageMask, targetStageMask, 0u, nullptr, 0u, nullptr, 0u, nullptr);
	}

	// ---------------------------------------------------

	template <uint32_t count, uint32_t bufferBarrierCount, uint32_t imageBarrierCount>
	ETInlineHint void CommandList::PipelineBarrier(
		VkDependencyFlags    dependencyFlags,
		VkPipelineStageFlags sourceStageMask,
		VkPipelineStageFlags targetStageMask,
		const VkMemoryBarrier (&barriers)[count],
		const VkBufferMemoryBarrier (&bufferBarriers)[bufferBarrierCount],
		const VkImageMemoryBarrier (&imageBarriers)[imageBarrierCount]) {
		this->PipelineBarrier(dependencyFlags, sourceStageMask, targetStageMask, count, barriers, bufferBarrierCount, bufferBarriers, imageBarrierCount, imageBarriers);
	}

	// ---------------------------------------------------

	template <uint32_t count>
	ETInlineHint void CommandList::Execute(const VkCommandBuffer (&commands)[count]) {
		this->Execute(count, commands);
	}

	// ---------------------------------------------------

	ETInlineHint void Swap(CommandList& lhs, CommandList& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._pool, rhs._pool);
		Swap(lhs._commandBuffer, rhs._commandBuffer);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
