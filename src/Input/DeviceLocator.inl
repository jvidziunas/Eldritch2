/*==================================================================*\
  InputBus.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Input {

	ETInlineHint ETForceInlineHint DeviceLocator::DeviceLocator(const Mutex& mutex, ArrayList<InputDevice>& devices) :
		_mutex(ETAddressOf(mutex)),
		_devices(ETAddressOf(devices)) {
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool DeviceLocator::TryAcquireDevice(DeviceId id, InputClient& handler, InputDevice::BindingMap bindingByCode) {
		ReadLock _(*_mutex);
		if (id >= _devices->GetSize()) {
			return false;
		}

		return (*_devices)[id].TryAcquire(handler, eastl::move(bindingByCode));
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool DeviceLocator::TryAcquireDevice(InputClient& handler, InputDevice::BindingMap bindingByCode) {
		ReadLock _(*_mutex);
		for (InputDevice& device : *_devices) {
			if (device.TryAcquire(handler, eastl::move(bindingByCode))) {
				return true;
			}
		}

		return false;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint size_t DeviceLocator::GetDeviceCount() const {
		ReadLock _(*_mutex);
		return _devices->GetSize();
	}

}} // namespace Eldritch2::Input
