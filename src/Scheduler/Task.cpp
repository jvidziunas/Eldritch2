/*==================================================================*\
  Task.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduler/WorkerContext.hpp>
#include <Utility/ErrorCode.hpp>
#include <Scheduler/Task.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2;
using namespace ::std;

namespace Eldritch2 {
namespace Scheduler {

	const Task::ContinuationTaskSemantics	ContinuationTaskSemantics;
	const Task::CodependentTaskSemantics	CodependentTaskSemantics;

// ---------------------------------------------------

	Task::Task() : _state( ExecutionState::UNSCHEDULED ), _startCount( 1u ), _endCount( 1u ), _continuationTask( nullptr ), _codependentTask( nullptr ) {}

// ---------------------------------------------------

	Task::Task( Task& continuationTask, const ContinuationTaskSemantics ) : _state( ExecutionState::UNSCHEDULED ), _startCount( 1u ), _endCount( 1u ), _continuationTask( &continuationTask ), _codependentTask( nullptr ) {
		continuationTask._startCount.fetch_add( 1u, memory_order_release );
	}

// ---------------------------------------------------

	Task::Task( Task& codependentTask, const CodependentTaskSemantics ) : _state( ExecutionState::UNSCHEDULED ), _startCount( 1u ), _endCount( 1u ), _continuationTask( nullptr ), _codependentTask( &codependentTask ) {
		codependentTask._endCount.fetch_add( 1u, memory_order_release );
	}

// ---------------------------------------------------

	Task::Task( Task& continuationTask, Task& codependentTask ) : _state( ExecutionState::UNSCHEDULED ), _startCount( 1u ), _endCount( 1u ), _continuationTask( &continuationTask ), _codependentTask( &codependentTask ) {
		ETRuntimeAssert( &continuationTask != &codependentTask );

		continuationTask._startCount.fetch_add( 1u, memory_order_release );
		codependentTask._endCount.fetch_add( 1u, memory_order_release );
	}

// ---------------------------------------------------

	bool Task::TrySchedulingOnContext( WorkerContext& activeWorkerContext ) {
		auto	SchedulerEntryPoint( [] ( void* taskPointer, WorkerContext& activeWorkerContext ) {
			for( Task* task( static_cast<Task*>(taskPointer) ); nullptr != task; ) {
				task->_state.store( ExecutionState::RUNNING, memory_order_release );

				Task* const	next( task->Execute( activeWorkerContext ) );

				do {
					if( 1u == task->_endCount.fetch_sub( 1u, memory_order_release ) ) {
						// Store these here in case the task deletes itself.
						auto	continuation( task->_continuationTask );
						auto	codependent( task->_codependentTask );

						task->_state.store( ExecutionState::DONE, memory_order_release );

						task->Finalize( activeWorkerContext );

						// Start the tasks if they become ready
						if( continuation ) {
							continuation->TrySchedulingOnContext( activeWorkerContext );
						}

						// Traverse all completions to signal we are done
						task = codependent;
					} else {
						task = nullptr;
					}
				} while( task );

				// Don't run the next item if we would violate dependencies.
				if( next && (1u != next->_startCount.fetch_sub( 1u, memory_order_acquire )) ) {
					break;
				}

				task = next;
			}
		} );

		if( 1u == _startCount.fetch_sub( 1u, memory_order_acquire ) ) {
			_state.store( ExecutionState::SCHEDULED, memory_order_release );
			activeWorkerContext.Enqueue( { SchedulerEntryPoint, this } );
			return true;
		}

		return false;
	}

// ---------------------------------------------------

	Task::ExecutionState Task::GetStatus() const {
		return _state.load( memory_order_relaxed );
	}

// ---------------------------------------------------

	void Task::Finalize( WorkerContext& /*executingContext*/ ) {
		// The default implementation should not do anything.
	} 

}	// namespace Scheduler
}	// namespace Eldritch2