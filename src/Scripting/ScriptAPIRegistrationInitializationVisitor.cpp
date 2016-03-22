/*==================================================================*\
  ScriptApiRegistrationInitializationVisitor.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptAPIRegistrationInitializationVisitor.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {

	void ScriptApiRegistrationInitializationVisitor::DeclareEnumType( const char* typeName ) {
		this->RegisterEnumType( typeName );
	}

// ---------------------------------------------------

	void ScriptApiRegistrationInitializationVisitor::DeclareReferenceType( const char* typeName ) {
		this->RegisterReferenceType( typeName );
	}

// ---------------------------------------------------

	void ScriptApiRegistrationInitializationVisitor::DeclareValueType( const char* typeName, size_t objectSizeInBytes ) {
		this->RegisterValueType( typeName, objectSizeInBytes );
	}

// ---------------------------------------------------

	void ScriptApiRegistrationInitializationVisitor::CompleteEnumTypeRegistration( TypeToken /*token*/ ) {}

// ---------------------------------------------------

	void ScriptApiRegistrationInitializationVisitor::CompleteReferenceTypeRegistration( TypeToken /*token*/ ) {}

// ---------------------------------------------------

	void ScriptApiRegistrationInitializationVisitor::CompleteValueTypeRegistration( TypeToken /*token*/ ) {}

}	// namespace Scripting
}	// namespace Eldritch2



