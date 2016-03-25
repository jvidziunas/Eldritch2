/*==================================================================*\
  WorldView.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Foundation/GameEngine.hpp>
#include <Foundation/WorldView.hpp>
#include <Foundation/World.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Foundation {

	WorldView::WorldView( World& owningWorld ) : _owningWorld( owningWorld ) {
		owningWorld.GetAttachedViews().PushFront( *this );
	}

// ---------------------------------------------------

	Allocator& WorldView::GetWorldAllocator() {
		return _owningWorld.GetAllocator();
	}

// ---------------------------------------------------

	const ContentLibrary& WorldView::GetContentLibrary() const {
		return _owningWorld.GetContentLibrary();
	}

// ---------------------------------------------------

	void WorldView::RequestWorldShutdown() const {
		_owningWorld.Terminate();
	}

// ---------------------------------------------------

	void WorldView::OnFrameTick( WorkerContext& /*executingContext*/ ) {
		// Default implementation should not do anything.
	}

// ---------------------------------------------------

	void WorldView::OnPreScriptTick( WorkerContext& /*executingContext*/ ) {
		// Default implementation should not do anything.
	}

// ---------------------------------------------------

	void WorldView::OnScriptTick( WorkerContext& /*executingContext*/ ) {
		// Default implementation should not do anything.
	}

// ---------------------------------------------------

	void WorldView::OnPostScriptTick( WorkerContext& /*executingContext*/ ) {
		// Default implementation should not do anything.
	}

// ---------------------------------------------------

	void WorldView::AcceptViewVisitor( const ScriptExecutionPreparationVisitor ) {
		// Default implementation should not do anything.
	}

// ---------------------------------------------------

	void WorldView::AcceptViewVisitor( const DeletionPreparationVisitor ) {
		// Default implementation should not do anything.
	}

// ---------------------------------------------------

	void WorldView::OnGameStart( WorkerContext& /*executingContext*/ ) {
		// Default implementation should not do anything.
	}

}	// namespace Foundation
}	// namespace Eldritch2