/*==================================================================*\
  Keyboard.cpp
  ------------------------------------------------------------------
  Purpose:
  Defines a Win32 application decorator object that invokes input
  system event objects when the user presses the corresponding key,
  moves a stick or shifts the mouse.

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Input/Win32/ScriptComponents/Keyboard.hpp>
#include <Utility/MPL/CharTypes.hpp>
#if !defined( WIN32_LEAN_AND_MEAN )
#	define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Input {
namespace Win32 {
namespace ScriptComponents {

	void Keyboard::ReadInputPacket( const RAWINPUT& /*inputPacket*/ ) {
	//	const RAWKEYBOARD&	keyboard( inputPacket.data.keyboard );
	//	const bool			isKeyUpEvent( !!(keyboard.Flags & RI_KEY_BREAK) );
	}

}	// namespace ScriptComponents
}	// namespace Win32
}	// namespace Input
}	// namespace Eldritch2