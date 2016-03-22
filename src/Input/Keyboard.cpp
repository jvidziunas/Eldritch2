/*==================================================================*\
  Keyboard.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptApiRegistrationInitializationVisitor.hpp>
#include <Utility/ErrorCode.hpp>
#include <Input/Keyboard.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Input;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Input {

	const char* const Keyboard::scriptTypeName = "Keyboard";

// ---------------------------------------------------

	ETNoAliasHint void Keyboard::ExposeScriptAPI( ScriptApiRegistrationInitializationVisitor& /*typeRegistrar*/ ) {}

}	// namespace Input
}	// namespace Eldritch2