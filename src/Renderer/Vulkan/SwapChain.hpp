/*==================================================================*\
  SwapChain.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/SmartPointers.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	class SwapChain {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		SwapChain( Vulkan::UniquePointer<::VkSurfaceKHR>&& surface );

		~SwapChain() = default;

	// - DATA MEMBERS ------------------------------------

	private:
		Vulkan::UniquePointer<::VkSurfaceKHR>	_systemSurface;
	};

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2