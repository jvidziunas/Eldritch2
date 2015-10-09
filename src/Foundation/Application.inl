/*==================================================================*\
  Application.inl
  ------------------------------------------------------------------
  Purpose:
  Definition for a basic application class that serves as a sort of
  'global namespace' for the execution context. This should be
  subclassed and extended for specific compile-time functionality.

  ------------------------------------------------------------------
  ©2010-2012 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Foundation {

	namespace Foundation	= ::Eldritch2::Foundation;
	namespace Scheduler		= ::Eldritch2::Scheduler;

// ---------------------------------------------------

	ETInlineHint const Scheduler::TaskScheduler& Application::GetTaskScheduler() const {
		return _scheduler;
	}

// ---------------------------------------------------

	ETInlineHint Scheduler::TaskScheduler& Application::GetTaskScheduler() {
		return _scheduler;
	}

// ---------------------------------------------------

	ETInlineHint ::Eldritch2::Allocator& Application::GetAllocator() {
		return _allocator;
	}

}	// namespace Foundation
}	// namespace Eldritch2