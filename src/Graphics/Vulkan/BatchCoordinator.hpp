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

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	class BatchCoordinator {
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
		VkResult BindResources(Gpu& gpu, VkDeviceSize parameterBufferSize, VkDeviceSize instanceBufferSize);

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
