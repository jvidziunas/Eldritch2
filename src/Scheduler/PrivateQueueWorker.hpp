/*==================================================================*\
  PrivateQueueWorker.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduler/WorkerContext.hpp>
#include <Scheduler/Thread.hpp>
//------------------------------------------------------------------//
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scheduler {

	template <class HostScheduler>
	class PrivateQueueWorker : public Scheduler::Thread, public Scheduler::WorkerContext {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref PrivateQueueWorker instance.
		/*!	@param[in] scheduler The @ref HostScheduler instance that will own the new @ref PrivateQueueWorker.
			@param[in] allocator The @ref Allocator the @ref PrivateQueueWorker will use to perform internal allocations.
			*/
		PrivateQueueWorker( HostScheduler& scheduler, ::Eldritch2::Allocator& allocator );

		~PrivateQueueWorker() = default;

	// ---------------------------------------------------

		const ::Eldritch2::UTF8Char* const	GetHumanReadableName() const override sealed;

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	Run() override sealed;

		void					RequestGracefulShutdown() override;

	// ---------------------------------------------------

	protected:
		ETInlineHint bool	TryBeginWorkSharingWithThief( PrivateQueueWorker& thief );

		void				AcquireReceiverInitiatedStolenTask( HostScheduler& scheduler );

		void				TryCompleteWorkSharingRequest();

	// - TYPE PUBLISHING ---------------------------------

	private:
		enum class BarrierStatus : ::Eldritch2::uint32 {
			AWAITING_TRANSFER,
			COMPLETE,
			AVAILABLE
		};

	// ---

		enum class ExecutionBehavior : uint32 {
			TERMINATE,
			CONTINUE
		};

	// - DATA MEMBERS ------------------------------------

		::std::atomic<PrivateQueueWorker<HostScheduler>*>	_thiefCell;
		size_t												_randomWorkerSeed;
		::std::atomic<ExecutionBehavior>					_executionBehavior;
		::std::atomic<BarrierStatus>						_transferCell;
	};

}	// namespace Scheduler
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scheduler/PrivateQueueWorker.inl>
//------------------------------------------------------------------//