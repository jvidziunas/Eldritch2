/*==================================================================*\
  FiberScheduler.WorkerThread.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduler/Win32/FiberScheduler.hpp>
#include <Utility/Algorithms.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace {

	static ETThreadLocal Scheduler::Win32::FiberScheduler::WorkerThread*	activeWorkerThread = nullptr;

}	// anonymous namespace

namespace Eldritch2 {
namespace Scheduler {
namespace Win32 {

	FiberScheduler::WorkerThread::WorkerThread( FiberScheduler& scheduler, ::Eldritch2::Allocator& allocator ) : WorkerContext( { &scheduler._dummyCounter, { &scheduler, [] ( void* scheduler, WorkerContext& executingContext ) {
																													static_cast<WorkerThread&>(executingContext).AcquireReceiverInitiatedStolenTask( *static_cast<FiberScheduler*>(scheduler) );
																												 } } }, allocator ),
																												 _thiefCell( this ),
																												 _randomWorkerSeed( static_cast<size_t>(::__rdtsc()) ),
																												 _executionBehavior( ExecutionBehavior::Continue ),
																												 _transferCell( BarrierStatus::Complete ),
																												 _suspendedContexts( { allocator, UTF8L("Suspended Context Collection Allocator") } ),
																												 _pooledFibers( { allocator, UTF8L("Pooled Fiber Collection Allocator") } ),
																												 _initialFiber( nullptr ),
																												 _targetFiber( nullptr ),
																												 _switchFiber( nullptr ) {
		// Active worker thread can't be updated here, as there's no guarantee this function is being run on the correct operating system thread yet.
		// Create fibers, etc. during Run() for more concurrency.
		_suspendedContexts.Reserve( 16 );
		_pooledFibers.Reserve( 16 );
	}

// ---------------------------------------------------

	FiberScheduler::WorkerThread::~WorkerThread() {
		::DeleteFiber( _counterWaitFiber );
		::DeleteFiber( _switchFiber );

		while( _pooledFibers ) {
			::DeleteFiber( _pooledFibers.Back() );
			_pooledFibers.PopBack();
		}
	}

// ---------------------------------------------------

	FiberScheduler::WorkerThread::WorkerThread( WorkerThread&& source ) : WorkerContext( ::std::move( source ) ),
																		  _thiefCell( this ),
																		  _randomWorkerSeed( static_cast<size_t>(::__rdtsc()) ),
																		  _executionBehavior( ExecutionBehavior::Continue ),
																		  _transferCell( BarrierStatus::Complete ),
																		  _suspendedContexts( ::std::move( source._suspendedContexts ) ),
																		  _pooledFibers( ::std::move( _pooledFibers ) ),
																		  _initialFiber( ::std::move( source._initialFiber ) ),
																		  _targetFiber( ::std::move( source._targetFiber ) ),
																		  _switchFiber( ::std::move( source._switchFiber ) ),
																		  _waitingTaskStagingArea( ::std::move( source._waitingTaskStagingArea ) ) {}

// ---------------------------------------------------

	const ::Eldritch2::UTF8Char* const FiberScheduler::WorkerThread::GetHumanReadableName() const {
		return UTF8L("Worker Thread");
	}

// ---------------------------------------------------

	void FiberScheduler::WorkerThread::Run() {
		enum : SIZE_T {
			SupportFiberStackSizeInBytes	= (32768u * 4u),
			FiberStackSizeInBytes			= 524288u,
			FiberCount						= 15u
		};

	// ---

		activeWorkerThread = this;

		_counterWaitFiber	= ::CreateFiberEx( 0u, SupportFiberStackSizeInBytes, FIBER_FLAG_FLOAT_SWITCH, [] ( void* workerThread ) {
			for( ;; ) {
				// Fancy loop trick will cause execution to jump back around to here for the next caller after switching away.

				// Commit the previous fiber into the suspended context list.
				static_cast<WorkerThread*>(workerThread)->_suspendedContexts.PushBack( static_cast<WorkerThread*>(workerThread)->_waitingTaskStagingArea );

				::SwitchToFiber( static_cast<WorkerThread*>(workerThread)->_targetFiber );
			}
		}, this );
		_switchFiber		= ::CreateFiberEx( 0u, SupportFiberStackSizeInBytes, FIBER_FLAG_FLOAT_SWITCH, [] ( void* workerThread ) {
			for( ;; ) {
				// Fancy loop trick will cause execution to jump back around to here for the next caller after switching away.

				// Commit the previous fiber into the suspended context list.
				static_cast<WorkerThread*>(workerThread)->_pooledFibers.PushBack( static_cast<WorkerThread*>(workerThread)->_waitingTaskStagingArea.fiber );

				::SwitchToFiber( static_cast<WorkerThread*>(workerThread)->_targetFiber );
			}
		}, this );

		for( size_t fiberIndex( 0u ); fiberIndex < FiberCount; ++fiberIndex ) {
			_pooledFibers.PushBack( ::CreateFiberEx( 0u, FiberStackSizeInBytes, FIBER_FLAG_FLOAT_SWITCH, [] ( void* workerThread ) {
				static_cast<WorkerThread*>(workerThread)->FiberEntryPoint();
			}, this ) );
		}

		_initialFiber = ::ConvertThreadToFiberEx( this, FIBER_FLAG_FLOAT_SWITCH );

		FiberEntryPoint();

		::ConvertFiberToThread();

		activeWorkerThread = nullptr;
	}

// ---------------------------------------------------

	void FiberScheduler::WorkerThread::RequestGracefulShutdown() {
		_executionBehavior.store( ExecutionBehavior::Terminate, ::std::memory_order_release );
	}

// ---------------------------------------------------

	FiberScheduler::WorkerThread* FiberScheduler::WorkerThread::GetActiveWorkerThread() {
		return activeWorkerThread;
	}

// ---------------------------------------------------

	ETInlineHint bool FiberScheduler::WorkerThread::TryBeginWorkSharingWithThief( WorkerThread& thief ) {
		WorkerThread*	expectedStealingWorker( nullptr );

		return _thiefCell.compare_exchange_weak( expectedStealingWorker, &thief, ::std::memory_order_release, ::std::memory_order_relaxed );
	}

// ---------------------------------------------------

	void FiberScheduler::WorkerThread::AcquireReceiverInitiatedStolenTask( FiberScheduler& scheduler ) {
		// Prepare to receive work.
		_transferCell.store( BarrierStatus::AwaitingTransfer, ::std::memory_order_relaxed );

		// Inform the worker that we could use some more work by writing our address into its transfer cell.
		if( scheduler.GetRandomWorkerThread( *this, _randomWorkerSeed ).TryBeginWorkSharingWithThief( *this ) ) {
			do {
				// Switch over to the other thread on systems with Hyper-Threading technology, as it might be able to respond/share work.
				::_mm_pause(); ::_mm_pause();
				// If anyone asks for some tasks, inform them that we don't have anything to share.
				if( const auto thief = _thiefCell.load( ::std::memory_order_consume ) ) {
					_thiefCell.store( nullptr, ::std::memory_order_release );

					// Hand-inlining this instead of deferring to TryCompleteWorkSharingRequest() results in less busy time
					// (we're never going to have shareable tasks, and thusly we waste time trying to check), meaning we can
					// relinquish more processor time to other potentially work-creating threads.
					thief->_transferCell.store( BarrierStatus::Complete, ::std::memory_order_relaxed );
				}
			} while( BarrierStatus::AwaitingTransfer == _transferCell.load( ::std::memory_order_consume ) );
		}
	}

// ---------------------------------------------------

	void FiberScheduler::WorkerThread::CompleteWorkSharingRequest( WorkerThread& thief ) {
		// Note that this will (and should) not transfer the final steal work task in slot 0 of the work queue.
		const auto	stolenItemBegin( _completionItems.End() - (_completionItems.Size() / 2) );

		for( const auto& completion : Range<const CompletionItem*> { stolenItemBegin, _completionItems.ConstEnd() } ) {
			thief._completionItems.PushBack( completion );
		}

		// Ensure all our writes complete before signaling a finished state.
		thief._transferCell.store( BarrierStatus::Complete, ::std::memory_order_release );

		_completionItems.Erase( stolenItemBegin, _completionItems.End() );
	}

// ---------------------------------------------------

	void FiberScheduler::WorkerThread::SwitchFibers( const SystemFiber targetFiber ) {
		_targetFiber			= targetFiber;
		_waitingTaskStagingArea	= { ::GetCurrentFiber(), nullptr, 0, [] ( const SuspendedContext& context ) { return true; } };

		::SwitchToFiber( _switchFiber );
	}

// ---------------------------------------------------

	void FiberScheduler::WorkerThread::WaitForCounter( FinishCounter& finishCounter, int value ) {
		if( finishCounter.load( ::std::memory_order_consume ) == value ) {
			return;
		}
		// SET TARGET FIBER HERE AND IN THE OTHER WAIT FUNCTIONS!!!

		if( _pooledFibers ) {
			_targetFiber = _pooledFibers.Back();
			_pooledFibers.PopBack();
		}

		_waitingTaskStagingArea = { ::GetCurrentFiber(), &finishCounter, value, [] ( const SuspendedContext& context ) {
			return context.desiredCounterValue == context.counter->load( std::memory_order_consume );
		} };

		::SwitchToFiber( _counterWaitFiber );
	}

// ---------------------------------------------------

	void FiberScheduler::WorkerThread::WaitForCounterLessThan( FinishCounter& finishCounter, int value ) {
		if( finishCounter.load( ::std::memory_order_consume ) < value ) {
			return;
		}

		if( _pooledFibers ) {
			_targetFiber = _pooledFibers.Back();
			_pooledFibers.PopBack();
		}

		_waitingTaskStagingArea = { ::GetCurrentFiber(), &finishCounter, value, [] ( const SuspendedContext& context ) {
			return context.desiredCounterValue < context.counter->load( std::memory_order_consume );
		} };

		::SwitchToFiber( _counterWaitFiber );
	}

// ---------------------------------------------------

	void FiberScheduler::WorkerThread::WaitForCounterGreaterThan( FinishCounter& finishCounter, int value ) {
		if( finishCounter.load( ::std::memory_order_consume ) > value ) {
			return;
		}

		if( _pooledFibers ) {
			_targetFiber = _pooledFibers.Back();
			_pooledFibers.PopBack();
		}

		_waitingTaskStagingArea = { ::GetCurrentFiber(), &finishCounter, value, [] ( const SuspendedContext& context ) {
			return context.desiredCounterValue > context.counter->load( std::memory_order_consume );
		} };

		::SwitchToFiber( _counterWaitFiber );
	}

// ---------------------------------------------------

	void FiberScheduler::WorkerThread::FiberEntryPoint() {
		_thiefCell.store( nullptr, ::std::memory_order_release );

		while( ExecutionBehavior::Terminate != _executionBehavior.load( ::std::memory_order_consume ) ) {
			auto	resumableTask( Find( _suspendedContexts.Begin(), _suspendedContexts.End(), [] ( const SuspendedContext& context ) { return context.testResumableFunction( context ); } ) );

			if( resumableTask != _suspendedContexts.End() ) {
				const auto	fiber( resumableTask->fiber );

				_suspendedContexts.Erase( resumableTask, ::Eldritch2::UnorderedSemantics );

				SwitchFibers( fiber );
				continue;
			}

			const auto	completionItem( PopCompletionItem() );

			// Distribute any work (if it remains) to other worker threads if they ask for it.
			if( const auto thief = _thiefCell.load( ::std::memory_order_consume ) ) {
				// Don't need to compare here.
				_thiefCell.store( nullptr, ::std::memory_order_release );

				CompleteWorkSharingRequest( *thief );
			}

			completionItem.workItem.function( completionItem.workItem.argument, *this );
			completionItem.counter->fetch_sub( 1, ::std::memory_order_acq_rel );
		}

		// Mark ourselves as unable to be stolen from-- we're not producing work items any more!
		if( const auto thief = _thiefCell.exchange( this, ::std::memory_order_acquire ) ) {
			thief->_transferCell.store( BarrierStatus::Complete, ::std::memory_order_release );
		}
	}

}	// namespace Win32
}	// namespace Scheduler
}	// namespace Eldritch2