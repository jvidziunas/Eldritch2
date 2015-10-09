/*==================================================================*\
  Keyboard.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
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

	namespace Scripting	= ::Eldritch2::Scripting;
	namespace Input		= ::Eldritch2::Input;

// ---------------------------------------------------

	class Keyboard : public Scripting::ReferenceTypeBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this Keyboard instance.
		Keyboard();

		// Destroys this Keyboard instance.
		~Keyboard();

	// - SCRIPT API REFERENCE ----------------------------

		// Registers all script-callable methods for the Keyboard type with the specified script type registrar.
		static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& typeRegistrar );

	// - DATA MEMBERS ------------------------------------

	public:
		static const char* const	scriptTypeName;

	protected:
		unsigned char	_keyStates[256];
	};

}	// namespace Input
}	// namespace Eldritch2