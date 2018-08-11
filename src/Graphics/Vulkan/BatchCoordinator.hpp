/*==================================================================*\
  BatchCoordinator.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GpuResources.hpp>
//------------------------------------------------------------------//
#include <vulkan/vulkan_core.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {
	class CommandList;
}}} // namespace Eldritch2::Graphics::Vulkan

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	struct View {
		//!  Set of planes to clip objects against.
		Vector   frustumPlanes[6];
		VkRect2D scissorRect;
		uint8    id;
	};

	// ---

	class BatchCoordinator {
		// - TYPE PUBLISHING ---------------------------------

	public:
		enum : VkDeviceSize {
			InstanceBufferSize  = 2u * 1024u * 1024u, /*  2MB */
			ParameterBufferSize = 16u * 1024u         /* 16KB */
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		BatchCoordinator(const BatchCoordinator&) = delete;
		//!	Constructs this @ref BatchCoordinator instance.
		BatchCoordinator(BatchCoordinator&&);
		//!	Constructs this @ref BatchCoordinator instance.
		BatchCoordinator();

		~BatchCoordinator() = default;

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

	public:
		void AddThing(void* thing);

		void ResetCounts();

		// ---------------------------------------------------

		//!	Disable copy assignment.
		BatchCoordinator& operator=(const BatchCoordinator&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		//!	Container for draw call parameters to be passed to the GPU.
		UniformBuffer           _drawParameters;
		ArrayMap<void*, uint32> _countsByThing;

		// ---------------------------------------------------

		friend void Swap(BatchCoordinator&, BatchCoordinator&);
	};

}}} // namespace Eldritch2::Graphics::Vulkan
