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
	void WorldView::BroadcastTaskVisitor( Scheduler::WorkerContext& executingContext, Scheduler::WorkerContext::FinishCounter& finishCounter, TaskVisitor&& visitor ) {
		for( auto& view : _owningWorld._attachedViews ) {
			view.AcceptTaskVisitor( executingContext, finishCounter, visitor );
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