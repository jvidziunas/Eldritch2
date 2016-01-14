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

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scheduler {

	template <class HostScheduler>
	PrivateQueueWorker<HostScheduler>::PrivateQueueWorker( HostScheduler& scheduler, ::Eldritch2::Allocator& allocator ) : Scheduler::WorkerContext( { []( void* scheduler, WorkerContext& context ) { static_cast<PrivateQueueWorker&>(context).AcquireReceiverInitiatedStolenTask( *static_cast<HostScheduler*>(scheduler) ); }, &scheduler },
																														   							 allocator ),
																														   _randomWorkerSeed( static_cast<size_t>(::__rdtsc()) ),
																														   _transferCell( BarrierStatus::COMPLETE ),
																														   _thiefCell( this ),
																														   _executionBehavior( ExecutionBehavior::CONTINUE ) {}

// ---------------------------------------------------

	template <class HostScheduler>
	const ::Eldritch2::UTF8Char* const PrivateQueueWorker<HostScheduler>::GetHumanReadableName() const {
		return UTF8L("Worker Thread");
	}

// ---------------------------------------------------

	template <class HostScheduler>
	ErrorCode PrivateQueueWorker<HostScheduler>::Run() {
		_thiefCell.store( nullptr, ::std::memory_order_release );

		while( ExecutionBehavior::TERMINATE != _executionBehavior.load( ::std::memory_order_relaxed ) ) {
			const auto	currentWorkItem( PopTask() );

			// Distribute any work (if it remains) to other worker threads if they ask for it.
			TryCompleteWorkSharingRequest();

			currentWorkItem.codePointer( currentWorkItem.data, *this );
		}

		// Mark ourselves as unable to be stolen from-- we're not producing work items any more!
		if( const auto thief = _thiefCell.exchange( this, ::std::memory_order_relaxed ) ) {
			thief->_transferCell.store( BarrierStatus::COMPLETE, ::std::memory_order_relaxed );
		}

		return Error::NONE;
	}

// ---------------------------------------------------

	template <class HostScheduler>
	void PrivateQueueWorker<HostScheduler>::RequestGracefulShutdown() {
		_executionBehavior.store( ExecutionBehavior::TERMINATE, ::std::memory_order_release );
	}

// ---------------------------------------------------

	template <class HostScheduler>
	ETInlineHint bool PrivateQueueWorker<HostScheduler>::TryBeginWorkSharingWithThief( PrivateQueueWorker& thief ) {
		PrivateQueueWorker*	expectedStealingWorker( nullptr );

		return _thiefCell.compare_exchange_weak( expectedStealingWorker, &thief, ::std::memory_order_release, ::std::memory_order_relaxed );
	}

// ---------------------------------------------------

	template <class HostScheduler>
	void PrivateQueueWorker<HostScheduler>::AcquireReceiverInitiatedStolenTask( HostScheduler& scheduler ) {
		// Prepare to receive work.
		_transferCell.store( BarrierStatus::AWAITING_TRANSFER, ::std::memory_order_relaxed );

		// Inform the worker that we could use some more work by writing our address into its transfer cell.
		if( scheduler.GetRandomWorkerThread( *this, _randomWorkerSeed ).TryBeginWorkSharingWithThief( *this ) ) {
			do {
				// Switch over to the other thread on systems with Hyper-Threading technology, as it might be able to respond/share work.
				::_mm_pause(); ::_mm_pause();
				// If anyone asks for some tasks, inform them that we don't have anything to share.
				if( const auto thief = _thiefCell.load( ::std::memory_order_relaxed ) ) {
					// Hand-inlining this instead of deferring to TryCompleteWorkSharingRequest() results in less busy time
					// (we're never going to have shareable tasks, and thusly we waste time trying to check), meaning we can
					// relinquish more processor time to other potentially work-creating threads.
					thief->_transferCell.store( BarrierStatus::COMPLETE, ::std::memory_order_release );
					_thiefCell.store( nullptr, ::std::memory_order_release );
				}
			} while( BarrierStatus::AWAITING_TRANSFER == _transferCell.load( ::std::memory_order_relaxed ) );
		}
	}

// ---------------------------------------------------

	template <class HostScheduler>
	void PrivateQueueWorker<HostScheduler>::TryCompleteWorkSharingRequest() {
		if( const auto thief = _thiefCell.load( ::std::memory_order_relaxed ) ) {
			// Note that this will (and should) not transfer the final steal work task in slot 0 of the work queue.
			const auto	stolenItemBegin( _workItems.End() - (_workItems.Size() / 2) );

			for( const WorkItem& workItem : Range<const WorkItem*> { stolenItemBegin, _workItems.ConstEnd() } ) {
				thief->_workItems.PushBack( workItem );
			}

			thief->_transferCell.store( BarrierStatus::COMPLETE, ::std::memory_order_release );
			// Don't need to compare here. Other threads will be able to steal once we're good and ready.
			_thiefCell.store( nullptr, ::std::memory_order_relaxed );

			_workItems.Erase( stolenItemBegin, _workItems.End() );
		}
	}

}	// namespace Scheduler
}	// namespace Eldritch2