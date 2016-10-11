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
#include <Scheduling/Win32/FiberScheduler.hpp>
#include <Utility/Containers/Range.hpp>
#include <Utility/Algorithms.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scheduling {
namespace Win32 {

	FiberScheduler::WorkerThread::WorkerThread( FiberScheduler& scheduler ) : JobFiber( { scheduler._dummyBarrier, [&scheduler] ( JobFiber& executor ) { static_cast<WorkerThread&>(executor).StealTask( scheduler ); } }, scheduler._allocator ),
																			  _thief( this ),
																			  _randomWorkerSeed( static_cast<size_t>(__rdtsc()) ),
																			  _terminationBehavior( TerminationBehavior::Continue ),
																			  _transferCell( TaskTransferStatus::Complete ),
																			  _suspendedFibers( { scheduler._allocator, "Suspended Context Collection Allocator" } ),
																			  _pooledFibers( { scheduler._allocator, "Pooled Fiber Collection Allocator" } ),
																			  _bootFiber( nullptr ),
																			  _targetFiber( nullptr ),
																			  _switchFiber( nullptr ) {
	/*	Active worker thread can't be updated here, as there's no guarantee this function is being run on the correct operating system thread yet.
		Likewise, we create fibers, etc. during Run() for more concurrency. */
		_suspendedFibers.Reserve( 16 );
		_pooledFibers.Reserve( 16 );
	}

// ---------------------------------------------------

	FiberScheduler::WorkerThread::~WorkerThread() {
		DeleteFiber( _waitFiber );
		DeleteFiber( _switchFiber );
	}

// ---------------------------------------------------

	Utf8Literal FiberScheduler::WorkerThread::GetHumanReadableName() const {
		return "Worker Thread";
	}

// ---------------------------------------------------

	void FiberScheduler::WorkerThread::Run() {
		enum : SIZE_T {
			SupportFiberStackSizeInBytes	= 32768u,
			FiberStackSizeInBytes			= 524288u,
			FiberCount						= 16u
		};

	// ---

	//	Push this context out to the external object model.
		MakeActive();

		_waitFiber = CreateFiberEx( 0u, SupportFiberStackSizeInBytes, FIBER_FLAG_FLOAT_SWITCH, [] ( void* workerThread ) {
			for( ;; ) {
			//	Fancy loop trick will cause execution to jump back around to here for the next caller after switching away.

			//	Commit the previous fiber into the suspended fiber list.
				static_cast<WorkerThread*>(workerThread)->_suspendedFibers.PushBack( static_cast<WorkerThread*>(workerThread)->_sourceFiber );

				SwitchToFiber( static_cast<WorkerThread*>(workerThread)->_targetFiber );
			}
		}, this );

		_switchFiber = CreateFiberEx( 0u, SupportFiberStackSizeInBytes, FIBER_FLAG_FLOAT_SWITCH, [] ( void* workerThread ) {
			for( ;; ) {
			//	Fancy loop trick will cause execution to jump back around to here for the next caller after switching away.

			//	Commit the previous fiber into the suspended fiber list.
				static_cast<WorkerThread*>(workerThread)->_pooledFibers.PushBack( static_cast<WorkerThread*>(workerThread)->_sourceFiber.fiber );

				SwitchToFiber( static_cast<WorkerThread*>(workerThread)->_targetFiber );
			}
		}, this );

		for( size_t fiberIndex( 0u ); fiberIndex < FiberCount; ++fiberIndex ) {
			_pooledFibers.PushBack( CreateFiberEx( 0u, FiberStackSizeInBytes, FIBER_FLAG_FLOAT_SWITCH, [] ( void* workerThread ) {
				static_cast<WorkerThread*>(workerThread)->FiberEntryPoint();
			}, this ) );
		}

		_bootFiber	= ConvertThreadToFiberEx( this, FIBER_FLAG_FLOAT_SWITCH );

		while( !_pooledFibers.IsEmpty() ) {
			auto	fiber( _pooledFibers.Back() );

			_pooledFibers.PopBack();

			SwitchToFiber( fiber );
			DeleteFiber( _sourceFiber.fiber );

		//	We will resume here once one of the work fibers receives the quit signal.
		}

		ConvertFiberToThread();
	}

// ---------------------------------------------------

	void FiberScheduler::WorkerThread::RequestGracefulShutdown() {
		_terminationBehavior.store( TerminationBehavior::Terminate, std::memory_order_release );
	}

// ---------------------------------------------------

	ETInlineHint bool FiberScheduler::WorkerThread::TryBeginSharing( WorkerThread& thief ) {
		WorkerThread*	expectedValue( nullptr );

		return _thief.compare_exchange_weak( expectedValue, &thief, std::memory_order_release, std::memory_order_relaxed );
	}

// ---------------------------------------------------

