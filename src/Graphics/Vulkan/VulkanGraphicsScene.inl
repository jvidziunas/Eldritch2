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

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	ETInlineHint Transformation PlayerView::GetWorldToView() const {
		return _worldToView;
	}

	// ---------------------------------------------------

	ETInlineHint void PlayerView::SetWorldToView(Transformation worldToView) {
		_worldToView = worldToView;
	}

	// ---------------------------------------------------

	ETInlineHint Angle PlayerView::GetVerticalFov() const {
		return _verticalFov;
	}

	// ---------------------------------------------------

	ETInlineHint void PlayerView::SetVerticalFov(Angle angle) {
		_verticalFov = angle;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
