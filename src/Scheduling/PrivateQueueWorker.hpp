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
#include <Scheduling/JobFiber.hpp>
#include <Scheduling/Thread.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scheduling {

	template <class HostScheduler>
	class PrivateQueueWorker : public Scheduling::Thread, public Scheduling::JobFiber {
	// - TYPE PUBLISHING ---------------------------------

	private:
		enum class BarrierStatus : Eldritch2::uint32 {
			AwaitingTransfer,
			Complete,
			Available
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref PrivateQueueWorker instance.
	/*!	@param[in] defaultWorkItem Work item that will be run when no additional tasks are available to the context.
		@param[in] allocator The @ref Allocator the @ref PrivateQueueWorker will use to perform internal allocations. */
		PrivateQueueWorker( const WorkItem& defaultWorkItem, Eldritch2::Allocator& allocator );
	//!	Constructs this @ref PrivateQueueWorker instance.
		PrivateQueueWorker( const PrivateQueueWorker& ) = delete;
	protected:
	//!	Constructs this @ref PrivateQueueWorker instance.
		PrivateQueueWorker( PrivateQueueWorker&& );

	public:
		~PrivateQueueWorker() = default;

	// ---------------------------------------------------

		Eldritch2::Utf8Literal	GetHumanReadableName() const override;

	// ---------------------------------------------------

		void	RequestGracefulShutdown() override;

		void	Run() override;

	// ---------------------------------------------------

		virtual void	OnBegin();

		virtual void	OnTerminate();

	// ---------------------------------------------------

	protected:
		virtual void	CompleteWorkSharingRequest( PrivateQueueWorker& thief );

		bool			TryBeginWorkSharingWithThief( PrivateQueueWorker& thief );

	// ---------------------------------------------------

		void	AcquireReceiverInitiatedStolenTask( HostScheduler& scheduler );

	// - DATA MEMBERS ------------------------------------

	private:
		std::atomic<PrivateQueueWorker<HostScheduler>*>	_thief;
		size_t											_randomWorkerSeed;
		std::atomic<TerminationBehavior>				_terminationBehavior;
		std::atomic<BarrierStatus>						_transferCell;
	};

}	// namespace Scheduling
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scheduling/PrivateQueueWorker.inl>
//------------------------------------------------------------------//