/*==================================================================*\
  Application.hpp
  ------------------------------------------------------------------
  Purpose:
  Definition for a basic application class that serves as a sort of
  'global namespace' for the execution context. This should be
  subclassed and extended for specific compile-time functionality.

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/ChildAllocator.hpp>
#include <Utility/MPL/Noncopyable.hpp>
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Foundation {
		class	SystemInterface;
		class	Window;
	}

	namespace Scheduler {
		class	TaskScheduler;
	}
}

namespace Eldritch2 {
namespace Foundation {

	namespace Foundation	= ::Eldritch2::Foundation;
	namespace Scheduler		= ::Eldritch2::Scheduler;
	namespace Utility		= ::Eldritch2::Utility;

// ---------------------------------------------------

	class Application : public Utility::Noncopyable {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Initializes this Application instance.
		Application( Foundation::SystemInterface& systemInterface, Scheduler::TaskScheduler& scheduler, ::Eldritch2::Allocator& allocator );

		// Destroys this Application instance.
		~Application();

	// - PROGRAM EXECUTION CONTROL -----------------------

		bool	ShouldContinueExecution() const;

		// Signals to the application that the program should begin the termination procedure and cease execution.
		void	RaiseTerminationFlag() const;

	// ---------------------------------------------------

		// Retrieves a read-only view of the TaskScheduler instance this Application runs threads on.
		ETInlineHint const Scheduler::TaskScheduler&	GetTaskScheduler() const;
		// Retrieves the TaskScheduler instance this Application runs threads on.
		ETInlineHint Scheduler::TaskScheduler&			GetTaskScheduler();

		// Retrieves a reference to the system interface describing the hardware this Application instance is executing on.
		ETInlineHint Foundation::SystemInterface&		GetSystemInterface() const;

		ETInlineHint ::Eldritch2::Allocator&			GetAllocator();

	// - OPERATING SYSTEM WINDOW MANAGEMENT --------------

		Foundation::Window*	GetPrimaryWindow();

	// ---------------------------------------------------

	private:
		enum class ExecutionState : ::Eldritch2::uint32 {
			CONTINUE,
			HALT
		};

	// - DATA MEMBERS ------------------------------------

		::Eldritch2::ChildAllocator				_allocator;

		Foundation::SystemInterface&			_systemInterface;
		Scheduler::TaskScheduler&				_scheduler;

		mutable ::std::atomic<ExecutionState>	_executionState;
		Foundation::Window*						_primaryWindow;
	};

}	// namespace Foundation
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Foundation/Application.inl>
//------------------------------------------------------------------//