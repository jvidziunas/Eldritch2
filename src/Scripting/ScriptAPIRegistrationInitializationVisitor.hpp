/*==================================================================*\
  ScriptApiRegistrationInitializationVisitor.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Angelscript/UserDefinedTypeRegistrar.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {

	class ScriptApiRegistrationInitializationVisitor : public Scripting::AngelScript::UserDefinedTypeRegistrar {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ScriptApiRegistrationInitializationVisitor instance.
		/*!	@param[in] arguments Template parameter pack containing the arguments to be forwarded to the real type registrar constructor.
			*/
		template <typename... Arguments>
		ETInlineHint ScriptApiRegistrationInitializationVisitor( Arguments&&... arguments );

		~ScriptApiRegistrationInitializationVisitor() = default;
	};

}	// namespace Scripting
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/ScriptApiRegistrationInitializationVisitor.inl>
//------------------------------------------------------------------//
