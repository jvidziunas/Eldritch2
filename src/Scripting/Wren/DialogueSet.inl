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
		RuleSet<DialogueResponse>(AllocatorType("Wren Dialogue Set Allocator")),
		_asset(nullptr) {
	}

	// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint DialogueSet::MatchList<Allocator> DialogueSet::MatchAll(const Allocator& allocator, WrenHandle* facts) const {}

}}} // namespace Eldritch2::Scripting::Wren
