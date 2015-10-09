/*==================================================================*\
  ConfigurationPublishingInitializationVisitor.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Configuration/ConfigurationPublishingInitializationVisitor.hpp>
#include <Configuration/ConfigurationDatabase.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Configuration {

	ConfigurationPublishingInitializationVisitor::ConfigurationPublishingInitializationVisitor( ConfigurationDatabase& database ) : _database( database ), _section( "" ) {}

// ---------------------------------------------------

	ConfigurationPublishingInitializationVisitor& ConfigurationPublishingInitializationVisitor::PushSection( const UTF8Char* const sectionName ) {
		_section = sectionName;
		return *this;
	}

// ---------------------------------------------------

	ConfigurationPublishingInitializationVisitor& ConfigurationPublishingInitializationVisitor::Register( const UTF8Char* const name, ConfigurableVariable& variable ) {
		_database._variableDirectory.Insert( ::rde::make_pair( ConfigurationDatabase::Key( _section, name ), &variable ) );

		return *this;
	}

}	// namespace Configuration
}	// namespace Eldritch2