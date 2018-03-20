/*==================================================================*\
  PropertyRegistrar.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/PropertyRegistrar.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Core {

	PropertyRegistrar::PropertyRegistrar( PropertyDatabase& database ) : _database( &database ), _section( "" ) {}

// ---------------------------------------------------

	PropertyRegistrar& PropertyRegistrar::BeginSection( Utf8Literal sectionName ) {
		_section = sectionName;
		return *this;
	}

// ---------------------------------------------------

	PropertyRegistrar& PropertyRegistrar::WhenPropertyChanged( Utf8Literal name, PropertyDatabase::Setter setter ) {
		auto	insertResult( _database->_settersByKey.Emplace( eastl::make_pair( _section, name ), eastl::move( setter ) ) );

		ET_ASSERT( insertResult.second, "Duplicate property handler registration!" );

		return *this;
	}

// ---------------------------------------------------

	PropertyRegistrar& PropertyRegistrar::WhenDynamicPropertyChanged( PropertyDatabase::DynamicSetter handler ) {
		auto	insertResult( _database->_dynamicSettersByGroup.Emplace( eastl::move( _section ), eastl::move( handler ) ) );

		ET_ASSERT( insertResult.second, "Duplicate dynamic property handler registration!" );

		return *this;
	}

}	// namespace Core
}	// namespace Eldritch2