/*==================================================================*\
  InputDevices.Mouse.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Input/Win32/InputDevices.hpp>
//------------------------------------------------------------------//
#include <windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Input {
namespace Win32 {

	void Mouse::ReadPacket( const RAWINPUT& packet ) {
		const RAWMOUSE&	event( packet.data.mouse );
		const int16		wheelDelta( reinterpret_cast<const SHORT&>(event.usButtonData) );
		const int16		horizontalDelta( event.lLastX );
		const int16		verticalDelta( event.lLastY );

		if (event.usButtonFlags & RI_MOUSE_BUTTON_1_DOWN) {
			PressButton( LeftButton, 1 );
		}
			
		if (event.usButtonFlags & RI_MOUSE_BUTTON_1_UP) {
			PressButton( LeftButton, -1 );
		}

		if (event.usButtonFlags & RI_MOUSE_BUTTON_2_DOWN) {
			PressButton( RightButton, 1 );
		}

		if (event.usButtonFlags & RI_MOUSE_BUTTON_2_UP) {
			PressButton( RightButton, -1 );
		}

		if (event.usButtonFlags & RI_MOUSE_BUTTON_3_DOWN) {
			PressButton( MiddleButton, 1 );
		}

		if (event.usButtonFlags & RI_MOUSE_BUTTON_3_UP) {
			PressButton( MiddleButton, -1 );
		}

		if (event.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN) {
			PressButton( ExtendedButton0, 1 );
		}
			
		if (event.usButtonFlags & RI_MOUSE_BUTTON_4_UP) {
			PressButton( ExtendedButton0, -1 );
		}

		if (event.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN) {
			PressButton( ExtendedButton1, 1 );
		}
			
		if (event.usButtonFlags & RI_MOUSE_BUTTON_5_UP) {
			PressButton( ExtendedButton1, -1 );
		}

		if (event.usButtonFlags & RI_MOUSE_WHEEL) {
			PressButton( Wheel, wheelDelta );
		}

		if (horizontalDelta != 0) {
			PressButton( HorizontalAxis, horizontalDelta );
		}

		if (verticalDelta != 0) {
			PressButton( VerticalAxis, verticalDelta );
		}
	}

// ---------------------------------------------------

	void Mouse::PressButton( ScanCode code, int16 amount ) const {
		ReadLock							_( _mutex );
		const BindingMap<>::ConstIterator	binding( _bindingsByCode.Find( code ) );

		if (binding == _bindingsByCode.End()) {
			return;
		}

		amount *= binding->second.weight;
	}

}	// namespace Win32
}	// namespace Input
}	// namespace Eldritch2