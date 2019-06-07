/*==================================================================*\
  InputApi.inl
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

namespace Eldritch2 { namespace Input {

	ETInlineHint ETForceInlineHint DeviceLocator::DeviceLocator() ETNoexceptHint : _mutex(nullptr) {}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool DeviceLocator::TryAcquireOpenDevice(DeviceId& outId, InputClient& handler, InputDevice::BindingMap bindingByCode) {
		ET_READ_LOCK_SCOPE(*_mutex);
		for (InputDevice& device : _devices) {
			if (device.TryAcquire(handler, Move(bindingByCode))) {
				outId = DeviceId(eastl::distance(_devices.Begin(), ETAddressOf(device)));
				return true;
			}
		}

		return false;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool DeviceLocator::TryAcquireDevice(DeviceId id, InputClient& handler, InputDevice::BindingMap bindingByCode) {
		ET_READ_LOCK_SCOPE(*_mutex);
		return eastl::next(_devices.Begin(), DeviceSpan::SizeType(id))->TryAcquire(handler, Move(bindingByCode));
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint DeviceLocator::DeviceSpan::SizeType DeviceLocator::GetDeviceCount() const ETNoexceptHint {
		return DeviceSpan::SizeType(_devices.GetSize());
	}

}} // namespace Eldritch2::Input
