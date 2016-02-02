/*==================================================================*\
  WorkerContext.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduler/WorkerContext.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2;

namespace {

	static ETThreadLocal Scheduler::WorkerContext*	activeWorkerContext = nullptr;

}	// anonymous namespace

namespace Eldritch2 {
namespace Scheduler {

	WorkerContext::WorkerContext( const CompletionItem& defaultItem, ::Eldritch2::Allocator& allocator ) : _completionItems( { allocator, UTF8L("Worker Context Completion Queue Allocator") } ) {
		_completionItems.Reserve( 64u );
		_completionItems.PushBack( defaultItem );
	}

// ---------------------------------------------------

	WorkerContext::WorkerContext( WorkerContext&& source ) : _completionItems( ::std::move( source._completionItems ) ) {}

// ---------------------------------------------------

	void WorkerContext::Enqueue( FinishCounter& finishCounter, const WorkItem* workItemBegin, const WorkItem* workItemEnd ) {
		finishCounter.fetch_add( static_cast<int>(workItemEnd - workItemBegin), ::std::memory_order_relaxed );

		while( workItemBegin != workItemEnd ) {
			_completionItems.PushBack( { &finishCounter, *workItemBegin++, } );
		}
	}

// ---------------------------------------------------

	void WorkerContext::Enqueue( FinishCounter& finishCounter, const WorkItem& workItem ) {
		finishCounter.fetch_add( 1, ::std::memory_order_relaxed );

		_completionItems.PushBack( { &finishCounter, workItem } );
	}

// ---------------------------------------------------

	WorkerContext* WorkerContext::GetActiveWorkerContext() {
		return activeWorkerContext;
	}

// ---------------------------------------------------

	void WorkerContext::SetActiveWorkerContext( WorkerContext* const context ) {
		activeWorkerContext = context;
	}

// ---------------------------------------------------

	WorkerContext::CompletionItem WorkerContext::PopCompletionItem() {
		const auto	item( _completionItems.Back() );

		// Don't pop the default/last work item.
		if( _completionItems.Size() > 1u ) {
			_completionItems.PopBack();
		}

		return item;
	}

}	// namespace Scheduler
}	// namespace Eldritch2