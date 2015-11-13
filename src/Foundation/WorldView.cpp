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
#include <Utility/ErrorCode.hpp>
#include <Foundation/World.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2;
using namespace ::std;

namespace Eldritch2 {
namespace Foundation {

	WorldView::WorldView( World& owningWorld ) : _owningWorld( owningWorld ) {
		owningWorld._attachedViews.PushFront( *this );
	}

// ---------------------------------------------------

	Allocator& WorldView::GetWorldAllocator() {
		return _owningWorld._allocator;
	}

// ---------------------------------------------------

	const ContentLibrary& WorldView::GetEngineContentLibrary() const {
		return _owningWorld._owningEngine->_contentLibrary;
	}

// ---------------------------------------------------

	void WorldView::AcceptTaskVisitor( Allocator& /*subtaskAllocator*/, WorkerContext& /*executingContext*/, Task& /*visitingTask*/, const FrameTickTaskVisitor ) {
		// Default implementation should not do anything.
	}

// ---------------------------------------------------

	void WorldView::AcceptTaskVisitor( Allocator& /*subtaskAllocator*/, WorkerContext& /*executingContext*/, Task& /*visitingTask*/, const PreScriptTickTaskVisitor ) {
		// Default implementation should not do anything.
	}

// ---------------------------------------------------

	void WorldView::AcceptTaskVisitor( Allocator& /*subtaskAllocator*/, WorkerContext& /*executingContext*/, Task& /*visitingTask*/, const ScriptTickTaskVisitor ) {
		// Default implementation should not do anything.
	}

// ---------------------------------------------------

	void WorldView::AcceptTaskVisitor( Allocator& /*subtaskAllocator*/, WorkerContext& /*executingContext*/, Task& /*visitingTask*/, const PostScriptTickTaskVisitor ) {
		// Default implementation should not do anything.
	}

// ---------------------------------------------------

	void WorldView::AcceptViewVisitor( const ScriptExecutionPreparationVisitor ) {
		// Default implementation should not do anything.
	}

// ---------------------------------------------------

	void WorldView::AcceptViewVisitor( const LoadFinalizationVisitor ) {
		// Default implementation should not do anything.
	}

// ---------------------------------------------------

	void WorldView::AcceptViewVisitor( const DeletionPreparationVisitor ) {
		// Default implementation should not do anything.
	}

// ---------------------------------------------------

	void WorldView::SetPaused( bool paused ) {
		_owningWorld._isPaused = paused ? 1u : 0u;
	}

}	// namespace Foundation
}	// namespace Eldritch2