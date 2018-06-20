/*==================================================================*\
  ScopeGuard.hpp
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

#define STRING_JOIN(arg1, arg2) STRING_JOIN2(arg1, arg2)
#define STRING_JOIN2(arg1, arg2) arg1 ## arg2
#define ET_AT_SCOPE_EXIT(...) auto STRING_JOIN(scopeExit, __LINE__)( ::Eldritch2::AtScopeExit( [&] () -> void { __VA_ARGS__; } ) )

namespace Eldritch2 {

	template <typename Lambda>
	class ScopeGuard {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		ScopeGuard(const ScopeGuard&) = delete;
	//!	Constructs this @ref ScopeGuard instance.
		ScopeGuard(ScopeGuard&&);
	//!	Constructs this @ref ScopeGuard instance.
	/*!	@param[in] onScopeExit Code to be executed when this object leaves scope. */
		ScopeGuard(Lambda&& onScopeExit);

		~ScopeGuard();

	// - DATA MEMBERS ------------------------------------

	private:
		Lambda	_onScopeExit;
	};

// ---

	template <typename Lambda>
	ScopeGuard<Lambda> AtScopeExit(Lambda&& lambda);

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/ScopeGuard.inl>
//------------------------------------------------------------------//