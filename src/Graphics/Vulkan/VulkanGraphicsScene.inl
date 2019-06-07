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

	ETInlineHint ETForceInlineHint RootView::RootView(ViewportId target, Transformation worldToView, Angle verticalFov) ETNoexceptHint : _target(target),
																																		 _worldToView(worldToView),
																																		 _verticalFov(verticalFov) {}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint Transformation RootView::GetWorldToView() const ETNoexceptHint {
		return _worldToView;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint void RootView::SetWorldToView(Transformation worldToView) ETNoexceptHint {
		_worldToView = worldToView;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint Angle RootView::GetVerticalFov() const ETNoexceptHint {
		return _verticalFov;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint void RootView::SetVerticalFov(Angle angle) ETNoexceptHint {
		_verticalFov = angle;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint const PipelineBuilder& VulkanGraphicsScene::GetShadowPipelineBuilder() const ETNoexceptHint {
		return _shadowMapPipelineBuilder;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint const PipelineBuilder& VulkanGraphicsScene::GetLitPipelineBuilder() const ETNoexceptHint {
		return _litPipelineBuilder;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint const GraphicsPipeline& VulkanGraphicsScene::GetShadowMapPipeline() const ETNoexceptHint {
		return _shadowMapPipeline;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint GraphicsPipeline& VulkanGraphicsScene::GetShadowMapPipeline() ETNoexceptHint {
		return _shadowMapPipeline;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint const GraphicsPipeline& VulkanGraphicsScene::GetLitPipeline() const ETNoexceptHint {
		return _litPipeline;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint GraphicsPipeline& VulkanGraphicsScene::GetLitPipeline() ETNoexceptHint {
		return _litPipeline;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint const Framebuffer& VulkanGraphicsScene::GetShadowAtlas() const ETNoexceptHint {
		return _shadowMapAtlas;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint Framebuffer& VulkanGraphicsScene::GetShadowAtlas() ETNoexceptHint {
		return _shadowMapAtlas;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
