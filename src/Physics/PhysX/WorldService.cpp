/*==================================================================*\
  WorldService.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/ScriptComponents/CharacterCollider.hpp>
#include <Physics/PhysX/ScriptComponents/SkeletalArmature.hpp>
#include <Physics/PhysX/ScriptComponents/TerrainCollider.hpp>
#include <Physics/PhysX/ScriptMessages/CollisionMessage.hpp>
#include <Physics/PhysX/ScriptComponents/TriggerVolume.hpp>
#include <Physics/PhysX/ScriptComponents/MeshCollider.hpp>
#include <Utility/Math/StandardLibrary.hpp>
#include <Physics/PhysX/WorldService.hpp>
#include <Scripting/MessageBus.hpp>
#include <Utility/Assert.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//
#include <extensions/PxDefaultSimulationFilterShader.h>
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scheduling;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Core;
using namespace ::physx;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	WorldService::WorldService( UniquePointer<PxScene> scene, UniquePointer<PxControllerManager> controllerManager, const World& world ) : Core::WorldService( world.GetServiceBlackboard() ),
																																		   _scene( eastl::move( scene ) ),
																																		   _controllerManager( eastl::move( controllerManager ) ),
																																		   _animationLayerAllocator( world.GetAllocator(), "PhysX World Service Animation Layer Root Allocator" ),
																																		   _armatureAllocator( "PhysX World Service Armature Pool Allocator",
																																		   					   sizeof(ScriptComponents::SkeletalArmature),
																																		   					   (8192u / sizeof(ScriptComponents::SkeletalArmature)),
																																		   					   world.GetAllocator() ) {
		ETRuntimeAssert( _scene );
		ETRuntimeAssert( _controllerManager );

	// ---

		_scene->userData = this;
		_scene->setSimulationEventCallback( this );
		_scene->setContactModifyCallback( this );
	}

// ---------------------------------------------------

	WorldService::~WorldService() {}

// ---------------------------------------------------

	Utf8Literal WorldService::GetName() const {
		return "PhysX World Service";
	}

// ---------------------------------------------------

	void WorldService::AcceptVisitor( MessageBus& messageBus ) {
		_scene->fetchResults( true );
	}

// ---------------------------------------------------

	void WorldService::AcceptVisitor( ServiceBlackboard& blackboard ) {
		blackboard.Publish<PxControllerManager>( *_controllerManager );
		blackboard.Publish<PxScene>( *_scene );
	}

// ---------------------------------------------------

	void WorldService::OnPostScriptTick( JobFiber& /*fiber*/ ) {
		const auto	tickDurationInSeconds( 1.0f / 60.0f );

		{	MICROPROFILE_SCOPEI( "PhysX Physics Engine", "Compute character controller interactions", 0x22FE22 );
			_controllerManager->computeInteractions( tickDurationInSeconds );
		}
		
		{	MICROPROFILE_SCOPEI( "PhysX Physics Engine", "Dispatch simulate jobs", 0x32AACD );
			_scene->simulate( tickDurationInSeconds );
		}
	}

// ---------------------------------------------------

	void WorldService::AcceptVisitor( JobFiber& /*fiber*/, const GameStartVisitor ) {
		_scene->simulate( 0.0f );
	}

// ---------------------------------------------------

	void WorldService::AcceptVisitor( JobFiber& /*fiber*/, const GameStopVisitor ) {
		_scene->checkResults( true );
		_scene->flushSimulation();
	}

// ---------------------------------------------------

	void WorldService::onConstraintBreak( PxConstraintInfo* /*constraints*/, PxU32 /*count*/ ) {}

// ---------------------------------------------------

	void WorldService::onWake( PxActor** /*actors*/, PxU32 /*count*/ ) {}

// ---------------------------------------------------

	void WorldService::onSleep( PxActor** /*actors*/, PxU32 /*count*/ ) {}

// ---------------------------------------------------

	void WorldService::onContact( const PxContactPairHeader& pairHeader, const PxContactPair* /*pairs*/, PxU32 /*nbPairs*/ ) {
		if( pairHeader.flags.isSet( PxContactPairHeaderFlag::eREMOVED_ACTOR_0 ) | pairHeader.flags.isSet( PxContactPairHeaderFlag::eREMOVED_ACTOR_1 ) ) {
			return;
		}
	}

// ---------------------------------------------------

	void WorldService::onTrigger( PxTriggerPair* pairs, PxU32 count ) {
		for( const auto& pair : Range<PxTriggerPair*>( pairs, pairs + count ) ) {

		}
	}

// ---------------------------------------------------

	void WorldService::onContactModify( PxContactModifyPair* const /*pairs*/, PxU32 /*count*/ ) {}

// ---------------------------------------------------

	PxFilterFlags WorldService::FilterShader( PxFilterObjectAttributes attributes0, PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1, PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize ) {
		return PxDefaultSimulationFilterShader( attributes0, filterData0, attributes1, filterData1, pairFlags, constantBlock, constantBlockSize );
	}

// ---------------------------------------------------

	void WorldService::RegisterScriptApi( ApiRegistrar& registrar ) {
	//	Register message types.
		ScriptMessages::CollisionMessage::RegisterScriptApi( registrar );

	//	Register component types.
		ScriptComponents::SkeletalArmature::RegisterScriptApi( registrar );
		ScriptComponents::CharacterCollider::RegisterScriptApi( registrar );
		ScriptComponents::TerrainCollider::RegisterScriptApi( registrar );
		ScriptComponents::TriggerVolume::RegisterScriptApi( registrar );
		ScriptComponents::MeshCollider::RegisterScriptApi( registrar );
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2

