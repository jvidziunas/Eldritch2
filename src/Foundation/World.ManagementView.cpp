/*==================================================================*\
  World.ManagementView.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Packages/ContentPackage.hpp>
#include <Foundation/GameEngine.hpp>
#include <Foundation/World.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Scripting;

namespace Eldritch2 {
namespace Foundation {

	World::ManagementView::ManagementView( World& owningWorld ) : WorldView( owningWorld ) {}

// ---------------------------------------------------

	void World::ManagementView::ExecuteFrame( WorkerContext& executingContext ) {
		// This handle ensures the world will not be deleted until the task has finished executing.
		ObjectHandle<World>	worldReference( _owningWorld, ::Eldritch2::PassthroughReferenceCountingSemantics );

		InvokeTickFunction<&WorldView::OnFrameTick>( executingContext );
	}

// ---------------------------------------------------

	void World::ManagementView::TryFinalizeLoad( WorkerContext& executingContext ) {
		using ResidencyState = ContentPackage::ResidencyState;

	// ---

		// This handle ensures the world will not be deleted until the task has finished executing.
		ObjectHandle<World>	worldReference( _owningWorld, ::Eldritch2::PassthroughReferenceCountingSemantics );

		switch( worldReference->GetRootPackage().GetResidencyState() ) {
			case ResidencyState::Failed: {
				worldReference->RaiseFatalError();
				break;
			}	// case ResidencyState::Failed

			case ResidencyState::Published: {
				for( GameEngineService& service : _owningWorld._engine.GetAttachedServices() ) {
					auto	allocateViewResult( service.AllocateWorldView( GetWorldAllocator(), _owningWorld ) );

					if( !allocateViewResult ) {
						break;
					}
				}

				if( !worldReference->EncounteredFatalError() ) {
					worldReference->_isLoaded = 1u;
					InvokeTickFunction<&WorldView::OnGameStart>( executingContext );
				}
				
				break;
			}	// case ResidencyState::Published
		}	// switch( _worldReference->GetRootPackage().GetResidencyState() )
	}

// ---------------------------------------------------

	void World::ManagementView::PrepareForWorldShutdown() {
		BroadcastViewVisitor( WorldView::DeletionPreparationVisitor() );
	}

}	// namespace Foundation
}	// namespace Eldritch2