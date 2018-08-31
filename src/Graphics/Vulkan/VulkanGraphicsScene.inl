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

	ETInlineHint ETForceInlineHint PlayerView::PlayerView(ViewportReference target, Transformation worldToView, Angle verticalFov) :
		_target(eastl::move(target)),
		_worldToView(worldToView),
		_verticalFov(verticalFov) {
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint Transformation PlayerView::GetWorldToView() const ETNoexceptHint {
		return _worldToView;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void PlayerView::SetWorldToView(Transformation worldToView) ETNoexceptHint {
		_worldToView = worldToView;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint Angle PlayerView::GetVerticalFov() const ETNoexceptHint {
		return _verticalFov;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void PlayerView::SetVerticalFov(Angle angle) ETNoexceptHint {
		_verticalFov = angle;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint const Framebuffer& PlayerView::GetTarget() const ETNoexceptHint {
		return *_target;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint const ArrayList<PlayerView*> VulkanGraphicsScene::GetRootViews() const ETNoexceptHint {
		return _rootViews;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint const BatchCoordinator& VulkanGraphicsScene::GetOpaqueBatches() const ETNoexceptHint {
		return _opaqueBatches;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint const GraphicsPipeline& VulkanGraphicsScene::GetShadowPipeline() const ETNoexceptHint {
		return _shadowPipeline;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint const DescriptorTable& VulkanGraphicsScene::GetShaderResources() const ETNoexceptHint {
		return _shaderResources;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint const GraphicsPipeline& VulkanGraphicsScene::GetOpaqueLitPipeline() const ETNoexceptHint {
		return _opaqueLitPipeline;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint const Framebuffer& VulkanGraphicsScene::GetShadowAtlas() const ETNoexceptHint {
		return _shadowAtlas;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
