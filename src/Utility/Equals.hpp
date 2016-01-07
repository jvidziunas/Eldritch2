/*==================================================================*\
  Equals.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename T>
	class Equals {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref Equals instance.
		Equals() = default;

		~Equals() = default;

	// ---------------------------------------------------

		template <typename U = T>
		ETInlineHint bool	operator()( const T& object0, const U& object1 ) const;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Equals.inl>
//------------------------------------------------------------------//