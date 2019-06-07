/*==================================================================*\
  EqualTo.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename T>
class EqualTo {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref EqualTo instance.
	EqualTo(const EqualTo&) ETNoexceptHint = default;
	//!	Constructs this @ref EqualTo instance.
	EqualTo() ETNoexceptHint = default;

	~EqualTo() = default;

	// ---------------------------------------------------

public:
	bool operator()(const T&, const T&) const ETNoexceptHint;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/EqualTo.inl>
//------------------------------------------------------------------//
