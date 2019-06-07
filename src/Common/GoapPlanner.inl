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
ETInlineHint ETForceInlineHint GoapAgent<Action, Weight, Allocator>::GoapAgent(const AllocatorType& allocator) ETNoexceptHint : _actions(allocator) {}

// ---------------------------------------------------

template <typename Action, typename Weight, class Allocator>
ETInlineHint ETForceInlineHint GoapPlanner<Action, Weight, Allocator>::GoapPlanner(const AllocatorType& allocator) ETNoexceptHint : _tags(allocator) {}

} // namespace Eldritch2