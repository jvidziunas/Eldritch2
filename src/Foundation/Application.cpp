/*==================================================================*\
  Application.cpp
  ------------------------------------------------------------------
  Purpose:
  Definition for a basic application class that serves as a sort of
  'global namespace' for the execution context. This should be
  subclassed and extended for specific compile-time functionality.

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Foundation/Application.hpp>
#include <System/SystemInterface.hpp>
#include <Utility/ResultPair.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;
using namespace ::std;

namespace Eldritch2 {
namespace Foundation {

	Application::Application( SystemInterface& systemInterface, TaskScheduler& scheduler, Allocator& allocator ) : _allocator( allocator, UTF8L("Application Allocator") ),
																  												   _systemInterface( systemInterface ),
																  												   _scheduler( scheduler ),
																  												   _executionState( ExecutionState::CONTINUE ),
																												   _primaryWindow( nullptr ) {}

// ---------------------------------------------------

	Application::~Application() {
		if( _primaryWindow ) {
			_allocator.Delete( *_primaryWindow );
		}
	}

// ---------------------------------------------------

	SystemInterface& Application::GetSystemInterface() const {
		return _systemInterface;
	}

// ---------------------------------------------------

	Window* Application::GetPrimaryWindow() {
		if( !_primaryWindow ) {
			const Window::PresentationParameters	presentationParameters { Window::PresentationParameters::Mode::WINDOWED_FULLSCREEN, 1280, 720 };

			if( const auto result = GetSystemInterface().CreateAttachedWindow( _allocator, presentationParameters, GetTaskScheduler() ) ) {
				// Assigning this after the broadcast could result in reentrancy problems (i.e. a service testing if a newly-attached window is the primary one for some reason)
				// so make sure things are ready to go before we pass control off to the AttachWindow event
				_primaryWindow = result.object;
			}
		}

		return _primaryWindow;
	}

// ---------------------------------------------------

	void Application::RaiseTerminationFlag() const {
		_executionState.store( ExecutionState::HALT, memory_order_release );
	}

// ---------------------------------------------------

	bool Application::ShouldContinueExecution() const {
		return ExecutionState::CONTINUE == _executionState.load( memory_order_consume );
	}

}	// namespace Foundation
}	// namespace Eldritch2