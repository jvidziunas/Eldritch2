/*==================================================================*\
  EngineService.RawInputPollingThread.cpp
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
#include <Input/Win32/ScriptComponents/Keyboard.hpp>
#include <Input/Win32/ScriptComponents/Mouse.hpp>
#include <Utility/Win32ApplicationHelpers.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Utility/Containers/HashMap.hpp>
#include <Input/Win32/EngineService.hpp>
#include <Utility/CountedPointer.hpp>
#include <Utility/Result.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
#if !defined( WIN32_LEAN_AND_MEAN )
#	define WIN32_LEAN_AND_MEAN
#endif
#include <windowsx.h>
#include <Windows.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scheduling;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Utility;

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

		bool IsVirtualOrUnavailable() const {
			if( INVALID_HANDLE_VALUE == CreateFileW( name, 0u, (FILE_SHARE_READ | FILE_SHARE_WRITE), nullptr, OPEN_EXISTING, 0u, nullptr ) ) {
				return true;
			}

			return false;
		}

	// - DATA MEMBERS ------------------------------------

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
			windowClass.hInstance		= _instance = GetInstance( windowProc );
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
			KeyboardAttachFlags	= ( RIDEV_INPUTSINK | RIDEV_NOLEGACY | RIDEV_APPKEYS ),
			MouseAttachFlags	= ( RIDEV_INPUTSINK | RIDEV_NOLEGACY )
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

		return TRUE == RegisterRawInputDevices( inputDeviceRegistrationList, _countof(inputDeviceRegistrationList), sizeof(inputDeviceRegistrationList[0]) );
	}

// ---------------------------------------------------

	template <typename DeviceCollection>
	static ETInlineHint void EnumerateAvailableRawInputDevices( Allocator& allocator, DeviceCollection& devices ) {
		UINT	count( 0u );

	//	Ask Windows how many raw input devices it knows about.
		if( static_cast<UINT>(-1) == GetRawInputDeviceList( nullptr, &count, sizeof(RAWINPUTDEVICELIST) ) ) {
		//	GetLogger( LogMessageType::Error )( "Input service: Failed to enumerate raw input devices!" ET_UTF8_NEWLINE_LITERAL );
			return;
		}

	//	Allocate a temporary array to hold all the enumerated devices.
		RAWINPUTDEVICELIST* const	enumeratedDevices( static_cast<RAWINPUTDEVICELIST*>(_alloca( count * sizeof(RAWINPUTDEVICELIST) )) );

	//	GetLogger( LogMessageType::Message )( "Input service: Enumerated {} raw input devices." ET_UTF8_NEWLINE_LITERAL, count );

	//	Fill a temporary array with more detailed information about each device, including its type and an internal handle.
		if( static_cast<UINT>(-1) == GetRawInputDeviceList( enumeratedDevices, &count, sizeof( RAWINPUTDEVICELIST ) ) ) {
		//	GetLogger( LogMessageType::Error )( "Input service: Failed to associate input devices!" ET_UTF8_NEWLINE_LITERAL );
			return;
		}

		for( auto& device : Range<RAWINPUTDEVICELIST*>( enumeratedDevices, enumeratedDevices + count ) ) {
			auto	createDevice( HandleDeviceAttach( allocator, device.hDevice ) );

			if( !createDevice || !*createDevice ) {
				continue;
			}

			devices.Emplace( eastl::move( device.hDevice ), eastl::move( *createDevice ) );
		}
	}

}	// anonymous namespace

	EngineService::RawInputPollingThread::RawInputPollingThread( Allocator& allocator ) : _allocator( allocator, "Raw Input Polling Thread Allocator" ), _window( nullptr ) {}

// ---------------------------------------------------

	Utf8Literal EngineService::RawInputPollingThread::GetHumanReadableName() const {
		return "Win32 Input Service Polling Module";
	}

// ---------------------------------------------------

	void EngineService::RawInputPollingThread::Run() {
		const WindowClass	windowClass;
		const auto			windowHandle( CreateWindowExW( 0, windowClass, nullptr, 0, 0, 0, 0, 0, HWND_MESSAGE, nullptr, nullptr, nullptr ) );

		ETRuntimeVerificationWithMessage( RegisterForRawInput( windowHandle ), "Error creating input thread" );

		_window.store( windowHandle );

	//	GetMessage() returns 0 if it recieved a WM_QUIT message. This call intentionally blocks.
		for( MSG message; 0 != GetMessageW( &message, windowHandle, 0u, 0u ); ) {
			if( WM_INPUT == message.message ){
				RAWINPUT	rawInput;
				UINT		rawInputSize( sizeof(rawInput) );

				MICROPROFILE_SCOPEI( "Input", "Dispatch raw input events", 0xABABAB );

				if( static_cast<UINT>(-1) != GetRawInputData( reinterpret_cast<HRAWINPUT>(message.lParam), RID_INPUT, &rawInput, &rawInputSize, sizeof(RAWINPUTHEADER) ) ) {

				}
			}
			
			TranslateMessage( &message );
			DispatchMessageW( &message );
		}

		DestroyWindow( windowHandle );
	}

// ---------------------------------------------------

	void EngineService::RawInputPollingThread::RequestGracefulShutdown() {
		const auto window( _window.exchange( nullptr, std::memory_order_relaxed ) );

		if( !window ) {
			return;
		}

		SendMessageW( window, WM_QUIT, 0, 0 );
	}

}	// namespace Win32
}	// namespace Input
}	// namespace Eldritch2