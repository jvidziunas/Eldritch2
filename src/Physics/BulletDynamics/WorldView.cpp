/*==================================================================*\
  WorldView.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/BulletDynamics/EngineService.hpp>
#include <Physics/BulletDynamics/WorldView.hpp>
#include <Utility/Math/StandardLibrary.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Utility/ErrorCode.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Physics;
using namespace ::Eldritch2;

//==================================================================//
// LIBRARIES
//==================================================================//
#if( ET_DEBUG_MODE_ENABLED )
	ET_LINK_LIBRARY( "BulletCollision_DEBUG.lib" )
	ET_LINK_LIBRARY( "BulletDynamics_DEBUG.lib" )
	ET_LINK_LIBRARY( "BulletSoftBody_DEBUG.lib" )
	ET_LINK_LIBRARY( "LinearMath_DEBUG.lib")
#else
	ET_LINK_LIBRARY( "BulletCollision.lib" )
	ET_LINK_LIBRARY( "BulletDynamics.lib" )
	ET_LINK_LIBRARY( "BulletSoftBody.lib" )
	ET_LINK_LIBRARY( "LinearMath.lib" )
#endif
//------------------------------------------------------------------//

namespace {

	static ETThreadLocal BulletDynamics::WorldView*	activeScriptWorldViewForThread	= nullptr;
	static const ::btScalar							defaultWorldExtent				= static_cast<::btScalar>(10000.0f);
	static const UTF8Char							extentPropertyName[]			= UTF8L("Extent");

// ---------------------------------------------------

	static ETNoAliasHint ::btVector3 DetermineAABBMinimaForWorld( const World& world ) {
		const auto	extent( -AbsoluteValue( world.GetPropertyByKey( extentPropertyName, defaultWorldExtent ) ) );

		return ::btVector3( extent, extent, extent );
	}

// ---------------------------------------------------

	static ETNoAliasHint ::btVector3 DetermineAABBMaximaForWorld( const World& world ) {
		const auto	extent( AbsoluteValue( world.GetPropertyByKey( extentPropertyName, defaultWorldExtent ) ) );

		return ::btVector3( extent, extent, extent );
	}

// ---------------------------------------------------

	static ETNoAliasHint unsigned int DetermineMaxHandleCountForWorld( const World& /*world*/ ) {
		return 15000;
	}

// ---------------------------------------------------

	static ETInlineHint ETNoAliasHint ::btDefaultCollisionConstructionInfo CreateCollisionConfigurationConstructionInfo( ::btPoolAllocator& manifoldPool, ::btPoolAllocator& collisionAlgorithmPool ) {
		::btDefaultCollisionConstructionInfo	result;

		result.m_collisionAlgorithmPool = &collisionAlgorithmPool;
		result.m_persistentManifoldPool = &manifoldPool;
		return result;
	}

}	// anonymous namespace

namespace Eldritch2 {
namespace Physics {
namespace BulletDynamics {

	WorldView::WorldView( World& owningWorld, const EngineService& hostingEngine ) : Foundation::WorldView( owningWorld ),
																					 _persistentManifoldPool( 32u, hostingEngine.GetWorldPersistentManifoldPoolSizeInElements() ),
																					 _collisionAlgorithmPool( 16u, hostingEngine.GetWorldCollisionAlgorithmPoolSizeInElements() ),
																					 _softBodySolver(),
																					 _collisionConfiguration( CreateCollisionConfigurationConstructionInfo( _persistentManifoldPool, _collisionAlgorithmPool ) ),
																					 _dispatcher( &_collisionConfiguration ),
																					 _pairCache(),
																					 _broadphaseInterface( DetermineAABBMinimaForWorld( owningWorld ),
																										   DetermineAABBMaximaForWorld( owningWorld ),
																										   DetermineMaxHandleCountForWorld( owningWorld ),
																										   &_pairCache ),
																					 _constraintSolver(),
																					 _dynamicsWorld( &_softBodySolver,
																									 &_dispatcher,
																									 &_broadphaseInterface,
																									 &_constraintSolver,
																									 &_collisionConfiguration ),
																					 _ghostPairCallback() {
		_dynamicsWorld.getPairCache()->setInternalGhostPairCallback( &_ghostPairCallback );
	}

// ---------------------------------------------------

	void WorldView::AcceptViewVisitor( const ScriptExecutionPreparationVisitor ) {
		activeScriptWorldViewForThread = this;
	}

// ---------------------------------------------------

	void WorldView::AcceptTaskVisitor( WorkerContext& /*executingContext*/, WorkerContext::FinishCounter& /*finishCounter*/, const PreScriptTickTaskVisitor ) {
		// Do pathfinding?
	}

// ---------------------------------------------------

	void WorldView::AcceptTaskVisitor( WorkerContext& executingContext, WorkerContext::FinishCounter& finishCounter, const PostScriptTickTaskVisitor ) {
		executingContext.Enqueue( finishCounter, { this, [] ( void* view, WorkerContext& /*executingContext*/ ) {
			static_cast<WorldView*>(view)->_dynamicsWorld.stepSimulation2( 1.0f / 60.0f );
		} } );
	}

// ---------------------------------------------------

	WorldView& WorldView::GetActiveWorldView() {
		ETRuntimeAssert( nullptr != activeScriptWorldViewForThread );

	// ---

		return *activeScriptWorldViewForThread;
	}

// ---------------------------------------------------

	void WorldView::ExposeScriptAPI( ScriptAPIRegistrationInitializationVisitor& visitor ) {
		CharacterArmatureComponent::ExposeScriptAPI( visitor );
		TerrainColliderComponent::ExposeScriptAPI( visitor );
		TriggerVolumeComponent::ExposeScriptAPI( visitor );
		// PhysicalSoftBodyComponent::ExposeScriptAPI( visitor );
	}

}	// namespace BulletDynamics
}	// namespace Physics
}	// namespace Eldritch2