/*==================================================================*\
  ConfigurableUTF8String.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Configuration/ConfigurableUTF8String.hpp>
#include <Utility/Memory/StandardLibrary.hpp>
#include <Utility/Math/StandardLibrary.hpp>
#include <Utility/Containers/Range.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace {

	static const UTF8Char	configurableStringAllocatorName[] = UTF8L("Configurable String Allocator");

}	// anonymous namespace

namespace Eldritch2 {
namespace Configuration {

	ConfigurableUTF8String::ConfigurableUTF8String( const UTF8Char* const value, Allocator& allocator ) : UTF8String<>( value, FindEndOfString( value ), { allocator, configurableStringAllocatorName } ) {}

// ---------------------------------------------------

	ConfigurableUTF8String::ConfigurableUTF8String( const EmptyStringSemantics semantics, Allocator& allocator ) : UTF8String<>( semantics, { allocator, configurableStringAllocatorName } ) {}

// ---------------------------------------------------
	
	void ConfigurableUTF8String::SetValue( const Range<const UTF8Char*>& string ) {
		Assign( string.first, static_cast<SizeType>(string.onePastLast - string.first) );
	}

// ---------------------------------------------------

	void ConfigurableUTF8String::GetValue( UTF8Char* destinationBuffer, size_t bufferOctetCount ) const {
		CopyString( destinationBuffer, GetCharacterArray(), bufferOctetCount );
	}

}	// namespace Configuration
}	// namespace Eldritch2