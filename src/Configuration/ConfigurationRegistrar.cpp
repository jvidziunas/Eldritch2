/*==================================================================*\
  ConfigurationRegistrar.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Configuration/ConfigurationRegistrar.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Configuration {

	ConfigurationRegistrar::ConfigurationRegistrar( ConfigurationDatabase& database ) : _database( database ), _section( "" ) {}

// ---------------------------------------------------

	ConfigurationRegistrar& ConfigurationRegistrar::BeginSection( Utf8Literal sectionName ) {
		_section = sectionName;
		return *this;
	}

// ---------------------------------------------------

	ConfigurationRegistrar& ConfigurationRegistrar::Register( Utf8Literal name, const ConfigurationDatabase::PropertySetter& keyHandler ) {
		auto	insertResult( _database._propertySetters.Emplace( eastl::make_pair( _section, name ), keyHandler ) );

	//	If this is triggering, you're trying to register the same variable twice.
		ETRuntimeAssert( insertResult.second );

		return *this;
	}

// ---------------------------------------------------

	ConfigurationRegistrar& ConfigurationRegistrar::RegisterDynamicKeySetter( const ConfigurationDatabase::DynamicKeySetter& handler ) {
		auto	insertResult( _database._dynamicKeySetters.Emplace( _section, handler ) );

	//	If this is triggering, you're trying to register conflicting handlers.
		ETRuntimeAssert( insertResult.second );

		return *this;
	}

}	// namespace Configuration
}	// namespace Eldritch2