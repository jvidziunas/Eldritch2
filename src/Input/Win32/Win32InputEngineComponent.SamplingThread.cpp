/*==================================================================*\
  Win32InputEngineComponent.SamplingThread.cpp
  ------------------------------------------------------------------
  Purpose:
  Defines a Win32 application decorator object that invokes input
  system event objects when the user presses the corresponding key,
  moves a stick or shifts the mouse.

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Input/Win32/Win32InputEngineComponent.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
#include <windowsx.h>
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Input {
namespace Win32 {
namespace {

	struct RawInputDeviceInfo : public RID_DEVICE_INFO {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref RawInputDeviceInfo instance.
		ETInlineHint RawInputDeviceInfo( HANDLE deviceHandle ) {
			infoSize = cbSize = sizeof(RID_DEVICE_INFO);
			GetRawInputDeviceInfoW( deviceHandle, RIDI_DEVICEINFO, this, &infoSize );
			GetRawInputDeviceInfoW( deviceHandle, RIDI_DEVICENAME, name, &nameLength );
		}

		~RawInputDeviceInfo() = default;

	// ---------------------------------------------------

	public:
		bool IsVirtualOrUnavailable() const {
			if (CreateFileW( name, 0u, (FILE_SHARE_READ | FILE_SHARE_WRITE), nullptr, OPEN_EXISTING, 0u, nullptr ) == INVALID_HANDLE_VALUE) {
				return true;
			}

			return false;
		}

	// - DATA MEMBERS ------------------------------------

	public:
		UINT	infoSize;
		UINT	nameLength;
		wchar_t	name[MAX_PATH];
	};

// ---

	class WindowClass {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref WindowClass instance.
		ETInlineHint WindowClass( WNDPROC windowProc ) {
			WNDCLASSEXW	windowClass;

			windowClass.cbSize			= sizeof(windowClass);
			windowClass.style			= 0;
			windowClass.lpfnWndProc		= windowProc;
			windowClass.cbClsExtra		= 0;
			windowClass.cbWndExtra		= 0;
			windowClass.hInstance		= _instance = GetModuleByAddress( windowProc );
			windowClass.hIcon			= nullptr;
			windowClass.hCursor			= nullptr;
			windowClass.hbrBackground	= nullptr;
			windowClass.lpszMenuName	= nullptr;
			windowClass.lpszClassName	= L"MessageWindow";
			windowClass.hIconSm			= nullptr;

			_atom = reinterpret_cast<LPCWSTR>( RegisterClassExW( &windowClass ) );
		}

	//!	Constructs this @ref WindowClass instance.
		ETInlineHint WindowClass() : WindowClass( &DefWindowProcW ) {}

		ETInlineHint ~WindowClass() {
			UnregisterClassW( _atom, _instance );
		}

	// ---------------------------------------------------

	public:
		ETInlineHint operator LPCWSTR() const {
			return _atom;
		}

	// - DATA MEMBERS ------------------------------------

	private:
		HINSTANCE	_instance;
		LPCWSTR		_atom;
	};

// ---------------------------------------------------

	static ETInlineHint bool RegisterForRawInput( HWND windowHandle ) {
		enum : DWORD {
			KeyboardAttachFlags	= ( RIDEV_INPUTSINK | RIDEV_DEVNOTIFY ),
			MouseAttachFlags	= ( RIDEV_INPUTSINK | RIDEV_DEVNOTIFY )
		};

		enum : USHORT {
			KeyboardUsagePage	= 0x01,
			KeyboardUsage		= 0x06,
			MouseUsagePage		= 0x01,
			MouseUsage			= 0x02
		};

		const RAWINPUTDEVICE	inputDeviceRegistrationList[] = {
			{ KeyboardUsagePage,	KeyboardUsage,	KeyboardAttachFlags,	windowHandle },
			{ MouseUsagePage,		MouseUsage,		MouseAttachFlags,		windowHandle }
		};

	// ---

		return RegisterRawInputDevices( inputDeviceRegistrationList, _countof(inputDeviceRegistrationList), sizeof(*inputDeviceRegistrationList) ) == TRUE;
	}

}	// anonymous namespace

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Logging;

	Win32InputEngineComponent::SamplingThread::SamplingThread(
		Log& log
	) : _log( log ),
		_window( nullptr ),
		_keyboardsByHandle( MallocAllocator( "Keyboards By Handle Map Allocator" ) ),
		_miceByHandle( MallocAllocator( "Mice By Handle Map Allocator" ) ) {
	}

// ---------------------------------------------------

	Utf8Literal Win32InputEngineComponent::SamplingThread::GetName() const {
		return "Win32 Input Sampler";
	}

// ---------------------------------------------------

	void Win32InputEngineComponent::SamplingThread::ScanDevices() {
		DeviceMap<Keyboard>	keyboardsByHandle( _keyboardsByHandle.GetAllocator() );
		DeviceMap<Mouse>	miceByHandle( _miceByHandle.GetAllocator() );
		UINT				deviceCount( 0u );

	//	Ask Windows how many raw input devices it knows about.
		if (GetRawInputDeviceList( nullptr, &deviceCount, sizeof(RAWINPUTDEVICELIST) ) == static_cast<UINT>(-1)) {
			_log.Write( MessageType::Error, "Failed to enumerate Win32 input devices!" UTF8_NEWLINE );
			return;
		}

	//	Fill a temporary array with more detailed information about each device, including its type and an internal handle.
		RAWINPUTDEVICELIST* const devices( static_cast<RAWINPUTDEVICELIST*>(_alloca( deviceCount * sizeof(RAWINPUTDEVICELIST) )) );
		if (GetRawInputDeviceList( devices, &deviceCount, sizeof(RAWINPUTDEVICELIST) ) == static_cast<UINT>(-1)) {
			_log.Write( MessageType::Error, "Failed to enumerate Win32 input devices!" UTF8_NEWLINE );
			return;
		}

		for (UINT index( 0u ); index < deviceCount; ++index) {
			const RAWINPUTDEVICELIST& device( devices[index] );

			switch (device.dwType) {
				case RIM_TYPEMOUSE: {
					miceByHandle.Emplace( device.hDevice, Mouse() );

					_log.Write( MessageType::Message, "Attached mouse (handle: {}) successfully." UTF8_NEWLINE, device.hDevice );
					break;
				}	// case RIM_TYPEMOUSE

				case RIM_TYPEKEYBOARD: {
					keyboardsByHandle.Emplace( device.hDevice, Keyboard() );

					_log.Write( MessageType::Message, "Attached keyboard (handle: {}) successfully." UTF8_NEWLINE, device.hDevice );
					break;
				}	// case RIM_TYPEKEYBOARD

				default: {
					_log.Write( MessageType::Warning, "Ignoring unknown HID (type: {:#x}, handle: {})." UTF8_NEWLINE, device.dwType, device.hDevice );
					break;
				}	// default
			}	// switch( device.dwType )
		}

		Swap( _keyboardsByHandle,	keyboardsByHandle );
		Swap( _miceByHandle,		miceByHandle );

		_log.Write( MessageType::Message, "Input enumeration complete." UTF8_NEWLINE );
	}

// ---------------------------------------------------

	void Win32InputEngineComponent::SamplingThread::Run() {
	/*	Since we operate in short bursts of work before sleeping, boost thread priority slightly to reduce the amount of downtime before the
	 *	scheduler will queue us for execution. Windows internally does some priority boosting for us behind the scenes when we receive input,
	 *	this is designed to supplement that behavior. */
		SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_HIGHEST );

		const WindowClass	windowClass;
		const HWND			window( CreateWindowExW( 0, windowClass, nullptr, 0, 0, 0, 0, 0, HWND_MESSAGE, nullptr, nullptr, nullptr ) );

		ET_VERIFY( RegisterForRawInput( window ), "Error creating input listener window" );

		ScanDevices();

		_window.store( window, std::memory_order_release );

	//	GetMessage() returns 0 if it recieved a WM_QUIT message. This call intentionally blocks.
		BOOL errorFlag;

		for (MSG message; ET_LIKELY( errorFlag = GetMessageW( &message, window, 0u, 0u ) ) != 0; ) {
			if (ET_UNLIKELY( errorFlag == -1 )) {
				break;
			}

			if (ET_LIKELY( message.message == WM_INPUT )) {
				RAWINPUT	input;
				UINT		size( sizeof(input) );

				if (GetRawInputData( reinterpret_cast<HRAWINPUT>(message.lParam), RID_INPUT, &input, &size, sizeof(RAWINPUTHEADER) ) == static_cast<UINT>(-1)) {
					switch (input.header.dwType) {
						case RIM_TYPEMOUSE: {
							const DeviceMap<Mouse>::Iterator	target( _miceByHandle.Find( input.header.hDevice ) );

							if (ET_UNLIKELY( target == _miceByHandle.End() )) {
							//	We may be receiving input notifications for virtual or other devices we do not have registered handlers for. Ignore incoming data in this case.
								continue;
							}

							target->second.ReadPacket( input );
							break;
						}	// case RIM_TYPEMOUSE

						case RIM_TYPEKEYBOARD: {
							const DeviceMap<Keyboard>::Iterator	target( _keyboardsByHandle.Find( input.header.hDevice ) );

							if (ET_UNLIKELY( target == _keyboardsByHandle.End() )) {
							//	We may be receiving input notifications for virtual or other devices we do not have registered handlers for. Ignore incoming data in this case.
								continue;
							}

							target->second.ReadPacket( input );
							break;
						}	// case RIM_TYPEKEYBOARD
					}	// switch( input.header.dwType )
				}
			}
			
			TranslateMessage( &message );
			DispatchMessageW( &message );
		}

		DestroyWindow( window );
	}

// ---------------------------------------------------

	void Win32InputEngineComponent::SamplingThread::SetShouldShutDown() {
		PostMessageW( _window.load( std::memory_order_acquire ), WM_CLOSE, 0, 0 );
	}

}	// namespace Win32
}	// namespace Input
}	// namespace Eldritch2