/*==================================================================*\
  InputDevices.Keyboard.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Input/Win32/InputDevices.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Input {
namespace Win32 {

	void Keyboard::ReadPacket( const RAWINPUT& packet ) {
		const RAWKEYBOARD&	event( packet.data.keyboard );
		const bool			isBreak( event.Flags & RI_KEY_BREAK );
		UINT				scanCode( event.MakeCode );
		UINT				key( event.VKey );

		if (key == 0xFF) {
		//	Discard "fake keys" which are part of an escaped sequence.
			return;
		}

		if (key == VK_SHIFT) {
		//	Left-/right-hand SHIFT
			key = MapVirtualKeyW( scanCode, MAPVK_VSC_TO_VK_EX );
		} else if (key == VK_NUMLOCK) {
		//	Correct PAUSE/BREAK and NUM LOCK silliness, and set the extended bit.
			scanCode = MapVirtualKeyW( key, MAPVK_VK_TO_VSC ) | 0x100;
		}

	/*	E0 and E1 are escape sequences used for certain special keys, such as PRINT and PAUSE/BREAK;
	 *	see http://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html */
		const bool	hasE0( (event.Flags & RI_KEY_E0) != 0 );

		if ((event.Flags & RI_KEY_E1) != 0u) {
		/*	For escaped sequences, turn the virtual key into the correct scan code using MapVirtualKey.
		 *	However, MapVirtualKey is unable to map VK_PAUSE (this is a known bug), hence we map that by hand. */
			scanCode = (key == VK_PAUSE) ? 0x45 : MapVirtualKeyW( key, MAPVK_VK_TO_VSC );
		}

		switch (key) {
		//	Right-hand CONTROL/ALT have their E0 bit set.
			case VK_CONTROL:	key = (hasE0 ? VK_RCONTROL : VK_LCONTROL);	break;
			case VK_MENU:		key = (hasE0 ? VK_RMENU    : VK_LMENU);		break;
		//	NUMPAD enter/separator has the E0 bit set.
			case VK_RETURN:		if (hasE0)  key = VK_SEPARATOR;				break;
		/*	The standard INSERT, DELETE, HOME, END, PRIOR and NEXT keys will always have their e0 bit set,
		 *	but the corresponding keys on the NUMPAD will not. */
			case VK_INSERT:		if (!hasE0) key = VK_NUMPAD0;				break;
			case VK_DELETE:		if (!hasE0) key = VK_DECIMAL;				break;
			case VK_HOME:		if (!hasE0) key = VK_NUMPAD7;				break;
			case VK_END:		if (!hasE0) key = VK_NUMPAD1;				break;
			case VK_PRIOR:		if (!hasE0) key = VK_NUMPAD9;				break;
			case VK_NEXT:		if (!hasE0) key = VK_NUMPAD3;				break;
		/*	The standard arrow keys will always have their E0 bit set, but the corresponding keys on the
		 *	NUMPAD will not. */
			case VK_LEFT:		if (!hasE0) key = VK_NUMPAD4;				break;
			case VK_RIGHT:		if (!hasE0) key = VK_NUMPAD6;				break;
			case VK_UP:			if (!hasE0) key = VK_NUMPAD8;				break;
			case VK_DOWN:		if (!hasE0) key = VK_NUMPAD2;				break;
		//	NUMPAD 5 doesn't have its e0 bit set.
			case VK_CLEAR:		if (!hasE0) key = VK_NUMPAD5;				break;
		}	// switch (virtualKey)

		PressButton( static_cast<ScanCode>(key), !isBreak );
	}

// ---------------------------------------------------

	void Keyboard::PressButton( ScanCode code, int16 amount ) const {
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