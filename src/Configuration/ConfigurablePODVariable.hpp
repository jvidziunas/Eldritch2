/*==================================================================*\
  ConfigurablePODVariable.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Configuration/SimpleConfigurableVariableWithStorage.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Configuration {

	namespace Configuration	= ::Eldritch2::Configuration;

// ---------------------------------------------------

	template <typename ActualType>
	class ConfigurablePODVariable : public Configuration::SimpleConfigurableVariableWithStorage<ActualType> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref ConfigurablePODVariable instance.
		/*! @param[in] initialValue Value to initialize the actual POD type backing this configurable variable.
			*/
		ConfigurablePODVariable( const ActualType initialValue );

		//! Destroys this @ref ConfigurablePODVariable instance.
		~ConfigurablePODVariable() = default;

	// ---------------------------------------------------

		void	SetValue( const ::Eldritch2::Range<const ::Eldritch2::UTF8Char*>& string ) override;

		void	GetValue( ::Eldritch2::UTF8Char* destinationBuffer, size_t bufferOctetCount ) const override;
	};

// ---

	extern template class ConfigurablePODVariable<bool>;
	extern template class ConfigurablePODVariable<char>;
	extern template class ConfigurablePODVariable<signed char>;
	extern template class ConfigurablePODVariable<unsigned char>;
	extern template class ConfigurablePODVariable<short>;
	extern template class ConfigurablePODVariable<unsigned short>;
	extern template class ConfigurablePODVariable<int>;
	extern template class ConfigurablePODVariable<unsigned int>;
	extern template class ConfigurablePODVariable<long>;
	extern template class ConfigurablePODVariable<unsigned long>;
	extern template class ConfigurablePODVariable<float>;
	extern template class ConfigurablePODVariable<double>;

}	// namespace Configuration
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Configuration/ConfigurablePODVariable.inl>
//------------------------------------------------------------------//