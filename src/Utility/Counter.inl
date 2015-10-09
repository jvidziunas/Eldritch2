/*==================================================================*\
  Counter.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename UnderlyingPrimitiveType>
	ETForceInlineHint Counter<UnderlyingPrimitiveType>::Counter( UnderlyingPrimitiveType initialValue ) : _value( initialValue ) {}

// ---------------------------------------------------

	template <typename UnderlyingPrimitiveType>
	ETForceInlineHint UnderlyingPrimitiveType Counter<UnderlyingPrimitiveType>::operator+=( UnderlyingPrimitiveType value ) {
		return _value += value;
	}

// ---------------------------------------------------

	template <typename UnderlyingPrimitiveType>
	ETForceInlineHint UnderlyingPrimitiveType Counter<UnderlyingPrimitiveType>::operator++() {
		return ++_value;
	}

// ---------------------------------------------------

	template <typename UnderlyingPrimitiveType>
	ETForceInlineHint UnderlyingPrimitiveType Counter<UnderlyingPrimitiveType>::operator++( ::ETPostfixOperatorHint ) {
		return _value++;
	}

// ---------------------------------------------------

	template <typename UnderlyingPrimitiveType>
	ETForceInlineHint Counter<UnderlyingPrimitiveType>::operator UnderlyingPrimitiveType() const {
		return _value;
	}

}	// namespace Eldritch2