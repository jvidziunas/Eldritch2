/*==================================================================*\
  WorldView.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Foundation/World.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Foundation {

	template <typename TaskVisitor>
	void WorldView::BroadcastTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::WorkerContext& executingContext, Scheduler::Task& visitingTask, TaskVisitor&& visitor ) {
		for( auto& view : _owningWorld._attachedViews ) {
			view.AcceptTaskVisitor( subtaskAllocator, executingContext, visitingTask, visitor );
		}
	}

// ---------------------------------------------------

	template <typename ViewVisitor>
	void WorldView::BroadcastViewVisitor( ViewVisitor&& visitor ) {
		for( auto& view : _owningWorld._attachedViews ) {
			view.AcceptViewVisitor( visitor );
		}
	}

// ---------------------------------------------------

	ETInlineHint const Foundation::World& WorldView::GetOwningWorld() const {
		return _owningWorld;
	}

}	// namespace Foundation
}	// namespace Eldritch2