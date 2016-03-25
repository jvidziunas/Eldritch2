/*==================================================================*\
  SwapChain.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/SwapChain.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	SwapChain::SwapChain( Vulkan::UniquePointer<::VkSurfaceKHR>&& surface ) : _systemSurface( ::std::move( surface ) ) {}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2



