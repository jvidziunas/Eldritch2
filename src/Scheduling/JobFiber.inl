/*==================================================================*\
  JobFiber.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scheduling {

	template <typename WorkItem>
	ETInlineHint JobFiber::CompletionItem::CompletionItem( Scheduling::JobBarrier& barrier, WorkItem&& workItem ) : barrier( &barrier ), workItem( eastl::forward<WorkItem>( workItem ) ) {}
		
// ---------------------------------------------------

	ETInlineHint JobFiber::CompletionItem::CompletionItem( CompletionItem&& other ) : barrier( eastl::move( other.barrier ) ), workItem( eastl::move( other.workItem ) ) {}

// ---------------------------------------------------

	template <typename WorkItem>
	ETInlineHint void JobFiber::Enqueue( JobBarrier& barrier, WorkItem&& workItem ) {
		barrier.fetch_add( 1, std::memory_order_relaxed );

		_completionItems.EmplaceBack( barrier, eastl::forward<WorkItem>( workItem ) );
	}

}	// namespace Scheduling
}	// namespace Eldritch2