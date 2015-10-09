/*==================================================================*\
  Win32InputService.RawInputPollingThread.cpp
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
#include <Input/Win32InputService.hpp>
#include <Utility/Concurrency/Lock.hpp>
//------------------------------------------------------------------//
#include <windowsx.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2::Input;
using namespace ::Eldritch2;

namespace {

	static ETForceInlineHint void RegisterCallingThreadForRawInput() {
		enum : ::DWORD {
			KEYBOARD_ATTACH_FLAGS	= (RIDEV_APPKEYS | RIDEV_NOLEGACY),
			MOUSE_ATTACH_FLAGS		= (RIDEV_NOLEGACY)
		};

		enum : ::USHORT {
			KEYBOARD_USAGE_PAGE = 0x01,
			KEYBOARD_USAGE = 0x06,
			MOUSE_USAGE_PAGE = 0x01,
			MOUSE_USAGE = 0x02
		};

		const ::RAWINPUTDEVICE	inputDeviceRegistrationList[] = {
			{ KEYBOARD_USAGE_PAGE, KEYBOARD_USAGE, KEYBOARD_ATTACH_FLAGS, nullptr },
			{ MOUSE_USAGE_PAGE, MOUSE_USAGE, MOUSE_ATTACH_FLAGS, nullptr }
		};

	// ---

		::RegisterRawInputDevices( inputDeviceRegistrationList, _countof(inputDeviceRegistrationList), sizeof(inputDeviceRegistrationList[0]) );
	}

}	// anonymous namespace

namespace Eldritch2 {
namespace Input {

	Win32InputService::RawInputPollingThread::RawInputPollingThread( Win32InputService& hostingService ) : _hostingInputService( hostingService ), _threadID( 0u ) {}

// ---------------------------------------------------

	Win32InputService::RawInputPollingThread::~RawInputPollingThread() {}

// ---------------------------------------------------

	const UTF8Char* const Win32InputService::RawInputPollingThread::GetHumanReadableName() const {
		return UTF8L("Win32 Input Service Polling Module");
	}

// ---------------------------------------------------

	ErrorCode Win32InputService::RawInputPollingThread::Run() {
		class NullDevice : public RawInputSubscriber {
		public:
			void ReadInputPacket( const ::RAWINPUT& ) override sealed {}
		};

	// ---

		NullDevice	nullDevice;
		::MSG		receivedMessage;

		_threadID = ::GetCurrentThreadId();

		// Create the thread message queue. This is done so that the thread can be terminated cleanly with the help of PostThreadMessage() and WM_QUIT.
		// See http://msdn.microsoft.com/en-us/library/windows/desktop/ms644946(v=vs.85).aspx for details on why this is necessary.
		::PeekMessage( &receivedMessage, nullptr, WM_USER, WM_USER, PM_NOREMOVE );

		RegisterCallingThreadForRawInput();

		// GetMessage() returns 0 if it recieved a WM_QUIT message. This call intentionally blocks.
		while( 0 != ::GetMessage( &receivedMessage, nullptr, 0u, 0xFFFFFFFF ) ) {
			switch( receivedMessage.message ) {
				case WM_INPUT: {
					::RAWINPUT	rawInput;
					::UINT		rawInputSize( sizeof(rawInput) );

					if( static_cast<::UINT>(-1) != ::GetRawInputData( reinterpret_cast<::HRAWINPUT>(receivedMessage.lParam), RID_INPUT, &rawInput, &rawInputSize, sizeof(::RAWINPUTHEADER) ) ) {
						ScopedReaderLock	_( _hostingInputService._deviceDirectoryMutex );
						// Dispatch the message to the appropriate device, or have the null device eat the packet.
						_hostingInputService._deviceDirectory.Find( rawInput.header.hDevice, &nullDevice )->ReadInputPacket( rawInput );
					}

					break;
				}

			// ---

				default: ETNoDefaultCaseHint;
			}	// switch( receivedMessage.message )
		}

		_threadID = 0u;

		return Errors::NONE;
	}

// ---------------------------------------------------

	void Win32InputService::RawInputPollingThread::RequestGracefulShutdown() {
		if( 0u != _threadID ) {
			::PostThreadMessage( _threadID, WM_QUIT, 0, 0 );
		}
	}

}	// namespace Input
}	// namespace Eldritch2