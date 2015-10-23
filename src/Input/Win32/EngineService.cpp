/*==================================================================*\
  EngineService.cpp
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
#include <Scripting/ScriptAPIRegistrationInitializationVisitor.hpp>
#include <Scheduler/CRTPTransientTask.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Input/Win32/EngineService.hpp>
#include <Scheduler/TaskScheduler.hpp>
#include <Foundation/GameEngine.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//
#include <windowsx.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2::Input;
using namespace ::Eldritch2;

#ifdef ERROR
#	undef ERROR
#endif

namespace {

	static ::LRESULT CALLBACK WinKeyEatHook( int nCode, ::WPARAM wParam, ::LPARAM lParam ) {
		const ::KBDLLHOOKSTRUCT&	hook( *reinterpret_cast<::KBDLLHOOKSTRUCT*>( lParam ) );
		bool						eatKeystroke( false );

		if( ETBranchLikelyHint( nCode >= HC_ACTION ) ) {
			switch( wParam ) {
				case WM_KEYDOWN:
				case WM_KEYUP:
				{
					// First, check to see if the windows key is specifically what's pressed.
					const bool	isWindowsKey( (VK_LWIN == hook.vkCode) | (VK_RWIN == hook.vkCode) );
					// Next, verify that this was an actual keypress, *not* a specially-injected Windows key event.
					// We're interested in stopping the cases whereupon a user accidentally fat-fingers the Windows key
					// when they actually meant to press the control or alt keys.
					eatKeystroke = isWindowsKey & !(hook.dwExtraInfo & LLMHF_INJECTED);
					break;
				}	// case WM_KEYDOWN, case WM_KEYUP
			}	// switch( wParam )

			if( ETBranchUnlikelyHint( eatKeystroke ) ) {
				return 1;
			}
		}

		// We have been instructed to not process this hook, simply pass it along.
		return ::CallNextHookEx( nullptr, nCode, wParam, lParam );
	}

}	// anonymous namespace

namespace Eldritch2 {
namespace Input {
namespace Win32 {

	EngineService::EngineService( GameEngine& owningEngine ) : GameEngineService( owningEngine ),
															   _allocator( { GetEngineAllocator(), UTF8L("Win32 Input Service Allocator") } ),
															   _keyboardHook( ::SetWindowsHookEx( WH_KEYBOARD_LL, &WinKeyEatHook, ::GetModuleHandle( nullptr ), 0 ) ),
															   _deviceDirectoryMutex( GetEngineTaskScheduler().AllocateReaderWriterUserMutex( _allocator ).object ),
															   _deviceDirectory( 0u, ::rde::less<::HANDLE>(), { _allocator, UTF8L("Win32 Raw Input Device Directory Allocator") } ),
															   _pollingThread( *this ) {
		ETRuntimeAssert( nullptr != _deviceDirectoryMutex );
	}

// ---------------------------------------------------

	EngineService::~EngineService() {
		_pollingThread.EnsureTerminated();

		// Remove the hook that allowed us to selectively ignore Windows key press events.
		if( _keyboardHook ) {
			::UnhookWindowsHookEx( _keyboardHook );
		}

		if( _deviceDirectoryMutex ) {
			_allocator.Delete( *_deviceDirectoryMutex, ::Eldritch2::AlignedDeallocationSemantics );
		}
	}

// ---------------------------------------------------

	const UTF8Char* const EngineService::GetName() const {
		return UTF8L("Win32 Raw Input Manager");
	}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( ScriptAPIRegistrationInitializationVisitor& visitor ) {
		EngineService::Mouse::ExposeScriptAPI( visitor );
		EngineService::Keyboard::ExposeScriptAPI( visitor );
	}

// ---------------------------------------------------

	void EngineService::AcceptTaskVisitor( Allocator& subtaskAllocator, Task& visitingTask, WorkerContext& executingContext, const PreConfigurationLoadedTaskVisitor ) {
		class InitializeRawInputTask : public CRTPTransientTask<InitializeRawInputTask> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			// Constructs this InitializeRawInputTask instance.
			ETInlineHint InitializeRawInputTask( EngineService& host, Task& visitingTask, WorkerContext& executingContext ) : CRTPTransientTask<InitializeRawInputTask>( visitingTask, Scheduler::CodependentTaskSemantics ),
																																  _host( host ) {
				TrySchedulingOnContext( executingContext );
			}

		// ---------------------------------------------------

			const UTF8Char* const GetHumanReadableName() const override sealed {
				return UTF8L("Initialize Win32 Raw Input Task");
			}

			Task* Execute( WorkerContext& /*executingContext*/ ) override sealed {
				_host.GetLogger()( UTF8L("Initializing input service.") ET_UTF8_NEWLINE_LITERAL );

				if( const auto result = _host.LaunchThread( _host._pollingThread ) ) {
					_host.GetLogger()( UTF8L("Initialized event polling thread.") ET_UTF8_NEWLINE_LITERAL );
				} else {
					_host.GetLogger( LogMessageType::ERROR )( UTF8L("Error initializing input service: %s!") ET_UTF8_NEWLINE_LITERAL, result.ToUTF8String() );
				}

				return nullptr;
			}

		// - DATA MEMBERS ------------------------------------

		private:
			EngineService&	_host;
		};

	// ---
		
		new(subtaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) InitializeRawInputTask( *this, visitingTask, executingContext );
	}

