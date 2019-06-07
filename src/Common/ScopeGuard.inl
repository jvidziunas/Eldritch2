/*==================================================================*\
  ScopeGuard.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/Compiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Lambda>
ETConstexpr ETForceInlineHint ScopeGuard<Lambda>::ScopeGuard(Lambda onScopeExit) ETNoexceptHint : _onScopeExit(Move(onScopeExit)) {}

// ---------------------------------------------------

template <typename Lambda>
ETInlineHint ETForceInlineHint ScopeGuard<Lambda>::~ScopeGuard() {
	_onScopeExit();
}

// ---------------------------------------------------

template <typename Lambda>
ETConstexpr ScopeGuard<Lambda> AtScopeExit(Lambda lambda) ETNoexceptHint {
	return ScopeGuard<Lambda>(Move(lambda));
}

} // namespace Eldritch2
