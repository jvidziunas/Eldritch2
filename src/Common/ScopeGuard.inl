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
	ETInlineHint ScopeGuard<Lambda>::ScopeGuard( Lambda&& onScopeExit ) : _onScopeExit( eastl::move( onScopeExit ) ) {}

// ---------------------------------------------------

	template <typename Lambda>
	ETInlineHint ScopeGuard<Lambda>::ScopeGuard( ScopeGuard&& guard ) : _onScopeExit( eastl::move( guard._onScopeExit ) ) {}

// ---------------------------------------------------

	template <typename Lambda>
	ETInlineHint ScopeGuard<Lambda>::~ScopeGuard() {
		_onScopeExit();
	}

// ---------------------------------------------------

	template <typename Lambda>
	ScopeGuard<Lambda> AtScopeExit( Lambda&& lambda ) {
		return ScopeGuard<Lambda>( eastl::forward<Lambda>( lambda ) );
	}

}	// namespace Eldritch2