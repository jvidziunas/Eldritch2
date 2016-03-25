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
#include <Scripting/ScriptApiRegistrationInitializationVisitor.hpp>
#include <Utility/Math/StandardLibrary.hpp>
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
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Physics;
using namespace ::Eldritch2;

using namespace ::physx;

namespace {

	static ETThreadLocal PhysX::WorldView*	activeWorldView = nullptr;

}	// anonymous namespace

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	WorldView::WorldView( UniquePointer<PxScene>&& scene, UniquePointer<PxControllerManager>&& controllerManager, World& world ) : Foundation::WorldView( world ),
																																   _scene( ::std::move( scene ) ),
																																   _controllerManager( ::std::move( controllerManager ) ),
																																   _armatureAllocator( UTF8L("PhysX World View Armature Pool Allocator"),
																																					   GetArmaturePoolElementSizeInBytes(),
																																					   (8192u / GetArmaturePoolElementSizeInBytes()),
																																					   GetWorldAllocator() ),
																																   _componentAllocator( UTF8L("PhysX World View Component Pool Allocator"),
																																						GetComponentPoolElementSizeInBytes(),
																																						(8192u / GetComponentPoolElementSizeInBytes()),
																																						GetWorldAllocator() ) {
		ETRuntimeAssert( nullptr != _scene );
		ETRuntimeAssert( nullptr != _controllerManager );

	// ---

		_scene->userData = this;
		_scene->setSimulationEventCallback( this, PX_DEFAULT_CLIENT );
		_scene->setContactModifyCallback( this );
		_scene->simulate( 0.0f );
	}

// ---------------------------------------------------

	WorldView::~WorldView() {}

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

		_controllerManager->computeInteractions( GetTickDurationInSeconds() );
		_scene->simulate( GetTickDurationInSeconds() );
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

	ETNoAliasHint void WorldView::ExposeScriptAPI( ScriptApiRegistrationInitializationVisitor& typeRegistrar ) {
		PhysicalArmature::ExposeScriptAPI( typeRegistrar );
		AnimationDrivenCharacterCollider::ExposeScriptAPI( typeRegistrar );
		MeshCollider::ExposeScriptAPI( typeRegistrar );
		TerrainCollider::ExposeScriptAPI( typeRegistrar );
		TriggerVolume::ExposeScriptAPI( typeRegistrar );
	}

// ---------------------------------------------------

	WorldView& WorldView::GetActiveWorldView() {
		return *activeWorldView;
	}

// ---------------------------------------------------

	ETNoAliasHint float32 WorldView::GetTickDurationInSeconds() {
		return ( 1.0f / 60.0f );
	}

// ---------------------------------------------------

	ETNoAliasHint size_t WorldView::GetComponentPoolElementSizeInBytes() {
		return Max( sizeof(AnimationDrivenCharacterCollider), Max( sizeof(MeshCollider), Max( sizeof(TerrainCollider), sizeof(TriggerVolume) ) ) );
	}

// ---------------------------------------------------

	ETNoAliasHint size_t WorldView::GetArmaturePoolElementSizeInBytes() {
		return sizeof(PhysicalArmature);
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2

