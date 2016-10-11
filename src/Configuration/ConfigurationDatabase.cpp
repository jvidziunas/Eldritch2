/*==================================================================*\
  ConfigurationDatabase.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Configuration/ConfigurationDatabase.hpp>
#include <Utility/Containers/Utf8String.hpp>
#include <Utility/Containers/Range.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Configuration {

	ConfigurationDatabase::ConfigurationDatabase( const UnknownKeyHandler& unknownKeyHandler, Allocator& allocator ) : _propertySetters( { allocator, "Configuration Database Variable Directory Allocator" } ),
																													   _dynamicKeySetters( { allocator, "Configuration Database Unknown Key Handler Directory Allocator" } ),
																													   _unknownKeyHandler( unknownKeyHandler ) {}

// ---------------------------------------------------

	ConfigurationDatabase::ConfigurationDatabase( Allocator& allocator ) : ConfigurationDatabase( [] ( const Utf8Char* const, const Utf8Char* const, Range<const Utf8Char*> ) {}, allocator ) {}

// ---------------------------------------------------

	void ConfigurationDatabase::SetProperty( Range<const Utf8Char*> section, Range<const Utf8Char*> name, Range<const Utf8Char*> value ) {
	//	We'll need to create temporary copies of the section and name strings, as we don't own the memory pointed to by the arguments and the strings are not guaranteed to be null-terminated.
		const Utf8String<>	terminatedSection( section.Begin(), section.End(), { _propertySetters.GetAllocator().GetParent(), "ConfigurationDatabase::SetProperty() Section Allocator" } );
		const Utf8String<>	terminatedName( name.Begin(), name.End(), { _propertySetters.GetAllocator().GetParent(), "ConfigurationDatabase::SetProperty() Name Allocator" } );

	//	First, try to find a setter for the specific key/section pair we're working with.

	//	We're kind of cheating here, as the strings we're constructing are only pretending to be literals. However, they only need to persist to the end of the function call, so this gets a pass.
		const auto	candidate( _propertySetters.Find( { Utf8Literal( terminatedSection ), Utf8Literal( terminatedName ) } ) );
		if( candidate != _propertySetters.End() ) {
			candidate->second( value );
			return;
		}

	//	If there are no setters for this exact property, see if any clients have registered a dynamic key handler for the current section.
		const auto dynamicSetterCandidate( _dynamicKeySetters.Find( Utf8Literal( terminatedSection ) ) );
		if( dynamicSetterCandidate != _dynamicKeySetters.End() ) {
			dynamicSetterCandidate->second( Utf8Literal( terminatedName ), value );
			return;
		}

	//	Finally, invoke the unknown key handler if there are no suitable property setter candidates.
		_unknownKeyHandler( Utf8Literal( terminatedSection ), Utf8Literal( terminatedName ), value );
	}

}	// namespace Configuration
}	// namespace Eldritch2