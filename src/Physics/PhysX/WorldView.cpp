/*==================================================================*\
  WorldView.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/WorldView.hpp>
//------------------------------------------------------------------//
#include <characterkinematic/PxControllerManager.h>
#include <microprofile/microprofile.h>
#include <PxScene.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Physics;
using namespace ::Eldritch2;

using namespace ::physx;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	WorldView::WorldView( PhysX::UniquePointer<::physx::PxScene>&& scene, World& world ) : Foundation::WorldView( world ), _scene( ::std::move( scene ) ) {
		_scene->userData = this;
		_scene->setSimulationEventCallback( this, PX_DEFAULT_CLIENT );
		// _scene->s
	}

// ---------------------------------------------------

	WorldView::~WorldView() {}

// ---------------------------------------------------

	void WorldView::OnScriptTick( WorkerContext& /*executingContext*/ ) {
		MICROPROFILE_SCOPEI( "PhysX Physics Engine", "Begin world tick", 0x22FE22 );
		_scene->simulate( 1.0f / 60.0f );
	}

// ---------------------------------------------------

	void WorldView::OnPostScriptTick( WorkerContext& /*executingContext*/ ) {
		MICROPROFILE_SCOPEI( "PhysX Physics Engine", "Finalize pending tick", 0x22FF22 );

		PxSimulationStatistics	frameStatistics;

		_scene->fetchResults( true );

		_scene->getSimulationStatistics( frameStatistics );
	}

// ---------------------------------------------------

	void WorldView::onConstraintBreak( PxConstraintInfo* /*constraints*/, PxU32 /*count*/ ) {}

// ---------------------------------------------------

	void WorldView::onWake( PxActor** /*actors*/, PxU32 /*count*/ ) {
		MICROPROFILE_SCOPEI( "PhysX Physics Engine", "Wake sleeping actors", 0x22FB22 );
	}

// ---------------------------------------------------

	void WorldView::onSleep( PxActor** /*actors*/, PxU32 /*count*/ ) {
		MICROPROFILE_SCOPEI( "PhysX Physics Engine", "Commit sleeping actors", 0x22FA22 );
	}

// ---------------------------------------------------

	void WorldView::onContact( const PxContactPairHeader& /*pairHeader*/, const PxContactPair* /*pairs*/, PxU32 /*nbPairs*/ ) {
		MICROPROFILE_SCOPEI( "PhysX Physics Engine", "Process", 0x22FC22 );
	}

// ---------------------------------------------------

	void WorldView::onTrigger( PxTriggerPair* /*pairs*/, PxU32 /*count*/ ) {
		MICROPROFILE_SCOPEI( "PhysX Physics Engine", "Process trigger events", 0x22FF22 );
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2

