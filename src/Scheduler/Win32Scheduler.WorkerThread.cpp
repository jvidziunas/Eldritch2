/*==================================================================*\
  Win32Scheduler.WorkerThread.cpp
  ------------------------------------------------------------------
  Purpose:
  Provides a scheduler implementation policy that wraps Windows
  specific implementations of job queuing functionalities.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduler/Win32Scheduler.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;
using namespace ::std;

namespace {

	enum : uint32 {
		TERMINATE_EXECUTION,
		CONTINUE_EXECUTION
	};

}	// anonymous namespace

namespace Eldritch2 {
namespace Scheduler {

	Win32Scheduler::WorkerThread::WorkerThread( Win32Scheduler& scheduler ) : WorkerContext( { [] ( void* scheduler, WorkerContext& context ) { static_cast<WorkerThread&>(context).AcquireReceiverInitiatedStolenTask( *static_cast<Win32Scheduler*>(scheduler) ); }, &scheduler }, scheduler._allocator ),
																			  _randomWorkerSeed( static_cast<size_t>(::__rdtsc()) ),
																			  _transferBarrier( BarrierStatus::COMPLETE ),
																			  _stealingWorker( this ),
																			  _killFlag( CONTINUE_EXECUTION ) {}

// ---------------------------------------------------

	const UTF8Char* const Win32Scheduler::WorkerThread::GetHumanReadableName() const {
		return UTF8L("Win32 Worker Thread");
	}

// ---------------------------------------------------

	ErrorCode Win32Scheduler::WorkerThread::Run() {
		_stealingWorker.store( nullptr, memory_order_release );

		while( TERMINATE_EXECUTION != _killFlag.load( memory_order_consume ) ) {
			const WorkItem	currentWorkItem( PopTask() );
			// Distribute any work (if it remains) to other worker threads if they ask for it.
			TryCompleteWorkSharingRequest();

			currentWorkItem.codePointer( currentWorkItem.data, *this );
		}

		if( WorkerThread* const stealingWorker = _stealingWorker.exchange( this, memory_order_acquire ) ) {
			stealingWorker->_transferBarrier.store( BarrierStatus::COMPLETE, memory_order_release );
		}

		return Error::NONE;
	}

// ---------------------------------------------------

	void Win32Scheduler::WorkerThread::RequestGracefulShutdown() {
		_killFlag.store( TERMINATE_EXECUTION, memory_order_release );
	}

// ---------------------------------------------------

	void Win32Scheduler::WorkerThread::AcquireReceiverInitiatedStolenTask( Win32Scheduler& scheduler ) {
		WorkerThread&	victim( scheduler.GetRandomWorkerThread( *this, _randomWorkerSeed ) );
		WorkerThread*	expectedStealingWorker( nullptr );

		_transferBarrier.store( BarrierStatus::AWAITING_TRANSFER, memory_order_relaxed );

		// Inform the worker that we could use some more work by writing our address into its transfer cell.
		if( victim._stealingWorker.compare_exchange_weak( expectedStealingWorker, this, memory_order_release, memory_order_relaxed ) ) {
			do {
				// Switch over to the other thread on systems with Hyper-Threading technology, as it might be able to respond/share work.
				::_mm_pause(); ::_mm_pause();
				// If anyone asks for some tasks, inform them that we don't have anything to share.
				if( WorkerThread* const stealingWorker = _stealingWorker.load( memory_order_acquire ) ) {
					// Hand-inlining this instead of deferring to TryCompleteWorkSharingRequest() results in less busy time
					// (we're never going to have shareable tasks, and thusly we waste time trying to check), meaning we can
					// relinquish more processor time to other potentially work-creating threads.
					stealingWorker->_transferBarrier.store( BarrierStatus::COMPLETE, memory_order_release );
					_stealingWorker.store( nullptr, memory_order_release );
				}
			} while( BarrierStatus::AWAITING_TRANSFER == _transferBarrier.load( memory_order_consume ) );
		}
	}

// ---------------------------------------------------

	void Win32Scheduler::WorkerThread::TryCompleteWorkSharingRequest() {
		if( WorkerThread* const thiefWorker = _stealingWorker.load( memory_order_relaxed ) ) {
			// Note that this will (and should) not transfer the final steal work task in slot 0 of the work queue.
			WorkItem* const	stolenItemBegin( _workItems.End() - (_workItems.Size() / 2) );
			
			for( const WorkItem& workItem : Range<const WorkItem*>( stolenItemBegin, _workItems.ConstEnd() ) ) {
				thiefWorker->_workItems.EmplaceBack( workItem );
			}

			thiefWorker->_transferBarrier.store( BarrierStatus::COMPLETE, memory_order_release );
			_stealingWorker.store( nullptr, memory_order_relaxed );

			_workItems.Erase( stolenItemBegin, _workItems.End() );
		}
	}

}	// namespace Scheduler
}	// namespace Eldritch2