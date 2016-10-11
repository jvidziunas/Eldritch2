/*==================================================================*\
  PrivateQueueWorker.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scheduling {

	template <class HostScheduler>
	PrivateQueueWorker<HostScheduler>::PrivateQueueWorker( const WorkItem& defaultWorkItem, Eldritch2::Allocator& allocator ) : JobFiber( defaultWorkItem, allocator ),
																																_randomWorkerSeed( static_cast<size_t>(__rdtsc()) ),
																																_transferCell( BarrierStatus::Complete ),
																																_thief( this ),
																																_terminationBehavior( TerminationBehavior::Continue ) {}

// ---------------------------------------------------

	template <class HostScheduler>
	PrivateQueueWorker<HostScheduler>::PrivateQueueWorker( PrivateQueueWorker<HostScheduler>&& source ) : JobFiber( eastl::forward<JobFiber>( source ) ) {}

// ---------------------------------------------------

	template <class HostScheduler>
	Eldritch2::Utf8Literal PrivateQueueWorker<HostScheduler>::GetHumanReadableName() const {
		return "Worker Thread";
	}

// ---------------------------------------------------

	template <class HostScheduler>
	void PrivateQueueWorker<HostScheduler>::Run() {
		OnBegin();

		_thief.store( nullptr, std::memory_order_release );

		while( TerminationBehavior::Terminate != _terminationBehavior.load( std::memory_order_consume ) ) {
			const auto	completionItem( PopCompletionItem() );

		//	Distribute any work (if it remains) to other worker threads if they ask for it.
			if( const auto thief = _thief.load( std::memory_order_consume ) ) {
			//	Don't need to compare here.
				_thief.store( nullptr, std::memory_order_release );

				CompleteWorkSharingRequest( *thief );
			}

			completionItem.workItem.function( completionItem.workItem.argument, *this );

			if( completionItem.barrier ) {
				completionItem.barrier->fetch_sub( 1, std::memory_order_acq_rel );
			}
		}

	//	Mark ourselves as unable to be stolen from-- we're not producing work items any more!
		if( const auto thief = _thief.exchange( this, std::memory_order_acquire ) ) {
			thief->_transferCell.store( BarrierStatus::Complete, std::memory_order_release );
		}

		OnTerminate();
	}

// ---------------------------------------------------

	template <class HostScheduler>
	void PrivateQueueWorker<HostScheduler>::RequestGracefulShutdown() {
		_terminationBehavior.store( TerminationBehavior::Terminate, std::memory_order_release );
	}

// ---------------------------------------------------

	template <class HostScheduler>
	void PrivateQueueWorker<HostScheduler>::OnBegin() {
	//	Default implementation does nothing.
	}

// ---------------------------------------------------

	template <class HostScheduler>
	virtual void PrivateQueueWorker<HostScheduler>::OnIdle() {
	//	Default implementation does nothing.
	}

// ---------------------------------------------------

	template <class HostScheduler>
	void PrivateQueueWorker<HostScheduler>::OnTerminate() {
	//	Default implementation does nothing.
	}

// ---------------------------------------------------

	template <class HostScheduler>
	ETInlineHint bool PrivateQueueWorker<HostScheduler>::TryBeginWorkSharingWithThief( PrivateQueueWorker& thief ) {
		PrivateQueueWorker*	expectedStealingWorker( nullptr );

		return _thief.compare_exchange_weak( expectedStealingWorker, &thief, std::memory_order_release, std::memory_order_relaxed );
	}

// ---------------------------------------------------

	template <class HostScheduler>
	void PrivateQueueWorker<HostScheduler>::AcquireReceiverInitiatedStolenTask( HostScheduler& scheduler ) {
	//	Prepare to receive work.
		_transferCell.store( BarrierStatus::AwaitingTransfer, std::memory_order_relaxed );

	//	Inform the worker that we could use some more work by writing our address into its transfer cell.
		if( scheduler.GetRandomWorkerThread( *this, _randomWorkerSeed ).TryBeginSharing( *this ) ) {
			do {
			//	Switch over to the other thread on systems with Hyper-Threading technology, as it might be able to respond/share work.
				_mm_pause(); _mm_pause();
			//	If anyone asks for some tasks, inform them that we don't have anything to share.
				if( const auto thief = _thief.load( std::memory_order_consume ) ) {
					_thief.store( nullptr, std::memory_order_release );

				//	Hand-inlining this instead of deferring to TryCompleteWorkSharingRequest() results in less busy time
				//	(we're never going to have shareable tasks, and thusly we waste time trying to check), meaning we can
				//	relinquish more processor time to other potentially work-creating threads.
					thief->_transferCell.store( BarrierStatus::Complete, std::memory_order_relaxed );
				}
			} while( BarrierStatus::AwaitingTransfer == _transferCell.load( std::memory_order_consume ) );
		}
	}

// ---------------------------------------------------

	template <class HostScheduler>
	void PrivateQueueWorker<HostScheduler>::CompleteWorkSharingRequest( PrivateQueueWorker& thief ) {
	//	Note that this will (and should) not transfer the final steal work task in slot 0 of the work queue.
		const auto	stolenItemBegin( _completionItems.End() - (_completionItems.GetSize() / 2) );

		for( const auto& completion : Range<const CompletionItem*> { stolenItemBegin, _completionItems.ConstEnd() } ) {
			thief._completionItems.PushBack( completion );
		}

	//	Ensure all our writes complete before signaling a finished state.
		thief._transferCell.store( BarrierStatus::Complete, std::memory_order_release );

		_completionItems.Erase( stolenItemBegin, _completionItems.End() );
	}

}	// namespace Scheduling
}	// namespace Eldritch2