/*==================================================================*\
  ScriptAPIRegistrationInitializationVisitor.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Angelscript/AngelscriptUserDefinedTypeRegistrar.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {

	namespace Scripting	= ::Eldritch2::Scripting;
	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	class ScriptAPIRegistrationInitializationVisitor : public Scripting::AngelscriptUserDefinedTypeRegistrar {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this ScriptAPIRegistrationInitializationVisitor instance.
		template <typename... Arguments>
		ETInlineHint ScriptAPIRegistrationInitializationVisitor( Arguments&&... arguments );

		// Destroys this ScriptAPIRegistrationInitializationVisitor instance.
		~ScriptAPIRegistrationInitializationVisitor();
	};

}	// namespace Scripting
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/ScriptAPIRegistrationInitializationVisitor.inl>
//------------------------------------------------------------------//
