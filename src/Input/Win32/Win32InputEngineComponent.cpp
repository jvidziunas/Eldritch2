/*==================================================================*\
  Win32InputEngineComponent.cpp
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
#include <Input/Win32/Win32InputEngineComponent.hpp>
#include <Input/Win32/Win32InputWorldComponent.hpp>
#include <Scheduling/JobSystem.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
#include <Windowsx.h>
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Input {
namespace Win32 {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;

namespace {

	static LRESULT CALLBACK WindowsKeyHook( int nCode, WPARAM wParam, LPARAM lParam ) {
		if (ET_LIKELY( nCode >= HC_ACTION )) {
			switch( wParam ) {
				case WM_KEYDOWN:
				case WM_KEYUP: {
					const KBDLLHOOKSTRUCT&	hook( *reinterpret_cast<KBDLLHOOKSTRUCT*>( lParam ) );
					const bool				isWindowsKey( (VK_LWIN == hook.vkCode) | (VK_RWIN == hook.vkCode) );
					const bool				isRealKeypress( !(hook.dwExtraInfo & LLMHF_INJECTED) );
				/*	Verify that this was an actual keypress, *not* a specially-injected Windows key event.
				 *	We're interested in stopping the cases whereupon a user accidentally fat-fingers the Windows key
				 *	when they actually meant to press the control or alt keys. */
					if (ET_UNLIKELY( isRealKeypress & isWindowsKey ) ) {
						return 1;
					}
				}	// case WM_KEYDOWN, case WM_KEYUP
			}	// switch( wParam )
		}

		return CallNextHookEx( nullptr, nCode, wParam, lParam );
	}

// ---------------------------------------------------

ET_PUSH_COMPILER_WARNING_STATE()
//	(6001) MSVC doesn't like the name query code and complains that the value is being used before being initialized. This is not the case, since GetRawInputDeviceInfo() writes to the provided buffer.
//	(6054) MSVC complains about the name string potentially not having a null terminator; GetRawInputDeviceInfo writes the terminator for us.
	ET_SET_MSVC_WARNING_STATE( disable : 6001 6054 )
	ETInlineHint bool ShouldIgnoreDevice( const RAWINPUTDEVICELIST& device ) {
		wchar_t	name[MAX_PATH];
		UINT	nameLength( _countof( name ) );

		if (device.dwType == RIM_TYPEHID) {
			return true;
		}

	//	Aggregate devices do not have a name that can be queried.
		if (GetRawInputDeviceInfoW( device.hDevice, RIDI_DEVICENAME, name, &nameLength ) < 0) {
			return true;
		}

	//	Virtual devices cannot be opened for read with CreateFile().
		if (CreateFileW( name, 0u, (FILE_SHARE_READ | FILE_SHARE_WRITE), nullptr, OPEN_EXISTING, 0u, nullptr ) == INVALID_HANDLE_VALUE) {
			return true;
		}

		return false;
	}
ET_POP_COMPILER_WARNING_STATE()

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint const char* GetDeviceTypeName( DWORD type ) {
		switch (type) {
		case RIM_TYPEMOUSE:    return "mouse";
		case RIM_TYPEKEYBOARD: return "keyboard";
		case RIM_TYPEHID:      return "generic HID";
		default:               return "<unknown>";
		};
	}

}	// anonymous namespace

	void	ReadPacket( const RAWKEYBOARD& packet, const InputDevice& device );
	void	ReadPacket( const RAWMOUSE& packet, const InputDevice& device );

// ---------------------------------------------------

	Win32InputEngineComponent::Win32InputEngineComponent( 
		const Blackboard& services,
		Log& log
	) : EngineComponent( services ),
		_log( log ),
		_keyboardHook( SetWindowsHookExW( WH_KEYBOARD_LL, &WindowsKeyHook, GetModuleHandleW( nullptr ), 0 ) ),
		_inputReader( *this ),
		_devicesByHandle( MallocAllocator( "Win32 Raw Input Device By Handle Map Allocator" ) ),
		_mice( MallocAllocator( "Win32 Raw Input Mouse List Allocator" ) ),
		_keyboards( MallocAllocator( "Win32 Raw Input Keyboard List Allocator" ) ) {
	}

