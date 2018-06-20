/*==================================================================*\
  DeviceCoordinator.inl
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

namespace Eldritch2 { namespace Input { namespace Win32 {

	ETInlineHint ArrayList<InputDevice>& DeviceCoordinator::GetKeyboards() {
		return _keyboards;
	}

	// ---------------------------------------------------

	ETInlineHint ArrayList<InputDevice>& DeviceCoordinator::GetMice() {
		return _mice;
	}

	// ---------------------------------------------------

	ETInlineHint const Mutex& DeviceCoordinator::GetMutex() const {
		return _deviceMutex;
	}

}}} // namespace Eldritch2::Input::Win32
