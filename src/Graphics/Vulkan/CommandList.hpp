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
	class GraphicsPipeline;
	class DescriptorTable;
	class Framebuffer;
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
		void ExecutePipeline(const GraphicsPipeline& pipeline, const DescriptorTable& descriptors, const Framebuffer& target);

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
		VkResult ResetPool(Gpu& gpu, VkCommandPoolResetFlags flags = 0);

		VkResult BeginRecording(VkCommandBufferUsageFlags flags);

		VkResult FinishRecording();

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, QueueConcept queue, VkCommandPoolCreateFlags flags);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		CommandList& operator=(const CommandList&) = delete;

		friend void Swap(CommandList&, CommandList&);

		// - DATA MEMBERS ------------------------------------

	private:
		VkCommandPool   _pool;
		VkCommandBuffer _commandBuffer;
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/CommandList.inl>
//------------------------------------------------------------------//