// ---------------------------------------------------

	Win32InputEngineComponent::~Win32InputEngineComponent() {
		_inputReader.AwaitCompletion();

	//	Remove the hook that allowed us to selectively ignore Windows key press events.
		if (_keyboardHook) {
			UnhookWindowsHookEx( _keyboardHook );
		}
	}

// ---------------------------------------------------

	Result<UniquePointer<WorldComponent>> Win32InputEngineComponent::CreateWorldComponent( Allocator& allocator, const World& world ) {
		UniquePointer<WorldComponent> inputComponent( CreateUnique<Win32InputWorldComponent>( allocator, world ) );
		if (inputComponent == nullptr) {
			return Error::OutOfMemory;
		}

		return eastl::move( inputComponent );
	}

// ---------------------------------------------------

	void Win32InputEngineComponent::AcceptVisitor( JobExecutor& /*executor*/, const InitializationVisitor ) {
		MICROPROFILE_SCOPEI( "Engine/Initialization", "Initialize Win32 input", 0xBBBBBB );

		{	const ErrorCode	result( FindService<JobSystem>().Launch( _inputReader ) );
			if (Failed( result )) {
				_log.Write( MessageType::Error, "Error launching input thread: {}!" UTF8_NEWLINE, AsCString( result ) );
				return;
			}
		}

		_log.Write( MessageType::Message, "Win32 input service initialized successfully." UTF8_NEWLINE );
	}

// ---------------------------------------------------

	void Win32InputEngineComponent::ScanDevices() {
		ArrayMap<HANDLE, InputDevice>	devicesByHandle( _devicesByHandle.GetAllocator() );
		UINT							deviceCount( 0u );

	//	Ask Windows how many raw input devices it knows about.
		if (GetRawInputDeviceList( nullptr, &deviceCount, sizeof(RAWINPUTDEVICELIST) ) == static_cast<UINT>(-1)) {
			_log.Write( MessageType::Error, "Failed to enumerate Win32 input devices!" UTF8_NEWLINE );
			return;
		}

	//	Fill a temporary array with more detailed information about each device, including its type and the Win32 handle.
		RAWINPUTDEVICELIST* const devices( static_cast<RAWINPUTDEVICELIST*>(_alloca( deviceCount * sizeof(RAWINPUTDEVICELIST) )) );
		if (GetRawInputDeviceList( devices, &deviceCount, sizeof(RAWINPUTDEVICELIST) ) == static_cast<UINT>(-1)) {
			_log.Write( MessageType::Error, "Failed to enumerate Win32 input devices!" UTF8_NEWLINE );
			return;
		}

		devicesByHandle.SetCapacity( deviceCount );

		for (const RAWINPUTDEVICELIST& device : Range<const RAWINPUTDEVICELIST*>( devices, devices + deviceCount )) {
			if (ShouldIgnoreDevice( device )) {
				_log.Write( MessageType::Message, "Ignoring unknown/virtual {} (handle: {})" UTF8_NEWLINE, GetDeviceTypeName( device.dwType ), device.hDevice );
				continue;
			}

			devicesByHandle.Emplace( device.hDevice, InputDevice() );
			_log.Write( MessageType::Message, "Bound {} (handle: {}) successfully." UTF8_NEWLINE, GetDeviceTypeName( device.dwType ), device.hDevice );
		}

	//	Switch the local device map with the public version.
		Swap( _devicesByHandle, devicesByHandle );

		_log.Write( MessageType::Message, "Input enumeration complete." UTF8_NEWLINE );
	}

// ---------------------------------------------------

	void Win32InputEngineComponent::Dispatch( const RAWINPUT& event ) const {
		const ArrayMap<HANDLE, InputDevice>::ConstIterator candidate( _devicesByHandle.Find( event.header.hDevice ) );
		if (ET_UNLIKELY( candidate == _devicesByHandle.End() )) {
		//	We may be receiving input notifications for virtual or other devices we do not have registered handlers for. Ignore incoming data in this case.
			return;
		}

		switch (event.header.dwType) {
		case RIM_TYPEMOUSE:		return ReadPacket( event.data.mouse,    candidate->second );
		case RIM_TYPEKEYBOARD:	return ReadPacket( event.data.keyboard, candidate->second );
		}	// switch( event.header.dwType )
	}

}	// namespace Win32
}	// namespace Input
}	// namespace Eldritch2