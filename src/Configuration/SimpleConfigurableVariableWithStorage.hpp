/*==================================================================*\
  SimpleConfigurableVariableWithStorage.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Configuration/ConfigurableVariable.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Configuration {

	template <typename UnderlyingStorage>
	class SimpleConfigurableVariableWithStorage : public Configuration::ConfigurableVariable {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref SimpleConfigurableVariableWithStorage instance.
		SimpleConfigurableVariableWithStorage( const UnderlyingStorage initialValue );

		//! Destroys this @ref SimpleConfigurableVariableWithStorage instance.
		~SimpleConfigurableVariableWithStorage() = default;

	// ---------------------------------------------------

		ETForceInlineHint	operator UnderlyingStorage&();
		ETForceInlineHint	operator const UnderlyingStorage&() const;

	// - DATA MEMBERS ------------------------------------

	protected:
		UnderlyingStorage	_value;
	};

}	// namespace Configuration
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Configuration/SimpleConfigurableVariableWithStorage.inl>
//------------------------------------------------------------------//