/*==================================================================*\
  InputApi.cpp
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

	InputDevice::InputDevice() ETNoexceptHint : _actionByScanCode(MallocAllocator("Input Device Binding Map Allocator")),
												_handler(nullptr) {}

	// ---------------------------------------------------

	InputDevice::~InputDevice() {
		Release();
	}

	// ---------------------------------------------------

	bool InputDevice::TryAcquire(InputClient& handler, BindingMap actionByScanCode) {
		InputClient* expected(nullptr);
		if (!_handler.compare_exchange_strong(expected, ETAddressOf(handler), std::memory_order_release, std::memory_order_release)) {
			return false;
		}

		handler.OnConnect(*this);
		_actionByScanCode = Move(actionByScanCode);

		return true;
	}

	// ---------------------------------------------------

	void InputDevice::Release() ETNoexceptHint {
		if (InputClient* const handler = _handler.exchange(nullptr, std::memory_order_consume)) {
			handler->OnDisconnect(*this);
		}
	}

	// ---------------------------------------------------

	void InputDevice::Dispatch(ScanCode code, int32 weight) const {
		const auto binding(_actionByScanCode.Find(code));
		if (ET_UNLIKELY(binding == _actionByScanCode.End())) {
			return;
		}

		_handler.load(std::memory_order_consume)->Activate(binding->second.action, weight * binding->second.weight);
	}

}} // namespace Eldritch2::Input
