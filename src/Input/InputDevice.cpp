/*==================================================================*\
  InputDevice.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Input/InputDevice.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Input {

	InputDevice::InputDevice() :
		_bindingByScanCode(MallocAllocator("Input Device Binding Map Allocator")),
		_handler(nullptr) {}

	// ---------------------------------------------------

	InputDevice::~InputDevice() {
		Release();
	}

	// ---------------------------------------------------

	bool InputDevice::TryAcquire(BindingMap<> bindingByScanCode, InputHandler& handler) {
		if (_handler != nullptr) {
			return false;
		}

		_bindingByScanCode = eastl::move(bindingByScanCode);
		_handler           = eastl::addressof(handler);

		handler.OnConnect();

		return true;
	}

	// ---------------------------------------------------

	void InputDevice::Release() {
		_bindingByScanCode.Clear();

		if (InputHandler* const handler = eastl::exchange(_handler, nullptr)) {
			handler->OnDisconnect();
		}
	}

	// ---------------------------------------------------

	void InputDevice::Dispatch(ScanCode code, int32 weight) const {
		const BindingMap<>::ConstIterator binding(_bindingByScanCode.Find(code));
		if (ET_UNLIKELY(binding == _bindingByScanCode.End())) {
			return;
		}

		_handler->Activate(binding->second.actionId, weight * binding->second.weight);
	}

}} // namespace Eldritch2::Input
