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

	ETInlineHint ETForceInlineHint Span<InputDevice*> DeviceCoordinator::GetKeyboards() ETNoexceptHint {
		return _keyboards;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint Span<InputDevice*> DeviceCoordinator::GetMice() ETNoexceptHint {
		return _mice;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint Mutex& DeviceCoordinator::GetMutex() ETNoexceptHint {
		return _deviceMutex;
	}

}}} // namespace Eldritch2::Input::Win32
