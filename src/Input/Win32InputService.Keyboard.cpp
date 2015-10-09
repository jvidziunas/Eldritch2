/*==================================================================*\
  Win32InputService.Keyboard.cpp
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

	Win32InputService::Keyboard::Keyboard( const ::HANDLE deviceHandle, Win32InputService& owner ) {
		using DeviceDirectory = decltype(owner._deviceDirectory);

	// ---

		owner._deviceDirectory.Insert( DeviceDirectory::ValueType( deviceHandle, this ) );
	}

// ---------------------------------------------------

	void Win32InputService::Keyboard::Dispose() {}

// ---------------------------------------------------

	void Win32InputService::Keyboard::ReadInputPacket( const ::RAWINPUT& inputPacket ) {
		const ::RAWKEYBOARD&	keyboard( inputPacket.data.keyboard );
		const bool				isKeyUpEvent( !!(keyboard.Flags & RI_KEY_BREAK) );
		UTF8Char				utf8String[16];
		wchar_t					textString[4];
		const int				convertedCharacters( ::ToUnicode( keyboard.VKey, keyboard.MakeCode, _keyStates, textString, _countof(textString), 0u ) );

		if( 0 < convertedCharacters ) {
		}
	}

}	// namespace Input
}	// namespace Eldritch2