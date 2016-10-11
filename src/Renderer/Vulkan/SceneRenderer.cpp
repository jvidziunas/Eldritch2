/*==================================================================*\
  SceneRenderer.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/SceneRenderer.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scheduling;

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	SceneRenderer::SceneRenderer( Allocator& allocator ) : _cameras( { allocator, "Vulkan Scene Renderer Camera Collection Allocator" } ) {}

// ---------------------------------------------------

	void SceneRenderer::BuildCommandBuffers( JobFiber& /*fiber*/, VkDevice /*device*/, VkCommandPool /*commandPool*/ ) const {}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2