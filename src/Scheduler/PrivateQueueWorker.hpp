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

namespace Eldritch2 {
namespace Scheduler {

	template <class HostScheduler>
	class PrivateQueueWorker : public Scheduler::Thread, public Scheduler::WorkerContext {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref PrivateQueueWorker instance.
		/*!	@param[in] defaultWorkItem Work item that will be run when no additional tasks are available to the context.
			@param[in] allocator The @ref Allocator the @ref PrivateQueueWorker will use to perform internal allocations.
			*/
		PrivateQueueWorker( const WorkItem& defaultWorkItem, ::Eldritch2::Allocator& allocator );
		//!	Constructs this @ref PrivateQueueWorker instance.
		PrivateQueueWorker( const PrivateQueueWorker& ) = delete;
	protected:
		//!	Constructs this @ref PrivateQueueWorker instance.
		PrivateQueueWorker( PrivateQueueWorker&& );

	public:
		~PrivateQueueWorker() = default;

	// ---------------------------------------------------

		const ::Eldritch2::UTF8Char* const	GetHumanReadableName() const override sealed;

	// ---------------------------------------------------

		void	Run() override;

		void	RequestGracefulShutdown() override;

	// ---------------------------------------------------

		virtual void	OnBegin();

		virtual void	OnTerminate();

	// ---------------------------------------------------

	protected:
		ETInlineHint bool	TryBeginWorkSharingWithThief( PrivateQueueWorker& thief );

		virtual void		CompleteWorkSharingRequest( PrivateQueueWorker& thief );

	// ---------------------------------------------------

		void	AcquireReceiverInitiatedStolenTask( HostScheduler& scheduler );

	// - TYPE PUBLISHING ---------------------------------

	private:
		enum class BarrierStatus : ::Eldritch2::uint32 {
			AWAITING_TRANSFER,
			COMPLETE,
			AVAILABLE
		};

	// ---

		enum class ExecutionBehavior : ::Eldritch2::uint32 {
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