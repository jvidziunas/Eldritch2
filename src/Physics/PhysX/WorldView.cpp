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
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#include <extensions/PxDefaultSimulationFilterShader.h>
#include <characterkinematic/PxControllerManager.h>
#include <microprofile/microprofile.h>
#include <PxScene.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Physics;
using namespace ::Eldritch2;

using namespace ::physx;

namespace {

	static ETThreadLocal PhysX::WorldView*	activeWorldView = nullptr;

}	// anonymous namespace

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	WorldView::WorldView( UniquePointer<PxScene>&& scene, UniquePointer<PxControllerManager>&& controllerManager, World& world ) : Foundation::WorldView( world ), _scene( ::std::move( scene ) ), _controllerManager( ::std::move( controllerManager ) ) {
		ETRuntimeAssert( nullptr != _scene );
		ETRuntimeAssert( nullptr != _controllerManager );

	// ---

		_scene->userData = this;
		_scene->setSimulationEventCallback( this, PX_DEFAULT_CLIENT );
		_scene->setContactModifyCallback( this );
	}

// ---------------------------------------------------

	WorldView::~WorldView() {
		_scene->checkResults( true );
		_scene->flushSimulation();
	}

// ---------------------------------------------------

	void WorldView::OnPreScriptTick( Scheduler::WorkerContext& /*executingContext*/ ) {
		MICROPROFILE_SCOPEI( "PhysX Physics Engine", "Finalize pending tick", 0x22FF22 );

		PxSimulationStatistics	frameStatistics;

		_scene->fetchResults( true );

		_scene->getSimulationStatistics( frameStatistics );
	}

// ---------------------------------------------------

	void WorldView::OnScriptTick( WorkerContext& /*executingContext*/ ) {}

// ---------------------------------------------------

	void WorldView::OnPostScriptTick( WorkerContext& /*executingContext*/ ) {
		MICROPROFILE_SCOPEI( "PhysX Physics Engine", "Begin world tick", 0x22FE22 );

		_controllerManager->computeInteractions( 1.0f / 60.0f );
		_scene->simulate( 1.0f / 60.0f );
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

// ---------------------------------------------------

	void WorldView::onContactModify( PxContactModifyPair* const /*pairs*/, PxU32 /*count*/ ) {}

// ---------------------------------------------------

	PxFilterFlags WorldView::FilterShader( PxFilterObjectAttributes attributes0, PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1, PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize ) {
		return PxDefaultSimulationFilterShader( attributes0, filterData0, attributes1, filterData1, pairFlags, constantBlock, constantBlockSize );
	}

// ---------------------------------------------------

	WorldView& WorldView::GetActiveWorldView() {
		return *activeWorldView;
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2

