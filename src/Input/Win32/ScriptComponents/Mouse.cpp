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
#include <Input/Win32/ScriptComponents/Mouse.hpp>
#if !defined( WIN32_LEAN_AND_MEAN )
#	define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Input {
namespace Win32 {
namespace ScriptComponents {

	void Mouse::ReadInputPacket( const RAWINPUT& packet ) {
		const RAWMOUSE&	mouse( packet.data.mouse );

		if( mouse.usFlags & MOUSE_MOVE_ABSOLUTE ) {
		} else {
		}

		if( mouse.usButtonFlags & RI_MOUSE_BUTTON_1_DOWN ) {}

		if( mouse.usButtonFlags & RI_MOUSE_BUTTON_1_UP ) {}

		if( mouse.usButtonFlags & RI_MOUSE_BUTTON_2_DOWN ) {}

		if( mouse.usButtonFlags & RI_MOUSE_BUTTON_2_UP ) {}

		if( mouse.usButtonFlags & RI_MOUSE_BUTTON_3_DOWN ) {}

		if( mouse.usButtonFlags & RI_MOUSE_BUTTON_3_UP ) {}

		if( mouse.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN ) {}

		if( mouse.usButtonFlags & RI_MOUSE_BUTTON_4_UP ) {}

		if( mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN ) {}

		if( mouse.usButtonFlags & RI_MOUSE_BUTTON_5_UP ) {}

		if( mouse.usButtonFlags & RI_MOUSE_WHEEL ) {}
	}

}	// namespace ScriptComponents
}	// namespace Win32
}	// namespace Input
}	// namespace Eldritch2