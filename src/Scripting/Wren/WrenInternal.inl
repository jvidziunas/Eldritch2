/*==================================================================*\
  WrenInternal.inl
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

namespace Eldritch2 { namespace Scripting { namespace Wren {

	template <typename Type>
	ETInlineHint ETForceInlineHint ETPureFunctionHint Type& AsForeign(WrenHandle* objectHandle) ETNoexceptHint {
		return reinterpret_cast<Type*>(AS_FOREIGN(objectHandle->value)->data);
	}

}}} // namespace Eldritch2::Scripting::Wren
