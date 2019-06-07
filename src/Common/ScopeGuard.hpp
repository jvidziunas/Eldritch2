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

#define ET_AT_SCOPE_EXIT(...) auto ET_CPP_JOIN(scopeExit, __LINE__)(::Eldritch2::AtScopeExit([&]() -> void { __VA_ARGS__; }))
#define ET_FOREACH_AT_SCOPE_EXIT(element, aggregate, ...) auto ET_CPP_JOIN(scopeExit, __LINE__)(::Eldritch2::AtScopeExit([&]() -> void { for( auto& element : aggregate ) { __VA_ARGS__; } }))

namespace Eldritch2 {

template <typename Lambda>
class ScopeGuard {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref ScopeGuard instance.
	/*!	@param[in] onScopeExit Code to be executed when this object leaves scope. */
	ETConstexpr ScopeGuard(Lambda onScopeExit) ETNoexceptHint;
	//!	Constructs this @ref ScopeGuard instance.
	ETConstexpr ScopeGuard(ScopeGuard&&) ETNoexceptHint = default;
	//!	Disable copy construction.
	ScopeGuard(const ScopeGuard&) = delete;
	//!	Disable default construction.
	ScopeGuard() = delete;

	~ScopeGuard();

	// - DATA MEMBERS ------------------------------------

private:
	Lambda _onScopeExit;
};

// ---

template <typename Lambda>
ETConstexpr ScopeGuard<Lambda> AtScopeExit(Lambda lambda) ETNoexceptHint;

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/ScopeGuard.inl>
//------------------------------------------------------------------//
