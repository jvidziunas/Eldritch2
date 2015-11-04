/*==================================================================*\
  ConfigurableUTF8String.hpp
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
#include <Utility/Containers/UTF8String.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Configuration {

	class ConfigurableUTF8String : public Configuration::ConfigurableVariable, public ::Eldritch2::UTF8String<> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ConfigurableUTF8String instance.
		ConfigurableUTF8String( const ::Eldritch2::UTF8Char* const value, ::Eldritch2::Allocator& allocator );
		//!	Constructs this @ref ConfigurableUTF8String instance.
		ConfigurableUTF8String( ::Eldritch2::Allocator& allocator );

		//!	Destroys this @ref ConfigurableUTF8String instance.
		~ConfigurableUTF8String() = default;

	// ---------------------------------------------------
		
		void	SetValue( const ::Eldritch2::Range<const ::Eldritch2::UTF8Char*>& string ) override;

		void	GetValue( ::Eldritch2::UTF8Char* destinationBuffer, size_t bufferOctetCount ) const override;
	};

}	// namespace Configuration
}	// namespace Eldritch2