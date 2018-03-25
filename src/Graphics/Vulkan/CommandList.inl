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

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	ETInlineHint VkCommandBuffer CommandList::Get() {
		return _commandBuffer;
	}

// ---------------------------------------------------

	ETInlineHint CommandList::operator VkCommandBuffer() {
		return _commandBuffer;
	}

// ---------------------------------------------------

	template <uint32_t bufferCount>
	ETInlineHint void CommandList::BindBuffers( const VkBuffer (&buffers)[bufferCount], const VkDeviceSize (&offsets)[bufferCount], VkBuffer indexBuffer, VkIndexType type ) {
		this->BindBuffers( bufferCount, buffers, offsets, indexBuffer, type );
	}

// ---------------------------------------------------

	template <uint32_t bufferCount>
	ETInlineHint void CommandList::BindBuffers( const VkBuffer (&buffers)[bufferCount], VkBuffer indexBuffer, VkIndexType type ) {
		const VkDeviceSize offsets[bufferCount] = { 0u };

		this->BindBuffers( bufferCount, buffers, offsets, indexBuffer, type );
	}

// ---------------------------------------------------

	template <uint32_t bufferCount>
	ETInlineHint void CommandList::BindBuffers( const VkBuffer (&buffers)[bufferCount], const VkDeviceSize (&offsets)[bufferCount] ) {
		this->BindBuffers( bufferCount, buffers, offsets );
	}

// ---------------------------------------------------

	template <uint32_t bufferCount>
	ETInlineHint void CommandList::BindBuffers( const VkBuffer (&buffers)[bufferCount] ) {
		const VkDeviceSize offsets[bufferCount] = { 0u };

		this->BindBuffers( bufferCount, buffers, offsets );
	}

// ---------------------------------------------------

	template <uint32_t viewportCount>
	ETInlineHint void CommandList::SetViewports( uint32_t firstViewport, const VkViewport (&viewports)[viewportCount] ) {
		this->SetViewports( firstViewport, viewportCount, viewports );
	}

// ---------------------------------------------------

	template <uint32_t scissorCount>
	ETInlineHint void CommandList::SetScissors( uint32_t firstScissor, const VkRect2D (&scissors)[scissorCount] ) {
		this->SetScissors( firstScissor, scissorCount, scissors );
	}

// ---------------------------------------------------

	template <uint32_t blitCount>
	ETInlineHint void CommandList::Copy( VkImage target, VkImageLayout targetLayout, VkImage source, VkImageLayout sourceLayout, const VkImageBlit (&regions)[blitCount], VkFilter filter ) {
		this->Copy( target, targetLayout, source, sourceLayout, blitCount, regions, filter );
	}

// ---------------------------------------------------

	template <uint32_t copyCount>
	ETInlineHint void CommandList::Copy( VkImage target, VkImageLayout targetLayout, VkImage source, VkImageLayout sourceLayout, const VkImageCopy (&regions)[copyCount] ) {
		this->Copy( target, targetLayout, source, sourceLayout, copyCount, regions );
	}

// ---------------------------------------------------

	template <uint32_t copyCount>
	ETInlineHint void CommandList::Copy( VkImage target, VkImageLayout targetLayout, VkBuffer source, const VkBufferImageCopy (&regions)[copyCount] ) {
		this->Copy( target, targetLayout, source, copyCount, regions );
	}

// ---------------------------------------------------

	template <uint32_t copyCount>
	ETInlineHint void CommandList::Copy( VkBuffer target, VkImage source, VkImageLayout sourceLayout, const VkBufferImageCopy (&regions)[copyCount] ) {
		this->Copy( target, source, sourceLayout, copyCount, regions );
	}

// ---------------------------------------------------

	template <uint32_t copyCount>
	ETInlineHint void CommandList::Copy( VkBuffer target, VkBuffer source, const VkBufferCopy (&regions)[copyCount] ) {
		this->Copy( target, source, copyCount, regions );
	}

// ---------------------------------------------------

	template <uint32_t resolveCount>
	ETInlineHint void CommandList::Resolve( VkImage target, VkImageLayout targetLayout, VkImage source, VkImageLayout sourceLayout, const VkImageResolve (&regions)[resolveCount] ) {
		this->Resolve( target, targetLayout, source, sourceLayout, resolveCount, regions );
	}

