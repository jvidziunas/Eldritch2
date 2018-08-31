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
		_actionByScanCode(MallocAllocator("Input Device Binding Map Allocator")),
		_handler(nullptr) {}

	// ---------------------------------------------------

	InputDevice::~InputDevice() {
		Release();
	}

	// ---------------------------------------------------

	bool InputDevice::TryAcquire(BindingMap actionByScanCode, InputHandler& handler) {
		if (_handler != nullptr) {
			return false;
		}

		_actionByScanCode = eastl::move(actionByScanCode);
		_handler          = ETAddressOf(handler);

		handler.OnConnect();

		return true;
	}

	// ---------------------------------------------------

	void InputDevice::Release() {
		_actionByScanCode.Clear();

		if (InputHandler* const handler = eastl::exchange(_handler, nullptr)) {
			handler->OnDisconnect();
		}
	}

	// ---------------------------------------------------

	void InputDevice::Dispatch(ScanCode code, int32 weight) const {
		const auto binding(_actionByScanCode.Find(code));
		if (ET_UNLIKELY(binding == _actionByScanCode.End())) {
			return;
		}

		_handler->Activate(binding->second.actionId, weight * binding->second.weight);
	}

}} // namespace Eldritch2::Input
