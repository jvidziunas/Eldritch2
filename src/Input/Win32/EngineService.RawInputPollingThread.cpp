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
#include <Input/Win32/EngineService.hpp>
#include <Utility/Concurrency/Lock.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
#include <windowsx.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2::Input;
using namespace ::Eldritch2;

namespace {

	static ETForceInlineHint void RegisterCallingThreadForRawInput() {
		enum : ::DWORD {
			KeyboardAttachFlags = (RIDEV_NOLEGACY | RIDEV_APPKEYS),
			MouseAttachFlags	= RIDEV_NOLEGACY
		};

		enum : ::USHORT {
			KeyboardUsagePage	= 0x01,
			KeyboardUsage		= 0x06,
			MouseUsagePage		= 0x01,
			MouseUsage			= 0x02
		};

		const ::RAWINPUTDEVICE	inputDeviceRegistrationList[] = {
			{ KeyboardUsagePage,	KeyboardUsage,	KeyboardAttachFlags,	nullptr },
			{ MouseUsagePage,		MouseUsage,		MouseAttachFlags,		nullptr }
		};

	// ---

		::RegisterRawInputDevices( inputDeviceRegistrationList, _countof(inputDeviceRegistrationList), sizeof(inputDeviceRegistrationList[0]) );
	}

}	// anonymous namespace

namespace Eldritch2 {
namespace Input {
namespace Win32 {

	EngineService::RawInputPollingThread::RawInputPollingThread( EngineService& hostingService ) : _hostingInputService( hostingService ), _threadID( 0u ) {}

// ---------------------------------------------------

	const UTF8Char* const EngineService::RawInputPollingThread::GetHumanReadableName() const {
		return UTF8L("Win32 Input Service Polling Module");
	}

// ---------------------------------------------------

	void EngineService::RawInputPollingThread::Run() {
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
		::PeekMessageW( &receivedMessage, nullptr, WM_USER, WM_USER, PM_NOREMOVE );

		RegisterCallingThreadForRawInput();

		// GetMessage() returns 0 if it recieved a WM_QUIT message. This call intentionally blocks.
		while( 0 != ::GetMessageW( &receivedMessage, nullptr, 0u, 0xFFFFFFFF ) ) {
			MICROPROFILE_SCOPEI( "Input", "Dispatch raw input events", 0xABABAB );

			switch( receivedMessage.message ) {
				case WM_INPUT: {
					::RAWINPUT	rawInput;
					::UINT		rawInputSize( sizeof(rawInput) );

					if( static_cast<::UINT>(-1) != ::GetRawInputData( reinterpret_cast<::HRAWINPUT>(receivedMessage.lParam), RID_INPUT, &rawInput, &rawInputSize, sizeof(::RAWINPUTHEADER) ) ) {
						ScopedReaderLock	_( *_hostingInputService._deviceDirectoryMutex );
						// Dispatch the message to the appropriate device, or have the null device eat the packet.
						_hostingInputService._deviceDirectory.Find( rawInput.header.hDevice, &nullDevice )->ReadInputPacket( rawInput );
					}

					break;
				}

			// ---

				default: ETNoDefaultCaseHint;
			}	// switch( receivedMessage.message )
			
			::TranslateMessage( &receivedMessage );
			::DispatchMessageW( &receivedMessage );
		}

		_threadID = 0u;
	}

// ---------------------------------------------------

	void EngineService::RawInputPollingThread::RequestGracefulShutdown() {
		if( 0u != _threadID ) {
			::PostThreadMessageW( _threadID, WM_QUIT, 0, 0 );
		}
	}

}	// namespace Win32
}	// namespace Input
}	// namespace Eldritch2