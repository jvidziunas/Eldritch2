/*==================================================================*\
  Win32InputService.Mouse.cpp
  ------------------------------------------------------------------
  Purpose:
  Defines a Win32 application decorator object that invokes input
  system event objects when the user presses the corresponding key,
  moves a stick or shifts the mouse.

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptAPIRegistrationInitializationVisitor.hpp>
#include <Input/Win32InputService.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Input;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Input {

	Win32InputService::Mouse::Mouse( const ::HANDLE deviceHandle, Win32InputService& owner ) {
		using DeviceDirectory = decltype(owner._deviceDirectory);
		
	// ---

		owner._deviceDirectory.Insert( DeviceDirectory::ValueType( deviceHandle, this ) );
	}

// ---------------------------------------------------

	void Win32InputService::Mouse::Dispose() {}

// ---------------------------------------------------

	void Win32InputService::Mouse::ReadInputPacket( const ::RAWINPUT& inputPacket ) {
		const ::RAWMOUSE&	mouse( inputPacket.data.mouse );

		if( mouse.usFlags & MOUSE_MOVE_ABSOLUTE ) {}
		else {}

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

}	// namespace Input
}	// namespace Eldritch2