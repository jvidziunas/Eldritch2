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

	ETInlineHint DeviceLocator::DeviceLocator(const Mutex& mutex, ArrayList<InputDevice>& devices) :
		_mutex(eastl::addressof(mutex)),
		_devices(eastl::addressof(devices)) {
	}

	// ---------------------------------------------------

	ETInlineHint bool DeviceLocator::TryAcquireDevice(DeviceId id, InputDevice::BindingMap<> bindingByCode, InputHandler& handler) {
		ReadLock _(*_mutex);

		if (_devices->GetSize() <= id) {
			return false;
		}

		return (*_devices)[id].TryAcquire(eastl::move(bindingByCode), handler);
	}

	// ---------------------------------------------------

	ETInlineHint void DeviceLocator::ReleaseDevice(DeviceId id) {
		ReadLock _(*_mutex);

		if (_devices->GetSize() <= id) {
			return;
		}

		(*_devices)[id].Release();
	}

	// ---------------------------------------------------

	ETInlineHint size_t DeviceLocator::GetDeviceCount() const {
		ReadLock _(*_mutex);

		return _devices->GetSize();
	}

}} // namespace Eldritch2::Input
