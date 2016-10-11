/*==================================================================*\
  Hash.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Compiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename T>
	class Hash {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Hash instance.
		Hash( const Hash& ) = default;
	//!	Constructs this @ref Hash instance.
		Hash() = default;

		~Hash() = default;

	// ---------------------------------------------------

		ETPureFunctionHint size_t	operator()( const T& object, size_t seed = 0u ) const;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Hash.inl>
//------------------------------------------------------------------//