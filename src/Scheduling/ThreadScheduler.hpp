/*==================================================================*\
  ThreadScheduler.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scheduling {
		class	Thread;
	}

	class	ErrorCode;
}

namespace Eldritch2 {
namespace Scheduling {

	enum class ThreadExecutionPriority {
		BelowNormal,
		Normal,
		AboveNormal,
		Priority,

		COUNT
	};

// ---

	using BackoffContext = Eldritch2::uintptr;

// ---------------------------------------------------

	class ETPureAbstractHint ThreadScheduler {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ThreadScheduler instance.
		ThreadScheduler( const ThreadScheduler& ) = default;
	//!	Constructs this @ref ThreadScheduler instance.
		ThreadScheduler() = default;

		~ThreadScheduler() = default;

	// - WORK SCHEDULING ---------------------------------

	public:
		virtual Eldritch2::ErrorCode	EnqueueOnCaller( Scheduling::Thread& thread ) abstract;

	//!	Attempts to assign exclusive ownership of the denoted @ref Module instance to this @ref ThreadScheduler.
	/*!	@remarks @parblock If the operation succeeds, the associated code for the @ref Module will be executed continually
		on a unique operating system thread until it is either dequeued from the @ref ThreadScheduler or manually told to
		cease execution. @endparblock
		@param[in] thread @ref Thread that will be executed.
		@returns Errors::None if the operation succeeded, or an @ref ErrorCode containing information on why the operation failed. */
		virtual Eldritch2::ErrorCode	Enqueue( Scheduling::Thread& thread ) abstract;

	// ---------------------------------------------------

	public:
		virtual void	ShutDown() abstract;

	// ---------------------------------------------------

	public:
		virtual void	Backoff( Scheduling::BackoffContext& context );

	// ---------------------------------------------------

	public:
		virtual size_t	GetMaximumParallelism() const abstract;

	// ---------------------------------------------------

	public:
		virtual void	SetCallerExecutionPriority( Scheduling::ThreadExecutionPriority priority );
	};

}	// namespace Scheduling
}	// namespace Eldritch2