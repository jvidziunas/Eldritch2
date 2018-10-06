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

	ETInlineHint ETForceInlineHint PlayerView::PlayerView(DisplayLink target, Transformation worldToView, Angle verticalFov) :
		_target(eastl::move(target)),
		_worldToView(worldToView),
		_verticalFov(verticalFov) {
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint Transformation PlayerView::GetWorldToView() const ETNoexceptHint {
		return _worldToView;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint void PlayerView::SetWorldToView(Transformation worldToView) ETNoexceptHint {
		_worldToView = worldToView;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint Angle PlayerView::GetVerticalFov() const ETNoexceptHint {
		return _verticalFov;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint void PlayerView::SetVerticalFov(Angle angle) ETNoexceptHint {
		_verticalFov = angle;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint const Framebuffer& PlayerView::GetTarget() const ETNoexceptHint {
		return *_target;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint Range<const PlayerView* const*> VulkanGraphicsScene::GetRootViews() const ETNoexceptHint {
		return { _rootViews.Begin(), _rootViews.End() };
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint const GraphicsPipeline& VulkanGraphicsScene::GetShadowPipeline() const ETNoexceptHint {
		return _shadowPipeline;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint const GraphicsPipeline& VulkanGraphicsScene::GetLitPipeline() const ETNoexceptHint {
		return _litPipeline;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint const Framebuffer& VulkanGraphicsScene::GetShadowAtlas() const ETNoexceptHint {
		return _shadowAtlas;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
