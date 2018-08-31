/*==================================================================*\
  DialogueSet.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {

	ETInlineHint DialogueSet::DialogueSet() :
		RuleSet<DialogueResponse>(AllocatorType("Wren Dialogue Set Allocator")) {
	}

}}} // namespace Eldritch2::Scripting::Wren
