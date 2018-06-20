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
#include <vulkan/vulkan_core.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {
	enum QueueConcept : uint32_t;
	class Gpu;
}}} // namespace Eldritch2::Graphics::Vulkan

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class CommandList {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		CommandList(const CommandList&) = delete;
		//!	Constructs this @ref CommandList instance.
		CommandList(CommandList&&);
		//!	Constructs this @ref CommandList instance.
		CommandList();

		~CommandList();

		// ---------------------------------------------------

	public:
		operator VkCommandBuffer();

		VkCommandBuffer Get();

		// ---------------------------------------------------

	public:
		void Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);

		void DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);

		void DrawIndirect(VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t drawStrideInBytes = sizeof(VkDrawIndirectCommand));

		void DrawIndexedIndirect(VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t drawStrideInBytes = sizeof(VkDrawIndexedIndirectCommand));

		//!	Dispatch the currently-bound compute shader pipeline.
		/*!	The dispatch can be optionally split into two subgroups to avoid GPU cache and dispatch bubbles. */
		void Dispatch(VkEvent halfComplete, VkEvent fullComplete, uint32_t x, uint32_t y, uint32_t z);

		// ---------------------------------------------------

	public:
		//!	Bind mesh with indices.
		template <uint32_t count>
		void BindBuffers(const VkBuffer (&buffers)[count], const VkDeviceSize (&offsets)[count], VkBuffer indexBuffer, VkIndexType type);
		template <uint32_t count>
		void BindBuffers(const VkBuffer (&buffers)[count], VkBuffer indexBuffer, VkIndexType type);
		void BindBuffers(
			uint32_t            count,
			const VkBuffer*     buffers,
			const VkDeviceSize* offsets,
			VkBuffer            indexBuffer,
			VkIndexType         type);
		template <uint32_t count>
		void BindBuffers(const VkBuffer (&buffers)[count], const VkDeviceSize (&offsets)[count]);
		template <uint32_t count>
		void BindBuffers(const VkBuffer (&buffers)[count]);
		void BindBuffers(uint32_t count, const VkBuffer* buffers, const VkDeviceSize* offsets);

		// ---------------------------------------------------

	public:
		void SetPipeline(
			VkPipelineBindPoint target,
			VkPipeline          pipeline,
			VkPipelineLayout    layout,
			uint32_t setCount, const VkDescriptorSet* descriptorSets,
			uint32_t offsetCount, const uint32_t* offsets);

		// ---------------------------------------------------

	public:
		void SetViewports(uint32_t firstViewport, uint32_t viewportCount, const VkViewport* viewports);
		template <uint32_t viewportCount>
		void SetViewports(uint32_t firstViewport, const VkViewport (&viewports)[viewportCount]);

		void SetScissors(uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* scissors);
		template <uint32_t scissorCount>
		void SetScissors(uint32_t firstScissor, const VkRect2D (&scissors)[scissorCount]);

		void SetLineWidth(float width);

		void SetBlendConstants(const float (&constants)[4]);

		// ---------------------------------------------------

	public:
		void SetDepthBias(float constantFactor, float clamp, float slopeFactor);

		void SetDepthBounds(float min, float max);

		void SetStencilReadMask(VkStencilFaceFlags face, uint32_t mask);

		void SetStencilWriteMask(VkStencilFaceFlags face, uint32_t mask);

		void SetStencilReference(VkStencilFaceFlags face, uint32_t reference);

		// ---------------------------------------------------

	public:
		void Copy(VkImage target, VkImageLayout targetLayout, VkImage source, VkImageLayout sourceLayout, uint32_t regionCount, const VkImageBlit* regions, VkFilter filter);
		template <uint32_t count>
		void Copy(VkImage target, VkImageLayout targetLayout, VkImage source, VkImageLayout sourceLayout, const VkImageBlit (&regions)[count], VkFilter filter);
		void Copy(VkImage target, VkImageLayout targetLayout, VkImage source, VkImageLayout sourceLayout, uint32_t copyCount, const VkImageCopy* regions);
		template <uint32_t count>
		void Copy(VkImage target, VkImageLayout targetLayout, VkImage source, VkImageLayout sourceLayout, const VkImageCopy (&regions)[count]);
		void Copy(VkImage target, VkImageLayout targetLayout, VkBuffer source, uint32_t copyCount, const VkBufferImageCopy* regions);
		template <uint32_t count>
		void Copy(VkImage target, VkImageLayout targetLayout, VkBuffer source, const VkBufferImageCopy (&regions)[count]);
		void Copy(VkBuffer target, VkImage source, VkImageLayout sourceLayout, uint32_t copyCount, const VkBufferImageCopy* regions);
		template <uint32_t count>
		void Copy(VkBuffer target, VkImage source, VkImageLayout sourceLayout, const VkBufferImageCopy (&regions)[count]);
		void Copy(VkBuffer target, VkBuffer source, uint32_t copyCount, const VkBufferCopy* regions);
		template <uint32_t count>
		void Copy(VkBuffer target, VkBuffer source, const VkBufferCopy (&regions)[count]);

		void Resolve(VkImage target, VkImageLayout targetLayout, VkImage source, VkImageLayout sourceLayout, uint32_t resolveCount, const VkImageResolve* regions);
		template <uint32_t count>
		void Resolve(VkImage target, VkImageLayout targetLayout, VkImage source, VkImageLayout sourceLayout, const VkImageResolve (&regions)[count]);

		void UpdateBuffer(VkBuffer target, VkDeviceSize offsetInBytes, const void* sourceData, VkDeviceSize lengthInBytes);

		void FillBuffer(VkBuffer target, VkDeviceSize offsetInBytes, uint32_t pattern, VkDeviceSize lengthInBytes);

		// ---------------------------------------------------

	public:
		void Clear(VkImage target, VkImageLayout layout, const VkClearDepthStencilValue& value, uint32_t rangeCount, const VkImageSubresourceRange* ranges);
		template <uint32_t count>
		void Clear(VkImage target, VkImageLayout layout, const VkClearDepthStencilValue& value, const VkImageSubresourceRange (&ranges)[count]);
		void Clear(VkImage target, VkImageLayout layout, const VkClearColorValue& value, uint32_t clearCount, const VkImageSubresourceRange* ranges);
		template <uint32_t count>
		void Clear(VkImage target, VkImageLayout layout, const VkClearColorValue& value, const VkImageSubresourceRange (&ranges)[count]);
		void Clear(uint32_t targetCount, const VkClearAttachment* targets, uint32_t regionCount, const VkClearRect* regions);
		template <uint32_t count, uint32_t regionCount>
		void Clear(const VkClearAttachment (&targets)[count], const VkClearRect (&regions)[regionCount]);

		// ---------------------------------------------------

	public:
		void SetEvent(VkEvent event, VkPipelineStageFlags mask);

		void ResetEvent(VkEvent event, VkPipelineStageFlags mask);

		void AwaitEvents(
			uint32_t count, const VkEvent* events,
			VkPipelineStageFlags sourceStageMask,
			VkPipelineStageFlags targetStageMask,
			uint32_t barrierCount, const VkMemoryBarrier* barriers,
			uint32_t bufferBarrierCount, const VkBufferMemoryBarrier* bufferBarriers,
			uint32_t imageBarrierCount, const VkImageMemoryBarrier* imageBarriers);
		template <uint32_t count, uint32_t barrierCount, uint32_t bufferBarrierCount, uint32_t imageBarrierCount>
		void AwaitEvents(
			const VkEvent (&events)[count],
			VkPipelineStageFlags sourceStageMask,
			VkPipelineStageFlags targetStageMask,
			const VkMemoryBarrier (&barriers)[barrierCount],
			const VkBufferMemoryBarrier (&bufferBarriers)[bufferBarrierCount],
			const VkImageMemoryBarrier (&imageBarriers)[imageBarrierCount]);
		void AwaitEvents(uint32_t count, const VkEvent* events, VkPipelineStageFlags sourceStageMask, VkPipelineStageFlags targetStageMask);
		template <uint32_t count>
		void AwaitEvents(const VkEvent (&events)[count], VkPipelineStageFlags sourceStageMask, VkPipelineStageFlags targetStageMask);

		void PipelineBarrier(
			VkDependencyFlags    dependencyFlags,
			VkPipelineStageFlags sourceStageMask,
			VkPipelineStageFlags targetStageMask,
			uint32_t count, const VkMemoryBarrier* barriers,
			uint32_t bufferBarrierCount, const VkBufferMemoryBarrier* bufferBarriers,
			uint32_t imageBarrierCount, const VkImageMemoryBarrier* imageBarriers);
		template <uint32_t count, uint32_t bufferBarrierCount, uint32_t imageBarrierCount>
		void PipelineBarrier(
			VkDependencyFlags    dependencyFlags,
			VkPipelineStageFlags sourceStageMask,
			VkPipelineStageFlags targetStageMask,
			const VkMemoryBarrier (&barriers)[count],
			const VkBufferMemoryBarrier (&bufferBarriers)[bufferBarrierCount],
			const VkImageMemoryBarrier (&imageBarriers)[imageBarrierCount]);

		// ---------------------------------------------------

	public:
		void BeginQuery(VkQueryPool pool, uint32_t query, VkQueryControlFlags flags);

		void EndQuery(VkQueryPool pool, uint32_t query);

		void CopyQueryPoolResults(
			VkQueryPool        pool,
			uint32_t           firstQuery,
			uint32_t           count,
			VkBuffer           target,
			VkDeviceSize       offsetIntoTargetInBytes,
			VkDeviceSize       targetStrideInBytes,
			VkQueryResultFlags flags);

		void ResetQueryPool(VkQueryPool pool, uint32_t firstQuery, uint32_t count);

		void WriteTimestamp(VkPipelineStageFlagBits stage, VkQueryPool pool, uint32_t query);

		void PushConstants(VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t sizeInBytes, const void* values);

		// ---------------------------------------------------

	public:
		void Execute(uint32_t count, const VkCommandBuffer* buffers);
		template <uint32_t count>
		void Execute(const VkCommandBuffer (&buffers)[count]);

		// ---------------------------------------------------

	public:
		void BeginRendering(VkSubpassContents contents, const VkRenderPassBeginInfo& beginInfo);

		void AdvanceSubpass(VkSubpassContents contents);

		void FinishRendering();

		// ---------------------------------------------------

	public:
		VkResult ResetPool(Gpu& gpu, VkCommandPoolResetFlags flags = 0);

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, QueueConcept queue, VkCommandPoolCreateFlags flags);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

	public:
		VkResult BeginRecording(VkCommandBufferUsageFlags flags);

		VkResult FinishRecording();

		// ---------------------------------------------------

		//!	Disable copy assignment.
		CommandList& operator=(const CommandList&) = delete;

		friend void Swap(CommandList&, CommandList&);

		// - DATA MEMBERS ------------------------------------

	protected:
		VkCommandPool   _pool;
		VkCommandBuffer _commandBuffer;
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/CommandList.inl>
//------------------------------------------------------------------//