// ---------------------------------------------------

	template <uint32_t clearCount>
	ETInlineHint void CommandList::Clear( VkImage target, VkImageLayout layout, const VkClearDepthStencilValue& value, const VkImageSubresourceRange (&ranges)[clearCount] ) {
		this->Clear( target, layout, value, clearCount, ranges );
	}

// ---------------------------------------------------

	template <uint32_t clearCount>
	ETInlineHint void CommandList::Clear( VkImage target, VkImageLayout layout, const VkClearColorValue& value, const VkImageSubresourceRange (&ranges)[clearCount] ) {
		this->Clear( target, layout, value, clearCount, ranges );
	}

// ---------------------------------------------------

	template <uint32_t clearCount, uint32_t regionCount>
	ETInlineHint void CommandList::Clear( const VkClearAttachment (&targets)[clearCount], const VkClearRect (&regions)[regionCount] ) {
		this->Clear( clearCount, targets, regionCount, regions );
	}

// ---------------------------------------------------

	template <uint32_t waitCount, uint32_t barrierCount, uint32_t bufferBarrierCount, uint32_t imageBarrierCount>
	ETInlineHint void CommandList::AwaitEvents(
		const VkEvent (&waitEvents)[waitCount],
		VkPipelineStageFlags sourceStageMask,
		VkPipelineStageFlags targetStageMask,
		const VkMemoryBarrier (&memoryBarriers)[barrierCount],
		const VkBufferMemoryBarrier (&bufferBarriers)[bufferBarrierCount],
		const VkImageMemoryBarrier (&imageBarriers)[imageBarrierCount]
	) {
		this->AwaitEvents( waitCount, waitEvents, sourceStageMask, targetStageMask, barrierCount, memoryBarriers, bufferBarrierCount, bufferBarriers, imageBarrierCount, imageBarriers );
	}

// ---------------------------------------------------

	ETInlineHint void CommandList::AwaitEvents( uint32_t waitCount, const VkEvent* waitEvents, VkPipelineStageFlags sourceStageMask, VkPipelineStageFlags targetStageMask ) {
		this->AwaitEvents( waitCount, waitEvents, sourceStageMask, targetStageMask, 0u, nullptr, 0u, nullptr, 0u, nullptr );
	}

// ---------------------------------------------------

	template <uint32_t waitCount>
	ETInlineHint void CommandList::AwaitEvents( const VkEvent (&waitEvents)[waitCount], VkPipelineStageFlags sourceStageMask, VkPipelineStageFlags targetStageMask ) {
		this->AwaitEvents( waitCount, waitEvents, sourceStageMask, targetStageMask, 0u, nullptr, 0u, nullptr, 0u, nullptr );
	}

// ---------------------------------------------------

	template <uint32_t barrierCount, uint32_t bufferBarrierCount, uint32_t imageBarrierCount>
	ETInlineHint void CommandList::PipelineBarrier(
		VkDependencyFlags dependencyFlags,
		VkPipelineStageFlags sourceStageMask,
		VkPipelineStageFlags targetStageMask,
		const VkMemoryBarrier (&memoryBarriers)[barrierCount],
		const VkBufferMemoryBarrier (&bufferBarriers)[bufferBarrierCount],
		const VkImageMemoryBarrier (&imageBarriers)[imageBarrierCount]
	) {
		this->PipelineBarrier( dependencyFlags, sourceStageMask, targetStageMask, barrierCount, memoryBarriers, bufferBarrierCount, bufferBarriers, imageBarrierCount, imageBarriers );
	}

// ---------------------------------------------------

	template <uint32_t bufferCount>
	ETInlineHint void CommandList::Execute( const VkCommandBuffer (&commands)[bufferCount] ) {
		this->Execute( bufferCount, commands );
	}

// ---------------------------------------------------

	ETInlineHint void Swap( CommandList& list0, CommandList& list1 ) {
		using ::Eldritch2::Swap;

		Swap( list0._pool,         	list1._pool );
		Swap( list0._commandBuffer, list1._commandBuffer );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2