/*==================================================================*\
  Win32InputEngineComponent.Mouse.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Input/InputDevice.hpp>
//------------------------------------------------------------------//
#include <windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Input {
namespace Win32 {
namespace {

	enum : ScanCode {
		LeftButton,
		RightButton,
		MiddleButton,
		ExtendedButton0,
		ExtendedButton1,
		Wheel,
		HorizontalAxis,
		VerticalAxis
	};

}	// anonymous namespace

	void ReadPacket( const RAWMOUSE& packet, const InputDevice& device ) {
		if (packet.usButtonFlags & RI_MOUSE_BUTTON_1_DOWN) {
			device.MapCodeAndDispatch( LeftButton, 1 );
		}
			
		if (packet.usButtonFlags & RI_MOUSE_BUTTON_1_UP) {
			device.MapCodeAndDispatch( LeftButton, -1 );
		}

		if (packet.usButtonFlags & RI_MOUSE_BUTTON_2_DOWN) {
			device.MapCodeAndDispatch( RightButton, 1 );
		}

		if (packet.usButtonFlags & RI_MOUSE_BUTTON_2_UP) {
			device.MapCodeAndDispatch( RightButton, -1 );
		}

		if (packet.usButtonFlags & RI_MOUSE_BUTTON_3_DOWN) {
			device.MapCodeAndDispatch( MiddleButton, 1 );
		}

		if (packet.usButtonFlags & RI_MOUSE_BUTTON_3_UP) {
			device.MapCodeAndDispatch( MiddleButton, -1 );
		}

		if (packet.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN) {
			device.MapCodeAndDispatch( ExtendedButton0, 1 );
		}
			
		if (packet.usButtonFlags & RI_MOUSE_BUTTON_4_UP) {
			device.MapCodeAndDispatch( ExtendedButton0, -1 );
		}

		if (packet.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN) {
			device.MapCodeAndDispatch( ExtendedButton1, 1 );
		}
			
		if (packet.usButtonFlags & RI_MOUSE_BUTTON_5_UP) {
			device.MapCodeAndDispatch( ExtendedButton1, -1 );
		}
		
		if (packet.usButtonFlags & RI_MOUSE_WHEEL) {
			device.MapCodeAndDispatch( Wheel, reinterpret_cast<const SHORT&>(packet.usButtonData) );
		}

		device.MapCodeAndDispatch( HorizontalAxis, packet.lLastX );
		device.MapCodeAndDispatch( VerticalAxis, packet.lLastY );
	}

}	// namespace Win32
}	// namespace Input
}	// namespace Eldritch2