/*==================================================================*\
  ApiRegistrar.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {

	template <typename Class>
	ETInlineHint void ApiRegistrar::DefineUserAllocatedClass( std::initializer_list<Scripting::Factory>		factories,
															  std::initializer_list<Scripting::Property>	properties,
															  std::initializer_list<Scripting::Method>		methods,
															  std::initializer_list<Scripting::Operator>	operators,
															  std::initializer_list<Scripting::Field>		fields ) {
		this->DefineUserAllocatedClass( typeid( Class ), factories, properties, methods, operators, fields );
	}

// ---------------------------------------------------

	template <typename Class>
	ETInlineHint void ApiRegistrar::DefineClass( std::initializer_list<Scripting::Constructor>	constructors,
												 std::initializer_list<Scripting::Property>		properties,
												 std::initializer_list<Scripting::Method>		methods,
												 std::initializer_list<Scripting::Operator>		operators,
												 std::initializer_list<Scripting::Field>		fields ) {
		this->DefineClass( typeid( Class ), sizeof( Class ), constructors, properties, methods, operators, fields );
	}

// ---------------------------------------------------

	template <typename Enum>
	ETInlineHint void ApiRegistrar::DefineEnum( std::initializer_list<Scripting::EnumValue> values ) {
		this->DefineEnum( typeid( Enum ), values );
	}

}	// namespace Scripting
}	// namespace Eldritch2

