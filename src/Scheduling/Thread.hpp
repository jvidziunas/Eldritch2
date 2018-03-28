/*==================================================================*\
  Thread.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scheduling {

	class ETPureAbstractHint Thread {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum ExecutionState : uint32 {
			Uninitialized,
			Running,
			Done
		};

	// ---

	/*!	Implementation aid for @ref Thread implementations that involve processes that do not have built-in signaling mechanisms.
	 	Classes are encouraged to use an std::atomic member of this enum for self-documentation. */
		enum RunBehavior : uint32 {
			Terminate,	//!< The @ref Thread should cease execution at the earliest possible opportunity.
			Continue	//!< The @ref Thread should continue execution.
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
	//!	Constructs this @ref Thread instance.
		Thread( const Thread& );
	//!	Constructs this @ref Thread instance.
		Thread();

		~Thread();

	// ---------------------------------------------------

	public:
	//! Retrieves a pointer to a null-terminated character array containing the UTF8-encoded name of this @ref Thread. */
		virtual Utf8Literal	GetName() const abstract;

	// ---------------------------------------------------

	public:
	//! Checks to see if this @ref Thread instance has been scheduled on a concrete operating system thread.
	/*!	@returns A boolean value indicating whether the @ref Thread is executing currently or has completed at some point in the past. */
		bool	HasStarted( std::memory_order order = std::memory_order_consume ) const;

	//! Checks to see if this @ref Thread instance is still scheduled on a concrete operating system thread.
		bool	IsRunning( std::memory_order order = std::memory_order_consume ) const;

	//! Notifies this Thread instance that it should terminate, and blocks the calling thread until it completes doing so.
	/*!	@remark The turnaround time for a full termination is very much unbounded. Where possible, try to call @ref Thread::ShutDown() ahead of time and then call this function at the last possible moment.
		@see @ref Thread::ShutDown()
		@see @ref Thread::IsRunning() */
		void	AwaitCompletion();

	// ---------------------------------------------------

	public:
	//! Notifies this @ref Thread instance that the bound operating system thread should terminate.
	/*! @remark Implementations should be fully re-entrant, as this may be called from multiple threads, multiple times on the same thread, or both. */
		virtual void	SetShouldShutDown() abstract;

		void			Enter();

	// ---------------------------------------------------

	protected:
		virtual void	Run() abstract;

	// - DATA MEMBERS ------------------------------------

	private:
	/*! Marker used by the underlying operating system thread to indicate whether or not it has forked/joined. */
		Atomic<ExecutionState>	_state;
	};

}	// namespace Scheduling
}	// namespace Eldritch2