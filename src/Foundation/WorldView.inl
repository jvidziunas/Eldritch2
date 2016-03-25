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
#include <Scheduler/WorkerContext.hpp>
#include <Foundation/World.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Foundation {

	template <void (WorldView::*TickFunction)(Scheduler::WorkerContext&)>
	void WorldView::InvokeTickFunction( Scheduler::WorkerContext::FinishCounter& finishCounter, Scheduler::WorkerContext& executingContext ) {
		for( WorldView& view : _owningWorld.GetAttachedViews() ) {
			executingContext.Enqueue( finishCounter, { &view, [] ( void* parameter, Scheduler::WorkerContext& executingContext ) { (static_cast<WorldView*>(parameter)->*TickFunction)( executingContext ); } } );
		}
	}

// ---------------------------------------------------

	template <void (WorldView::*TickFunction)( Scheduler::WorkerContext& )>
	void WorldView::InvokeTickFunction( Scheduler::WorkerContext& executingContext ) {
		Scheduler::WorkerContext::FinishCounter	finishCounter( 0 );

		InvokeTickFunction<TickFunction>( finishCounter, executingContext );

		executingContext.WaitForCounter( finishCounter );
	}

// ---------------------------------------------------

	template <typename ViewVisitor>
	void WorldView::BroadcastViewVisitor( ViewVisitor&& visitor ) {
		for( auto& view : _owningWorld.GetAttachedViews() ) {
			view.AcceptViewVisitor( visitor );
		}
	}

// ---------------------------------------------------

	ETInlineHint const World& WorldView::GetOwningWorld() const {
		return _owningWorld;
	}

}	// namespace Foundation
}	// namespace Eldritch2