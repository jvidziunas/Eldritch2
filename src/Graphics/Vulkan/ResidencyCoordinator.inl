/*==================================================================*\
  ResidencyCoordinator.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	ETInlineHint const GpuResourceBus& ResidencyCoordinator::GetBus() const {
		return _bus;
	}

	// ---------------------------------------------------

	ETInlineHint GpuResourceBus& ResidencyCoordinator::GetBus() {
		return _bus;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
