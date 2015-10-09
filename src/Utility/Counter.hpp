/*==================================================================*\
  Counter.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Compiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename UnderlyingPrimitiveType>
	class Counter {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		ETForceInlineHint Counter( UnderlyingPrimitiveType initialValue = static_cast<UnderlyingPrimitiveType>( 0 ) );

	// - ADDITION OPERATORS ------------------------------

		ETForceInlineHint UnderlyingPrimitiveType	operator+=( UnderlyingPrimitiveType value );

		ETForceInlineHint UnderlyingPrimitiveType	operator++();
		ETForceInlineHint UnderlyingPrimitiveType	operator++( ::ETPostfixOperatorHint );

	// - TYPE CASTING ------------------------------------

		ETForceInlineHint operator UnderlyingPrimitiveType() const;

	// - DATA MEMBERS ------------------------------------

	private:
		UnderlyingPrimitiveType	_value;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Counter.inl>
//------------------------------------------------------------------//