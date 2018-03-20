/*==================================================================*\
  JobSystem.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scheduling {
		class	Thread;
	}
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

	using BackoffContext = uintptr;

// ---------------------------------------------------

	class ETPureAbstractHint JobSystem {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref JobSystem instance.
		JobSystem( const JobSystem& ) = default;
	//!	Constructs this @ref JobSystem instance.
		JobSystem() = default;

		~JobSystem() = default;

	// - WORK SCHEDULING ---------------------------------

	public:
		virtual ErrorCode	LaunchOnCaller( Thread& thread ) abstract;

	//!	Attempts to assign exclusive ownership of the denoted @ref Thread instance to this @ref JobSystem.
	/*!	@remarks @parblock If the operation succeeds, the associated code for the @ref Thread will be executed continually
		on a unique operating system thread until it is either dequeued from the @ref JobSystem or manually told to
		cease execution. @endparblock
		@param[in] thread @ref Thread that will be executed.
		@returns Errors::None if the operation succeeded, or an @ref ErrorCode containing information on why the operation failed. */
		virtual ErrorCode	Launch( Thread& thread ) abstract;

	// ---------------------------------------------------

	public:
		virtual void	SetShouldShutDown( int shutdownCode ) abstract;

	// ---------------------------------------------------

	public:
		virtual void	BackOff( BackoffContext& context );

	// ---------------------------------------------------

	public:
		virtual size_t	GetMaximumParallelism() const abstract;

	// ---------------------------------------------------

	public:
		virtual void	SetCallerExecutionPriority( ThreadExecutionPriority priority );
	};

}	// namespace Scheduling
}	// namespace Eldritch2