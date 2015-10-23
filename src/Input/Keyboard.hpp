/*==================================================================*\
  Keyboard.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ReferenceTypeBase.hpp>
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	ScriptAPIRegistrationInitializationVisitor;
	}
}

namespace Eldritch2 {
namespace Input {

	class Keyboard : public Scripting::ReferenceTypeBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref Keyboard instance.
		Keyboard();

		//!	Destroys this @ref Keyboard instance.
		~Keyboard();

	// - SCRIPT API REFERENCE ----------------------------

		//!	Registers all script-callable methods for the @ref Keyboard type with the specified script type registrar.
		static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& typeRegistrar );

	// - DATA MEMBERS ------------------------------------

	public:
		static const char* const	scriptTypeName;

	protected:
		unsigned char	_keyStates[256];
	};

}	// namespace Input
}	// namespace Eldritch2