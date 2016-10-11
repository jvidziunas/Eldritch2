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
#include <Scheduling/ThreadScheduler.hpp>
#include <Input/Win32/EngineService.hpp>
#include <Scripting/ApiRegistrar.hpp>
#include <Scheduling/JobFiber.hpp>
#include <Utility/ErrorCode.hpp>
#include <Utility/Assert.hpp>
#include <Core/Engine.hpp>
//------------------------------------------------------------------//
#include <windowsx.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scheduling;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Logging;
using namespace ::Eldritch2::Core;

namespace Eldritch2 {
namespace Input {
namespace Win32 {
namespace {

	static LRESULT CALLBACK WindowsKeyHook( int nCode, WPARAM wParam, LPARAM lParam ) {
		const KBDLLHOOKSTRUCT&	hook( *reinterpret_cast<KBDLLHOOKSTRUCT*>( lParam ) );
		bool					eatKeystroke( false );

		if( ETBranchLikelyHint( nCode >= HC_ACTION ) ) {
			switch( wParam ) {
				case WM_KEYDOWN:
				case WM_KEYUP:
				{
				//	First, check to see if the windows key is specifically what's pressed.
					const bool	isWindowsKey( (VK_LWIN == hook.vkCode) | (VK_RWIN == hook.vkCode) );
				//	Next, verify that this was an actual keypress, *not* a specially-injected Windows key event.
				//	We're interested in stopping the cases whereupon a user accidentally fat-fingers the Windows key
				//	when they actually meant to press the control or alt keys.
					eatKeystroke = isWindowsKey & !(hook.dwExtraInfo & LLMHF_INJECTED);
					break;
				}	// case WM_KEYDOWN, case WM_KEYUP
			}	// switch( wParam )

			if( ETBranchUnlikelyHint( eatKeystroke ) ) {
				return 1;
			}
		}

	//	We have been instructed to not process this hook, simply pass it along.
		return CallNextHookEx( nullptr, nCode, wParam, lParam );
	}

}	// anonymous namespace

	EngineService::EngineService( const Engine& engine ) : Core::EngineService( engine.GetServiceBlackboard() ),
														   _log( engine.GetLog() ),
														   _keyboardHook( SetWindowsHookExW( WH_KEYBOARD_LL, &WindowsKeyHook, GetModuleHandleW( nullptr ), 0 ) ),
														   _pollingThread( engine.GetAllocator() ) {}

// ---------------------------------------------------

	EngineService::~EngineService() {
		_pollingThread.AwaitCompletion();

	//	Remove the hook that allowed us to selectively ignore Windows key press events.
		if( _keyboardHook ) {
			UnhookWindowsHookEx( _keyboardHook );
		}
	}

// ---------------------------------------------------

	Utf8Literal EngineService::GetName() const {
		return "Win32 Raw Input Manager";
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( ApiRegistrar& /*visitor*/ ) {}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( JobFiber& /*fiber*/, const BeginInitializationVisitor ) {
		_log( MessageSeverity::Message, "Initializing input service." ET_UTF8_NEWLINE_LITERAL );

		const ErrorCode	enqueueResult( LocateService<ThreadScheduler>().Enqueue( _pollingThread ) );
		if( !enqueueResult ) {
			_log( MessageSeverity::Error, "Error initializing input service: {}!" ET_UTF8_NEWLINE_LITERAL, GetErrorString( enqueueResult ) );
			return;
		}

		_log( MessageSeverity::Message, "Initialized event polling thread." ET_UTF8_NEWLINE_LITERAL );
	}

}	// namespace Win32
}	// namespace Input
}	// namespace Eldritch2