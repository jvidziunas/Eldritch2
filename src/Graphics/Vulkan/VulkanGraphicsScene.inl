/*==================================================================*\
  VulkanGraphicsScene.inl
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
namespace Graphics {
namespace Vulkan {

	ETInlineHint Transformation PlayerView::GetLocalToWorld() const {
		return _localToWorld;
	}

// ---------------------------------------------------

	ETInlineHint void PlayerView::SetLocalToWorld( Transformation localToWorld ) {
		_localToWorld = localToWorld;
	}

// ---------------------------------------------------

	ETInlineHint Angle PlayerView::GetVerticalFov() const {
		return _verticalFov;
	}

// ---------------------------------------------------

	ETInlineHint void PlayerView::SetVerticalFov( Angle angle ) {
		_verticalFov = angle;
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2