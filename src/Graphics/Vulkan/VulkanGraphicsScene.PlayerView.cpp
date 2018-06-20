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
		_scaler(),
		_localToWorld(Transformation::MakeIdentity()),
		_verticalFov(verticalFov) {
	}

	// ---------------------------------------------------

	VkExtent2D PlayerView::GetFramebufferExtent(VkExtent2D baseExtent) const {
		//	Resources must be sized according to the largest possible dynamic resolution.
		_scaler.ScaleMax(baseExtent.width, baseExtent.height);

		return baseExtent;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
