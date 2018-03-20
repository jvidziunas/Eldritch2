/*==================================================================*\
  CommandList.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <vulkan/vulkan.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Graphics {
		namespace Vulkan {
			enum	QueueConcept : uint32_t;
			class	Gpu;
		}
	}
}

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	class CommandList {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		CommandList( const CommandList& ) = delete;
	//!	Constructs this @ref CommandList instance.
		CommandList( CommandList&& );
	//!	Constructs this @ref CommandList instance.
		CommandList();

		~CommandList();

	// ---------------------------------------------------

	public:
		VkResult	ResetPool( Gpu& gpu, VkCommandPoolResetFlags flags = 0 );

	// ---------------------------------------------------

	public:
		VkResult	BindResources( Gpu& gpu, QueueConcept queue, VkCommandPoolCreateFlags flags );

		void		FreeResources( Gpu& gpu );

	// ---------------------------------------------------

	public:
		VkResult	BeginRecording( VkCommandBufferUsageFlags flags );

		VkResult	FinishRecording();

	// ---------------------------------------------------

	public:
		VkCommandBuffer	Get();

		operator VkCommandBuffer();

	// ---------------------------------------------------

	public:
		void	Draw( uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance );

		void	DrawIndexed( uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance );

		void	DrawIndirect( VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t drawStrideInBytes );

		void	DrawIndexedIndirect( VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t drawStrideInBytes );

		void	Dispatch( uint32_t x, uint32_t y, uint32_t z );

		void	DispatchIndirect( VkBuffer buffer, VkDeviceSize offset );

	// ---------------------------------------------------

	public:
		void	SetDescriptors( VkPipelineBindPoint target, VkPipelineLayout layout, uint32_t setCount, const VkDescriptorSet* descriptorSets, uint32_t offsetCount, const uint32_t* offsets );

		void	SetPipeline( VkPipelineBindPoint target, VkPipeline pipeline );

		void	SetIndexBuffer( VkBuffer buffer, VkDeviceSize offset, VkIndexType type );

		void	SetBuffers( uint32_t firstBuffer, uint32_t bufferCount, const VkBuffer* buffers, const VkDeviceSize* offsets );
		template <uint32_t bufferCount>
		void	SetBuffers( uint32_t firstBuffer, const VkBuffer (&buffers)[bufferCount], const VkDeviceSize (&offsets)[bufferCount] );

	// ---------------------------------------------------

	public:
		void	SetViewports( uint32_t firstViewport, uint32_t viewportCount, const VkViewport* viewports );
		template <uint32_t viewportCount>
		void	SetViewports( uint32_t firstViewport, const VkViewport (&viewports)[viewportCount] );

		void	SetScissors( uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* scissors );
		template <uint32_t scissorCount>
		void	SetScissors( uint32_t firstScissor, const VkRect2D (&scissors)[scissorCount] );

		void	SetLineWidth( float width );

		void	SetBlendConstants( const float (&constants)[4] );

	// ---------------------------------------------------

	public:
		void	SetDepthBias( float constantFactor, float clamp, float slopeFactor );

		void	SetDepthBounds( float min, float max );

		void	SetStencilReadMask( VkStencilFaceFlags face, uint32_t mask );

		void	SetStencilWriteMask( VkStencilFaceFlags face, uint32_t mask );
		
		void	SetStencilReference( VkStencilFaceFlags face, uint32_t reference );

	// ---------------------------------------------------

	public:
		void	CopyRegions( VkImage target, VkImageLayout targetLayout, VkImage source, VkImageLayout sourceLayout, uint32_t regionCount, const VkImageBlit* regions, VkFilter filter );
		template <uint32_t blitCount>
		void	CopyRegions( VkImage target, VkImageLayout targetLayout, VkImage source, VkImageLayout sourceLayout, const VkImageBlit (&regions)[blitCount], VkFilter filter );
		void	CopyRegions( VkImage target, VkImageLayout targetLayout, VkImage source, VkImageLayout sourceLayout, uint32_t copyCount, const VkImageCopy* regions );
		template <uint32_t copyCount>
		void	CopyRegions( VkImage target, VkImageLayout targetLayout, VkImage source, VkImageLayout sourceLayout, const VkImageCopy (&regions)[copyCount] );
		void	CopyRegions( VkImage target, VkImageLayout targetLayout, VkBuffer source, uint32_t copyCount, const VkBufferImageCopy* regions );
		template <uint32_t copyCount>
		void	CopyRegions( VkImage target, VkImageLayout targetLayout, VkBuffer source, const VkBufferImageCopy (&regions)[copyCount] );
		void	CopyRegions( VkBuffer target, VkImage source, VkImageLayout sourceLayout, uint32_t copyCount, const VkBufferImageCopy* regions );
		template <uint32_t copyCount>
		void	CopyRegions( VkBuffer target, VkImage source, VkImageLayout sourceLayout, const VkBufferImageCopy (&regions)[copyCount] );
		void	CopyRegions( VkBuffer target, VkBuffer source, uint32_t copyCount, const VkBufferCopy* regions );
		template <uint32_t copyCount>
		void	CopyRegions( VkBuffer target, VkBuffer source, const VkBufferCopy (&regions)[copyCount] );

		void	Resolve( VkImage target, VkImageLayout targetLayout, VkImage source, VkImageLayout sourceLayout, uint32_t resolveCount, const VkImageResolve* regions );
		template <uint32_t resolveCount>
		void	Resolve( VkImage target, VkImageLayout targetLayout, VkImage source, VkImageLayout sourceLayout, const VkImageResolve (&regions)[resolveCount] );

		void	UpdateBuffer( VkBuffer target, VkDeviceSize offsetInBytes, const void* sourceData, VkDeviceSize lengthInBytes );

		void	FillBuffer( VkBuffer target, VkDeviceSize offsetInBytes, uint32_t pattern, VkDeviceSize lengthInBytes );

	// ---------------------------------------------------

	public:
		void	Clear( VkImage target, VkImageLayout layout, const VkClearDepthStencilValue& value, uint32_t rangeCount, const VkImageSubresourceRange* ranges );
		template <uint32_t clearCount>
		void	Clear( VkImage target, VkImageLayout layout, const VkClearDepthStencilValue& value, const VkImageSubresourceRange (&ranges)[clearCount] );
		void	Clear( VkImage target, VkImageLayout layout, const VkClearColorValue& value, uint32_t clearCount, const VkImageSubresourceRange* ranges );
		template <uint32_t clearCount>
		void	Clear( VkImage target, VkImageLayout layout, const VkClearColorValue& value, const VkImageSubresourceRange (&ranges)[clearCount] );

		void	Clear( uint32_t targetCount, const VkClearAttachment* targets, uint32_t regionCount, const VkClearRect* regions );
		template <uint32_t clearCount, uint32_t regionCount>
		void	Clear( const VkClearAttachment (&targets)[clearCount], const VkClearRect (&regions)[regionCount] );
		
	// ---------------------------------------------------

	public:
		void	SetEvent( VkEvent event, VkPipelineStageFlags mask );

		void	ResetEvent( VkEvent event, VkPipelineStageFlags mask );

		void	AwaitEvents(
			uint32_t waitCount, const VkEvent* waitEvents,
			VkPipelineStageFlags sourceStageMask,
			VkPipelineStageFlags targetStageMask,
			uint32_t barrierCount, const VkMemoryBarrier* memoryBarriers,
			uint32_t bufferBarrierCount, const VkBufferMemoryBarrier* bufferBarriers,
			uint32_t imageBarrierCount, const VkImageMemoryBarrier* imageBarriers
		);
		template <uint32_t waitCount, uint32_t barrierCount, uint32_t bufferBarrierCount, uint32_t imageBarrierCount>
		void	AwaitEvents(
			const VkEvent (&events)[waitCount],
			VkPipelineStageFlags sourceStageMask,
			VkPipelineStageFlags targetStageMask,
			const VkMemoryBarrier (&memoryBarriers)[barrierCount],
			const VkBufferMemoryBarrier (&bufferBarriers)[bufferBarrierCount],
			const VkImageMemoryBarrier (&imageBarriers)[imageBarrierCount]
		);
		void	AwaitEvents( uint32_t waitCount, const VkEvent* waitEvents, VkPipelineStageFlags sourceStageMask, VkPipelineStageFlags targetStageMask );
		template <uint32_t waitCount>
		void	AwaitEvents( const VkEvent (&waitEvents)[waitCount], VkPipelineStageFlags sourceStageMask, VkPipelineStageFlags targetStageMask );
		
		void	PipelineBarrier(
			VkDependencyFlags dependencyFlags,
			VkPipelineStageFlags sourceStageMask,
			VkPipelineStageFlags targetStageMask,
			uint32_t barrierCount, const VkMemoryBarrier* memoryBarriers,
			uint32_t bufferBarrierCount, const VkBufferMemoryBarrier* bufferBarriers,
			uint32_t imageBarrierCount, const VkImageMemoryBarrier* imageBarriers
		);
		template <uint32_t barrierCount, uint32_t bufferBarrierCount, uint32_t imageBarrierCount>
		void	PipelineBarrier(
			VkDependencyFlags dependencyFlags,
			VkPipelineStageFlags sourceStageMask,
			VkPipelineStageFlags targetStageMask,
			const VkMemoryBarrier (&memoryBarriers)[barrierCount],
			const VkBufferMemoryBarrier (&bufferBarriers)[bufferBarrierCount],
			const VkImageMemoryBarrier (&imageBarriers)[imageBarrierCount]
		);

	// ---------------------------------------------------

	public:
		void	BeginQuery( VkQueryPool pool, uint32_t query, VkQueryControlFlags flags );

		void	EndQuery( VkQueryPool pool, uint32_t query );

		void	CopyQueryPoolResults(
			VkQueryPool pool,
			uint32_t firstQuery,
			uint32_t count,
			VkBuffer target,
			VkDeviceSize offsetIntoTargetInBytes,
			VkDeviceSize targetStrideInBytes,
			VkQueryResultFlags flags
		);

		void	ResetQueryPool( VkQueryPool pool, uint32_t firstQuery, uint32_t count );

		void	WriteTimestamp( VkPipelineStageFlagBits stage, VkQueryPool pool, uint32_t query );

		void	PushConstants( VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t sizeInBytes, const void* values );

	// ---------------------------------------------------

	public:
		void	Execute( uint32_t bufferCount, const VkCommandBuffer* buffers );
		template <uint32_t bufferCount>
		void	Execute( const VkCommandBuffer (&buffers)[bufferCount] );

	// ---------------------------------------------------

	public:
		void	BeginPass( VkSubpassContents contents, const VkRenderPassBeginInfo& beginInfo );

		void	BeginNextSubpass( VkSubpassContents contents );

		void	FinishRenderPass();

	// ---------------------------------------------------

	//!	Disable copy assignment.
		CommandList&	operator=( const CommandList& ) = delete;

		friend void	Swap( CommandList&, CommandList& );

	// - DATA MEMBERS ------------------------------------

	protected:
		VkCommandPool	_pool;
		VkCommandBuffer	_commandBuffer;
	};

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/CommandList.inl>
//------------------------------------------------------------------//