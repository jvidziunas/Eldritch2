/*==================================================================*\
  VulkanGraphicsScene.PlayerView.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/VulkanGraphicsScene.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	PlayerView::PlayerView(
		Angle verticalFov) :
		_worldToView(Transformation::MakeIdentity()),
		_verticalFov(verticalFov) {
	}

	// ---------------------------------------------------

	VkResult PlayerView::UpdateResources(Gpu& gpu) {
		const VkExtent2D dimensions(GetOwnedRegion().extent);

		_framebuffer.UpdateDynamicScaling(gpu);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void PlayerView::FreeResources(Gpu& gpu) {
		_framebuffer.FreeResources(gpu);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
