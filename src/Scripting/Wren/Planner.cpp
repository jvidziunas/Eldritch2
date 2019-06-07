/*==================================================================*\
  Planner.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/Planner.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {

	Tactic::Tactic(WrenHandle* action) ETNoexceptHint : _action(action) {}

	// ---------------------------------------------------

	Tactic::Tactic(Tactic&& tactic) ETNoexceptHint : _action(Exchange(tactic._action, nullptr)) {}

	// ---------------------------------------------------

	Tactic::~Tactic() {
		ETAssert(_action == nullptr, "Leaking Wren handle {}!", fmt::ptr(_action));
	}

	// ---------------------------------------------------

	Planner::Planner() : GoapPlanner(AllocatorType("Wren Planner Allocator")) {}

}}} // namespace Eldritch2::Scripting::Wren
