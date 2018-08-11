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

namespace Eldritch2 { namespace Core {

	PropertyRegistrar::PropertyRegistrar(PropertyDatabase& database) :
		_database(&database),
		_section("") {}

	// ---------------------------------------------------

	PropertyRegistrar& PropertyRegistrar::BeginSection(StringView<Utf8Char> sectionName) {
		_section = sectionName;
		return *this;
	}

	// ---------------------------------------------------

	PropertyRegistrar& PropertyRegistrar::DefineProperty(StringView<Utf8Char> name, PropertyDatabase::Setter setter) {
		auto insertResult(_database->_settersByKey.Emplace(eastl::make_pair(_section, name), eastl::move(setter)));
		return *this;
	}

	// ---------------------------------------------------

	PropertyRegistrar& PropertyRegistrar::DefineDynamicProperty(PropertyDatabase::DynamicSetter handler) {
		auto insertResult(_database->_dynamicSettersByGroup.Insert({ _section, eastl::move(handler) }));
		return *this;
	}

}} // namespace Eldritch2::Core