// ---------------------------------------------------

	void EngineService::EnumerateAvailableRawInputDevices() {
		ScopedLock	_( *_deviceDirectoryMutex );
		::UINT		deviceCount( 0u );

		// Ask Windows how many raw input devices it knows about.
		if( static_cast<::UINT>(-1) != ::GetRawInputDeviceList( nullptr, &deviceCount, sizeof(::RAWINPUTDEVICELIST) ) ) {
			// Allocate a temporary array to hold all the enumerated devices.
			::RAWINPUTDEVICELIST* const	deviceList( static_cast<::RAWINPUTDEVICELIST*>(_alloca( deviceCount * sizeof(::RAWINPUTDEVICELIST) )) );

			GetLogger()( UTF8L("Input service: Enumerated %u raw input devices.") ET_UTF8_NEWLINE_LITERAL, deviceCount );
			_deviceDirectory.Reserve( deviceCount );

			// Fill a temporary array with more detailed information about each device, including its type and an internal handle.
			if( static_cast<::UINT>(-1) != ::GetRawInputDeviceList( deviceList, &deviceCount, sizeof( ::RAWINPUTDEVICELIST ) ) ) {
				for( ::RAWINPUTDEVICELIST& device : Range<::RAWINPUTDEVICELIST*>( deviceList, deviceCount ) ) {
					HandleDeviceAttach( device.hDevice );
				}
			} else {
				GetLogger( LogMessageType::ERROR )( UTF8L("Input service: Failed to associate input devices!") ET_UTF8_NEWLINE_LITERAL );
			}

		} else {
			GetLogger( LogMessageType::ERROR )( UTF8L("Input service: Failed to enumerate raw input devices!") ET_UTF8_NEWLINE_LITERAL );
		}
	}

// ---------------------------------------------------

	void EngineService::HandleDeviceAttach( const ::HANDLE deviceHandle ) {
		struct RawInputDeviceInfo : public ::RID_DEVICE_INFO {
		public:
			ETInlineHint RawInputDeviceInfo( const ::HANDLE deviceHandle ) {
				infoSize = cbSize = sizeof(::RID_DEVICE_INFO);
				::GetRawInputDeviceInfoW( deviceHandle, RIDI_DEVICEINFO, this, &infoSize );
				::GetRawInputDeviceInfoW( deviceHandle, RIDI_DEVICENAME, name, &nameLength );
			}

			bool IsVirtualOrUnavailable() const {
				if( INVALID_HANDLE_VALUE == ::CreateFileW( name, 0u, (FILE_SHARE_READ | FILE_SHARE_WRITE), nullptr, OPEN_EXISTING, 0u, nullptr ) ) {
					return true;
				}

				return false;
			}

		// - DATA MEMBERS ------------------------------------

			::UINT	infoSize;
			::UINT	nameLength;
			wchar_t	name[MAX_PATH];
		};

	// ---

		const RawInputDeviceInfo	deviceInfo( deviceHandle );

		if( deviceInfo.IsVirtualOrUnavailable() ) {
			return;
		}

		switch( deviceInfo.dwType ) {
			case RIM_TYPEMOUSE: {
				GetLogger()( UTF8L("Attached mouse (handle %lX).") ET_UTF8_NEWLINE_LITERAL, reinterpret_cast<unsigned long>(deviceHandle) );
				new(_allocator, Allocator::AllocationOption::PERMANENT_ALLOCATION) EngineService::Mouse( deviceHandle, *this );
				break;
			}	// case RIM_TYPEMOUSE

		// ---

			case RIM_TYPEKEYBOARD: {
				GetLogger()( UTF8L("Attached keyboard (handle %lX).") ET_UTF8_NEWLINE_LITERAL, reinterpret_cast<unsigned long>(deviceHandle));
				new(_allocator, Allocator::AllocationOption::PERMANENT_ALLOCATION) EngineService::Keyboard( deviceHandle, *this );
				break;
			}	// case RIM_TYPEKEYBOARD

		// ---

			default: {
				GetLogger( LogMessageType::ERROR )( UTF8L("Received device attach notification for unknown HID type! (handle %lX).") ET_UTF8_NEWLINE_LITERAL, reinterpret_cast<unsigned long>(deviceHandle) );
				break;
			}	// default
		};	// switch( deviceInfo.dwType )
	}

}	// namespace Win32
}	// namespace Input
}	// namespace Eldritch2