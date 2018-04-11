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

namespace Eldritch2 {
namespace Input {

	InputDevice::InputDevice() : _bindingByCode( MallocAllocator( "Input Device Binding Map Allocator" ) ), _actionHandler() {}

// ---------------------------------------------------

	bool InputDevice::TryAcquire( BindingMap<> bindingByCode, Handler actionHandler ) {
		_bindingByCode = eastl::move( bindingByCode );
		_actionHandler = eastl::move( actionHandler );

		return true;
	}

// ---------------------------------------------------

	void InputDevice::Clear() {
		_bindingByCode.Clear();
	//	Don't need to clear handler, we have no entries and will never dispatch (see @ref InputDevice::PressButton)
	}

// ---------------------------------------------------

	void InputDevice::MapCodeAndDispatch( ScanCode code, int32 weight ) const {
		const BindingMap<>::ConstIterator	binding( _bindingByCode.Find( code ) );
		if (ET_UNLIKELY( binding == _bindingByCode.End() )) {
			return;
		}

		_actionHandler( binding->second.actionId, weight * binding->second.weight );
	}

}	// namespace Input
}	// namespace Eldritch2