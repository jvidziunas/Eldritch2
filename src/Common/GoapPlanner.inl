/*==================================================================*\
  GoapPlanner.inl
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

namespace Eldritch2 {

	template <typename Action, typename Weight, class Allocator>
	GoapAgent<Action, Weight, Allocator>::GoapAgent( const AllocatorType& allocator ) : _actions( allocator ) {}

// ---------------------------------------------------

	template <typename Action, typename Weight, class Allocator>
	GoapPlanner<Action, Weight, Allocator>::GoapPlanner(
		const AllocatorType& allocator
	) : _allocator( allocator ),
		_stateTags( ChildAllocator( _allocator, "GOAP Planner State Tag Allocator" ) ) {}

}	// namespace Eldritch2