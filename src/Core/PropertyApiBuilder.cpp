/*==================================================================*\
  PropertyApiBuilder.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/PropertyApiBuilder.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Core {

	PropertyApiBuilder::PropertyApiBuilder() ETNoexceptHint : _properties() {}

	// ---------------------------------------------------

	PropertyApiBuilder& PropertyApiBuilder::DefineGroup(StringSpan name) ETNoexceptHint {
		_group = name;
		return *this;
	}

	// ---------------------------------------------------

	PropertyApiBuilder& PropertyApiBuilder::DefineProperty(StringSpan name, PropertyDatabase::Setter setter) {
		_properties._settersByKey.Emplace(PropertyDatabase::SetterKey(_group, name), Move(setter));
		return *this;
	}

	// ---------------------------------------------------

	PropertyApiBuilder& PropertyApiBuilder::DefineDynamicProperty(PropertyDatabase::DynamicSetter handler) {
		auto insertResult(_properties._dynamicSettersByGroup.Emplace(_group, Move(handler)));
		return *this;
	}

}} // namespace Eldritch2::Core
