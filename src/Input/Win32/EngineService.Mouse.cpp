/*==================================================================*\
  EngineService.Mouse.cpp
  ------------------------------------------------------------------
  Purpose:
  Defines a Win32 application decorator object that invokes input
  system event objects when the user presses the corresponding key,
  moves a stick or shifts the mouse.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptApiRegistrationInitializationVisitor.hpp>
#include <Input/Win32/EngineService.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Input;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Input {
namespace Win32 {

	EngineService::Mouse::Mouse( const ::HANDLE deviceHandle, EngineService& owner ) {
		owner._deviceDirectory.Insert( { deviceHandle, this } );
	}

// ---------------------------------------------------

	void EngineService::Mouse::Dispose() {}

// ---------------------------------------------------

	void EngineService::Mouse::ReadInputPacket( const ::RAWINPUT& packet ) {
		const ::RAWMOUSE&	mouse( packet.data.mouse );

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

}	// namespace Win32
}	// namespace Input
}	// namespace Eldritch2