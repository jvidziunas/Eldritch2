/*==================================================================*\
  UiRenderer.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/UiRenderer.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scheduling;

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	UiRenderer::UiRenderer() : Renderer::UiRenderer() {}

// ---------------------------------------------------

	void UiRenderer::BuildCommandBuffers( JobFiber& /*fiber*/, VkDevice /*device*/, VkCommandPool /*commandPool*/ ) const {

	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2