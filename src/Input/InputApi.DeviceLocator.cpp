/*==================================================================*\
  InputApi.DeviceLocator.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Input/InputApi.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Input {

	Result DeviceLocator::BindResources(Mutex& mutex, Span<InputDevice*> devices) {
		_mutex   = ETAddressOf(mutex);
		_devices = devices;

		return Result::Success;
	}

	// ---------------------------------------------------

	void DeviceLocator::FreeResources() ETNoexceptHint {
		_mutex   = nullptr;
		_devices = Span<InputDevice*>();
	}

}} // namespace Eldritch2::Input
