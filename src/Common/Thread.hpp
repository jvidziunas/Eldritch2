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
#include <Common/Containers/AbstractStringView.hpp>
#include <Common/Atomic.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
class ErrorCode;
}

namespace Eldritch2 {

class ETPureAbstractHint Thread {
	// - TYPE PUBLISHING ---------------------------------

public:
	enum ExecutionState : uint32 {
		Uninitialized,
		Running,
		Terminated
	};

	// ---

	/*!	Implementation aid for @ref Thread implementations that involve processes that do not have built-in signaling mechanisms.
		Classes are encouraged to use an @ref Atomic member of this enum for self-documentation. */
	enum RunBehavior : uint32 {
		Terminate, //!< The @ref Thread should cease execution at the earliest possible opportunity.
		Continue   //!< The @ref Thread should continue execution.
	};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

protected:
	//!	Constructs this @ref Thread instance.
	Thread(const Thread&) ETNoexceptHint;
	//!	Constructs this @ref Thread instance.
	Thread() ETNoexceptHint;

	~Thread();

	// ---------------------------------------------------

public:
	ExecutionState GetState(MemoryOrder order = std::memory_order_consume) const ETNoexceptHint;

	//! Notifies this @ref Thread instance that the bound operating system thread should terminate.
	/*! @remark Implementations should be fully re-entrant, as clients may be call this method from multiple threads, multiple times on the same thread, or both. */
	virtual void SetShouldShutDown() ETNoexceptHint abstract;

	//! Notifies this Thread instance that it should terminate, and blocks the calling thread until it completes doing so.
	/*!	@remark The turnaround time for a full termination is very much unbounded. Where possible, try to call @ref Thread::SetShouldShutDown() ahead of time
			and then call this function at the last possible moment.
		@see @ref Thread::IsRunning() */
	void AwaitCompletion();

	// ---------------------------------------------------

public:
	ErrorCode BootOnCaller(AbstractStringView<Utf8Char> name);

	ErrorCode Boot(AbstractStringView<Utf8Char> name);

	// ---------------------------------------------------

private:
	virtual ErrorCode EnterOnCaller() abstract;

	// ---------------------------------------------------

private:
	static ETNoInlineHint void NameThread(AbstractStringView<Utf8Char> name) ETNoexceptHint;

	// - DATA MEMBERS ------------------------------------

private:
	/*! Marker used by the underlying operating system thread to indicate whether or not it has forked/joined. */
	Atomic<ExecutionState> _state;
};

Thread* GetCurrentThread() ETNoexceptHint;

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Thread.inl>
//------------------------------------------------------------------//
