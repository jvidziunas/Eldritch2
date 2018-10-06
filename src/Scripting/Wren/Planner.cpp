/*==================================================================*\
  Planner.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/Planner.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {

	Tactic::Tactic(WrenHandle* action) ETNoexceptHint : _action(action) {}

	// ---------------------------------------------------

	Tactic::Tactic(Tactic&& action) ETNoexceptHint : _action(eastl::exchange(action._action, nullptr)) {}

	// ---------------------------------------------------

	Tactic::~Tactic() {
		ET_ASSERT(_action == nullptr, "Leaking Wren handle {}!", fmt::ptr(_action));
	}

	// ---------------------------------------------------

	Planner::Planner() :
		GoapPlanner(MallocAllocator("Wren Planner Allocator")) {}

}}} // namespace Eldritch2::Scripting::Wren
