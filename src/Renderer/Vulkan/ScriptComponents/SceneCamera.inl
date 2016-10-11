/*==================================================================*\
  SceneCamera.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {
namespace ScriptComponents {

	ETInlineHint SceneCamera::operator const Renderer::Camera&() const {
		return _camera;
	}

// ---------------------------------------------------

	ETInlineHint SceneCamera::operator Renderer::Camera&() {
		return _camera;
	}

}	// namespace ScriptComponents
}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