	void FiberScheduler::WorkerThread::StealTask( FiberScheduler& scheduler ) {
	//	Prepare to receive work.
		_transferCell.store( TaskTransferStatus::AwaitingTransfer, std::memory_order_relaxed );

		auto&	victim( scheduler.GetRandomWorker( *this, _randomWorkerSeed ) );

	//	Inform the worker that we could use some more work by writing our address into its transfer cell.
		if( victim.TryBeginSharing( *this ) ) {
			do {
			//	Switch over to the other thread on systems with Hyper-Threading technology, as it might be able to respond/share work.
				_mm_pause();
			//	If anyone asks for some tasks, inform them that we don't have anything to share.
				if( const auto thief = _thief.load( std::memory_order_consume ) ) {
					_thief.store( nullptr, std::memory_order_release );

				/*	Hand-inlining this instead of deferring to TryCompleteWorkSharingRequest() results in less busy time
					(we're never going to have shareable tasks, and thusly we waste time trying to check), meaning we can
					relinquish more processor time to other potentially work-creating threads. */
					thief->_transferCell.store( TaskTransferStatus::Complete, std::memory_order_relaxed );
				}
			} while( TaskTransferStatus::AwaitingTransfer == _transferCell.load( std::memory_order_consume ) && !victim.HasCompleted() );
		}
	}

// ---------------------------------------------------

	void FiberScheduler::WorkerThread::CompleteStealOperation( WorkerThread& thief ) {
		const auto	stolenBegin( _completionItems.End() - ( _completionItems.GetSize() / 2 ) );
		const auto	stolenEnd( _completionItems.End() );

		for( auto item( stolenBegin ); item != stolenEnd; ++item ) {
			thief._completionItems.EmplaceBack( eastl::move( *item ) );
		}

	//	Ensure all our writes complete before signaling a finished state.
		thief._transferCell.store( TaskTransferStatus::Complete, std::memory_order_release );

		_completionItems.Erase( stolenBegin, stolenEnd );
	}

// ---------------------------------------------------

	void FiberScheduler::WorkerThread::SwitchFibers( PlatformFiber targetFiber ) {
		_targetFiber	= targetFiber;
		_sourceFiber	= { GetCurrentFiber(), nullptr, 0, [] ( const SuspendedFiber& /*fiber*/ ) { return true; } };

		SwitchToFiber( _switchFiber );
	}

// ---------------------------------------------------

	void FiberScheduler::WorkerThread::AwaitBarrier( JobBarrier& barrier, int value ) {
		if( barrier.load( std::memory_order_consume ) == value ) {
			return;
		}

		if( _pooledFibers ) {
			_targetFiber = _pooledFibers.Back();
			_pooledFibers.PopBack();
		}

		_sourceFiber = { GetCurrentFiber(), &barrier, value, [] ( const SuspendedFiber& fiber ) {
			return fiber.desiredValue == fiber.barrier->load( std::memory_order_consume );
		} };

		SwitchToFiber( _waitFiber );
	}

// ---------------------------------------------------

	void FiberScheduler::WorkerThread::FiberEntryPoint() {
		_thief.store( nullptr, std::memory_order_release );

		while( TerminationBehavior::Terminate != _terminationBehavior.load( std::memory_order_consume ) ) {
		//	Distribute any work (if it remains) to other worker threads if they ask for it.
			if( const auto thief = _thief.load( std::memory_order_consume ) ) {
			//	Don't need to compare here.
				_thief.store( nullptr, std::memory_order_release );

				CompleteStealOperation( *thief );
			}

			const auto	resumableTask( Find( _suspendedFibers.Begin(), _suspendedFibers.End(), [] ( const SuspendedFiber& context ) { return context.testResumableFunction( context ); } ) );

			if( resumableTask != _suspendedFibers.End() ) {
				const auto	fiber( resumableTask->fiber );

				_suspendedFibers.Erase( resumableTask, Eldritch2::UnorderedSemantics );

				SwitchFibers( fiber );
			}

			{	const auto	completionItem( PopCompletionItem() );
				completionItem.workItem( *this );
				completionItem.barrier->fetch_sub( 1, std::memory_order_acq_rel );
			}
		}

	//	Mark ourselves as unable to be stolen from-- we're not producing work items any more!
		if( const auto thief = _thief.exchange( this, std::memory_order_acquire ) ) {
			thief->_transferCell.store( TaskTransferStatus::Complete, std::memory_order_release );
		}

	//	Switch back to the main fiber.
		_sourceFiber.fiber = GetCurrentFiber();
		SwitchToFiber( _bootFiber );
	}

}	// namespace Win32
}	// namespace Scheduling
}	// namespace Eldritch2