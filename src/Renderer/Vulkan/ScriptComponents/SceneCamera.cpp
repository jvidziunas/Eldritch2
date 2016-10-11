/*==================================================================*\
  SceneCamera.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/ScriptComponents/SceneCamera.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Core;

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {
namespace ScriptComponents {

	SceneCamera::SceneCamera( const PlatformWindowSwapChain& swapChain, RigidTransform transform, RadianMeasure horizontalFovAngle, float32 aspectRatio ) : Camera( transform, horizontalFovAngle, aspectRatio ), _swapChain( const_cast<PlatformWindowSwapChain*>(&swapChain) ) {}

// ---------------------------------------------------

	VkViewport SceneCamera::GetViewport() const {
		return { 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };
	}

}	// namespace ScriptComponents
}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2