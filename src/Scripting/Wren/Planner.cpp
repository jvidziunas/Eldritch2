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

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	Action::Action( WrenHandle* action ) : _action( action ) {}

// ---------------------------------------------------

	Action::Action( Action&& action ) : _action( eastl::exchange( action._action, nullptr ) ) {}

// ---------------------------------------------------

	Action::~Action() {
		ET_ASSERT( _action == nullptr, "Leaking Wren handle!" );
	}

// ---------------------------------------------------

	Planner::Planner() : GoapPlanner( MallocAllocator( "Wren Planner Allocator" ) ) {}

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2