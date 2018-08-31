/*==================================================================*\
  DialogueSet.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/DialogueSet.hpp>
//------------------------------------------------------------------//
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {

	DialogueResponse::DialogueResponse(WrenHandle* function) :
		_function(function) {}

	// ---------------------------------------------------

	bool DialogueResponse::Invoke(WrenVM* vm, WrenHandle* receiver, WrenHandle* unaryCallStub) const {
		wrenEnsureSlots(vm, 2);

		wrenSetSlotHandle(vm, 0, _function);
		wrenSetSlotHandle(vm, 1, receiver);

		return wrenCall(vm, unaryCallStub) == WREN_RESULT_SUCCESS;
	}

	// ---------------------------------------------------

	void Swap(DialogueSet& /*lhs*/, DialogueSet& /*rhs*/) {
	}

}}} // namespace Eldritch2::Scripting::Wren
