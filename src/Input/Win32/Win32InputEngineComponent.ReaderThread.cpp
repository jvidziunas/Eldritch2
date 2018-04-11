/*==================================================================*\
  Win32InputEngineComponent.ReaderThread.cpp
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
			KeyboardAttachFlags	= ( RIDEV_INPUTSINK | RIDEV_DEVNOTIFY | RIDEV_NOLEGACY ),
			MouseAttachFlags	= ( RIDEV_INPUTSINK | RIDEV_DEVNOTIFY | RIDEV_NOLEGACY )
		};

		enum : USHORT {
			KeyboardUsagePage	= 0x01,
			KeyboardUsage		= 0x06,
			MouseUsagePage		= 0x01,
			MouseUsage			= 0x02
		};

		const RAWINPUTDEVICE inputDeviceRegistrationList[] = {
			{ KeyboardUsagePage, KeyboardUsage, KeyboardAttachFlags, windowHandle },
			{ MouseUsagePage,    MouseUsage,    MouseAttachFlags,    windowHandle }
		};

	// ---

		return RegisterRawInputDevices( inputDeviceRegistrationList, _countof(inputDeviceRegistrationList), sizeof(*inputDeviceRegistrationList) ) == TRUE;
	}

}	// anonymous namespace

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Logging;

	Win32InputEngineComponent::ReaderThread::ReaderThread( Win32InputEngineComponent& owner ) : _owner( owner ), _window( nullptr ) {}

// ---------------------------------------------------

	Utf8Literal Win32InputEngineComponent::ReaderThread::GetName() const {
		return "Win32 Input Sampler";
	}

// ---------------------------------------------------

	void Win32InputEngineComponent::ReaderThread::Run() {
	/*	Since we operate in short bursts of work before sleeping, boost thread priority slightly to reduce the amount of downtime before the
	 *	scheduler will queue us for execution. Windows internally does some priority boosting for us behind the scenes when we receive input,
	 *	this is designed to supplement that behavior. */
		SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_HIGHEST );

		const WindowClass	windowClass;
		const HWND			window( CreateWindowExW( 0, windowClass, nullptr, 0, 0, 0, 0, 0, HWND_MESSAGE, nullptr, nullptr, nullptr ) );

		ET_VERIFY( RegisterForRawInput( window ), "Error creating input listener" );

		_owner.ScanDevices();
		_window.store( window, std::memory_order_release );

	//	GetMessage() returns 0 if it recieved a WM_QUIT message. This call intentionally blocks.
		BOOL hadError;

		for (MSG message; ET_LIKELY( hadError = GetMessageW( &message, window, 0u, 0u ) ) != 0; ) {
			if (ET_UNLIKELY( hadError == -1 )) {
				break;
			}

			if (ET_LIKELY( message.message == WM_INPUT )) {
				RAWINPUT	event;
				UINT		size( sizeof(event) );

				if (ET_LIKELY( GetRawInputData( HRAWINPUT( message.lParam ), RID_INPUT, &event, &size, sizeof(RAWINPUTHEADER) ) > 0 )) {
					_owner.Dispatch( event );
				}
			}
			
			TranslateMessage( &message );
			DispatchMessageW( &message );
		}

		DestroyWindow( window );
	}

// ---------------------------------------------------

	void Win32InputEngineComponent::ReaderThread::SetShouldShutDown() {
		PostMessageW( _window.load( std::memory_order_acquire ), WM_CLOSE, 0, 0 );
	}

}	// namespace Win32
}	// namespace Input
}	// namespace Eldritch2