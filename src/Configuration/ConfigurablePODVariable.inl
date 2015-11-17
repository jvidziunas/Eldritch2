/*==================================================================*\
  ConfigurablePODVariable.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2011 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/Range.hpp>
//------------------------------------------------------------------//
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Configuration {

	template <typename ActualType>
	ConfigurablePODVariable<ActualType>::ConfigurablePODVariable( const ActualType initialValue ) : SimpleConfigurableVariableWithStorage( initialValue ) {}

// ---------------------------------------------------

	template <typename ActualType>
	void ConfigurablePODVariable<ActualType>::SetValue( const ::Eldritch2::Range<const ::Eldritch2::UTF8Char*>& string ) {
		using namespace ::boost::iostreams;

	// ---

		basic_array_source<::Eldritch2::UTF8Char>	source( string.first, string.onePastLast );
		stream<decltype(source)>					inStream( source );

		inStream >> _value;
	}

// ---------------------------------------------------

	template <typename ActualType>
	void ConfigurablePODVariable<ActualType>::GetValue( ::Eldritch2::UTF8Char* destinationBuffer, size_t destinationBufferSizeInOctets ) const {
		using namespace ::boost::iostreams;

	// ---

		basic_array_sink<::Eldritch2::UTF8Char>	sink( destinationBuffer, destinationBufferSizeInOctets );
		stream<decltype(sink)>					outStream( sink );

		outStream << _value;
	}

}	// namespace Configuration
}	// namespace Eldritch2

#if( ET_COMPILER_IS_MSVC )
//	Disable warnings about dead code elimination.
#	pragma warning( disable : 4505 )
#endif