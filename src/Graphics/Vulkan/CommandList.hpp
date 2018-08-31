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

	struct View {
		//!  Set of planes to clip objects against.
		Vector     frustumPlanes[6];
		VkViewport affineViewport;
		uint8      id;
	};

	// ---

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
		operator VkCommandBuffer() ETNoexceptHint;

		VkCommandBuffer Get() ETNoexceptHint;

		// ---------------------------------------------------

	public:
		void ExecutePipeline(const Framebuffer& target, const GraphicsPipeline& pipeline, const DescriptorTable& descriptors, const ArrayList<View>& views);

		void CompositeImage(const Framebuffer& target, const Framebuffer& source);

		// ---------------------------------------------------

	public:
		VkResult BeginRecording(Gpu& gpu, VkCommandPoolResetFlags flags, VkCommandBufferUsageFlags usageFlags) ETNoexceptHint;

		VkResult FinishRecording() ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, QueueConcept concept, VkCommandPoolCreateFlags flags);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

	private:
		//!	Dispatch the currently-bound compute shader pipeline.
		/*!	The dispatch can be optionally split into two subgroups to avoid GPU cache and dispatch bubbles. */
		void Dispatch(VkEvent halfComplete, VkEvent complete, uint32_t x, uint32_t y, uint32_t z) ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		CommandList& operator=(const CommandList&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		VkCommandPool   _pool;
		VkCommandBuffer _commands;

		// ---------------------------------------------------

		friend void Swap(CommandList&, CommandList&) ETNoexceptHint;
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/CommandList.inl>
//------------------------------------------------------------------//
