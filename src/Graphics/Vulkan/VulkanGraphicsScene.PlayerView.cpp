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
#include <Graphics/Vulkan/OutputWindow.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	PlayerView::PlayerView(
		CountedPointer<OutputWindow> target,
		Transformation localToWorld,
		Angle verticalFov
	) : _target( eastl::move( target ) ),
		_scaler(),
		_localToWorld( localToWorld ),
		_verticalFov( verticalFov ) {
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2