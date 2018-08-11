/*==================================================================*\
  ResourceBus.inl
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

	ETInlineHint const FrameTransferBus& ResourceBus::GetTransferBus() const {
		return _bus;
	}

	// ---------------------------------------------------

	ETInlineHint FrameTransferBus& ResourceBus::GetTransferBus() {
		return _bus;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
