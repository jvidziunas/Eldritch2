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
#include <Configuration/ConfigurableVariable.hpp>
#include <Utility/Memory/StandardLibrary.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Configuration {

	ConfigurationDatabase::ConfigurationDatabase( Allocator& allocator ) : _variableDirectory( 128u, {}, { allocator, UTF8L( "Configuration Database Variable Directory Allocator" ) } ) {}

// ---------------------------------------------------

	void ConfigurationDatabase::SetValue( const Range<const UTF8Char*>& section, const Range<const UTF8Char*>& name, const Range<const UTF8Char*>& value ) {
		// Since the strings may not be null-terminated, create a temporary set of strings on the stack and use those instead.
		UTF8Char* const	terminatedSection( static_cast<UTF8Char*>(_alloca( section.Size() + name.Size() + 3u )) );
		UTF8Char* const	terminatedName( terminatedSection + section.Size() + 1u );

		CopyArray( terminatedSection, section.first, section.Size() );
		CopyArray( terminatedName, name.first, name.Size() );
		terminatedSection[section.Size()]	= UTF8L('\0');
		terminatedName[name.Size()]			= UTF8L('\0');
		
		const auto	variable( _variableDirectory.Find( Key( terminatedSection, terminatedName ) ) );

		if( variable != _variableDirectory.End() ) {
			variable->second->SetValue( value );
		}
	}

}	// namespace Configuration
}	// namespace Eldritch2