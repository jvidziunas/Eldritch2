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

namespace Eldritch2 { namespace Graphics {
	namespace Vulkan {
		enum QueueConcept : uint32_t;
		class GraphicsPipeline;
		class Framebuffer;
		class Gpu;
	} // namespace Vulkan

	template <typename Vertex>
	class MeshSource;
	struct StaticVertex;
}} // namespace Eldritch2::Graphics

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	struct View {
		const MeshSource<StaticVertex>* maskGeometry;
		VkViewport                      affineViewport;
	};

	// ---

	class CommandList {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		CommandList(const CommandList&) = delete;
		//!	Constructs this @ref CommandList instance.
		CommandList(CommandList&&) ETNoexceptHint;
		//!	Constructs this @ref CommandList instance.
		CommandList() ETNoexceptHint;

		~CommandList();

		// ---------------------------------------------------

	public:
		ETConstexpr auto Get() const ETNoexceptHint -> const VkCommandBuffer (&)[1];

		// ---------------------------------------------------

	public:
		void ExecutePipeline(const Framebuffer& target, const GraphicsPipeline& pipeline, const ArrayList<View>& views);

		void CompositeImage(const Framebuffer& target, const Framebuffer& source);

		// ---------------------------------------------------

	public:
		VkResult BeginRecording(Gpu& gpu, VkCommandPoolResetFlags flags, VkCommandBufferUsageFlags usageFlags) ETNoexceptHint;

		VkResult FinishRecording() ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, QueueConcept concept, VkCommandPoolCreateFlags poolFlags);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		CommandList& operator=(const CommandList&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		VkCommandPool   _pool;
		VkCommandBuffer _commands[1];

		// ---------------------------------------------------

		friend void Swap(CommandList&, CommandList&) ETNoexceptHint;
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/CommandList.inl>
//------------------------------------------------------------------//
