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
#include <Utility/Containers/Range.hpp>
#include <Utility/Memory/MemStdLib.hpp>
#include <Utility/Math/MathUtils.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace {

	static const UTF8Char	configurableStringAllocatorName[] = UTF8L("Configurable String Allocator");

}	// anonymous namespace

namespace Eldritch2 {
namespace Configuration {

	ConfigurableUTF8String::ConfigurableUTF8String( const UTF8Char* const value, Allocator& allocator ) : UTF8String<>( value, ::Eldritch2::StringLength( value ), allocator, configurableStringAllocatorName ) {}

// ---------------------------------------------------

	ConfigurableUTF8String::ConfigurableUTF8String( const EmptyStringSemantics semantics, Allocator& allocator ) : UTF8String<>( semantics, allocator, configurableStringAllocatorName ) {}

// ---------------------------------------------------
	
	void ConfigurableUTF8String::SetValue( const Range<const UTF8Char*>& string ) {
		Assign( string.first, string.onePastLast - string.first );
	}

// ---------------------------------------------------

	void ConfigurableUTF8String::GetValue( UTF8Char* destinationBuffer, size_t bufferOctetCount ) const {
		CopyArray( destinationBuffer, GetCharacterArray(), Min<size_t>( Length(), bufferOctetCount ) );

		// Ensure the string is null-terminated.
		destinationBuffer[bufferOctetCount - 1u] = UTF8L('\0');
	}

}	// namespace Configuration
}	// namespace Eldritch2