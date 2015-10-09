/*==================================================================*\
  SimpleConfigurableVariableWithStorage.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2011 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Configuration {

	template <typename UnderlyingStorage>
	SimpleConfigurableVariableWithStorage<UnderlyingStorage>::SimpleConfigurableVariableWithStorage( const UnderlyingStorage initialValue ) : _value( initialValue ) {}

// ---------------------------------------------------

	template <typename UnderlyingStorage>
	ETForceInlineHint SimpleConfigurableVariableWithStorage<UnderlyingStorage>::operator UnderlyingStorage&() {
		return _value;
	}

// ---------------------------------------------------

	template <typename UnderlyingStorage>
	ETForceInlineHint SimpleConfigurableVariableWithStorage<UnderlyingStorage>::operator const UnderlyingStorage&() const {
		return _value;
	}

}	// namespace Configuration
}	// namespace Eldritch2