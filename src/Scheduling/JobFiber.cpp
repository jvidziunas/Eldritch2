/*==================================================================*\
  JobFiber.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduling/JobFiber.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scheduling {
namespace {

	static ETThreadLocal JobFiber*	activeFiber = nullptr;

}	// anonymous namespace

	JobFiber::JobFiber( CompletionItem&& defaultItem, Allocator& allocator ) : _completionItems( { allocator, "Worker Context Completion Queue Allocator" } ) {
		_completionItems.Reserve( 64u );
		_completionItems.EmplaceBack( eastl::move( defaultItem ) );
	}

// ---------------------------------------------------

	JobFiber::JobFiber( JobFiber&& source ) : _completionItems( eastl::move( source._completionItems ) ) {}

// ---------------------------------------------------

	JobFiber::CompletionItem JobFiber::PopCompletionItem() {
		const auto	item( _completionItems.Back() );

	//	Don't pop the default/last work item.
		if( ETBranchLikelyHint( _completionItems.GetSize() > 1u ) ) {
			_completionItems.PopBack();
		}

		return item;
	}

// ---------------------------------------------------

	void JobFiber::MakeActive() {
		activeFiber = this;
	}

// ---------------------------------------------------

	JobFiber& GetActiveJobFiber() {
		ETRuntimeAssert( nullptr != activeFiber );

		return *activeFiber;
	}

}	// namespace Scheduling
}	// namespace Eldritch2