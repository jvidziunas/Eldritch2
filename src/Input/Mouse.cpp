/*==================================================================*\
  Mouse.cpp
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
#include <Input/Mouse.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Input;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Input {

	const char* const Mouse::scriptTypeName = "Mouse";

// ---------------------------------------------------

	Mouse::Mouse( uint32 buttonBits ) : _deltaX( 0 ), _deltaY( 0 ) {}

// ---------------------------------------------------

	ETNoAliasHint void Mouse::ExposeScriptAPI( ScriptApiRegistrationInitializationVisitor& typeRegistrar ) {}

}	// namespace Input
}	// namespace Eldritch2