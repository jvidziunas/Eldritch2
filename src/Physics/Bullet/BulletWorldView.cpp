/*==================================================================*\
  BulletWorldView.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/Bullet/BulletWorldView.hpp>
#include <Scheduler/CRTPTransientTask.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Physics/BulletEngineService.hpp>
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

	static ETThreadLocal BulletWorldView*	activeScriptWorldViewForThread;

// ---------------------------------------------------

	static ETNoAliasHint ::btVector3 DetermineAABBMinimaForWorld( const World& /*world*/ ) {
		return ::btVector3( -10000.0f, -10000.0f, -10000.0f );
	}

// ---------------------------------------------------

	static ETNoAliasHint ::btVector3 DetermineAABBMaximaForWorld( const World& /*world*/ ) {
		return ::btVector3( 10000.0f, 10000.0f, 10000.0f );
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

	BulletWorldView::BulletWorldView( World& owningWorld, const BulletEngineService& hostingEngine ) : WorldView( owningWorld ),
																								_persistentManifoldPool( 32u, hostingEngine.GetWorldPersistentManifoldPoolSizeInElements() ),
																								_collisionAlgorithmPool( 16u, hostingEngine.GetWorldCollisionAlgorithmPoolSizeInElements() ),
																								_softBodySolver(),
																								_collisionConfiguration( CreateCollisionConfigurationConstructionInfo( _persistentManifoldPool, _collisionAlgorithmPool ) ),
																								_dispatcher( &_collisionConfiguration ),
																								_pairCache(),
																								_broadphaseInterface( DetermineAABBMinimaForWorld( owningWorld ), DetermineAABBMaximaForWorld( owningWorld ), DetermineMaxHandleCountForWorld( owningWorld ), &_pairCache ),
																								_constraintSolver(),
																								_dynamicsWorld( &_softBodySolver, &_dispatcher, &_broadphaseInterface, &_constraintSolver, &_collisionConfiguration ),
																								_ghostPairCallback() {
		_dynamicsWorld.getPairCache()->setInternalGhostPairCallback( &_ghostPairCallback );
	}

// ---------------------------------------------------

	BulletWorldView::~BulletWorldView() {}

// ---------------------------------------------------

	void BulletWorldView::AcceptViewVisitor( const ScriptExecutionPreparationVisitor ) {
		activeScriptWorldViewForThread = this;
	}

// ---------------------------------------------------

	void BulletWorldView::AcceptViewVisitor( ScriptMessageSink& /*messageSink*/ ) {
		// Dispatch collision notifications
	}

// ---------------------------------------------------

	void BulletWorldView::AcceptTaskVisitor( Allocator& /*subtaskAllocator*/, WorkerContext& /*executingContext*/, Task& /*visitingTask*/, const PreScriptTickTaskVisitor ) {
		// Do pathfinding?
	}

// ---------------------------------------------------

	void BulletWorldView::AcceptTaskVisitor( Allocator& subtaskAllocator, WorkerContext& executingContext, Task& visitingTask, const PostScriptTickTaskVisitor ) {
		class SimulateWorldTask : public CRTPTransientTask<SimulateWorldTask> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref SimulateWorldTask instance.
			ETInlineHint SimulateWorldTask( BulletWorldView& owner, WorkerContext& executingContext, Task& postScriptTickTask ) : CRTPTransientTask<SimulateWorldTask>( postScriptTickTask, Scheduler::CodependentTaskSemantics ),
																																  _owner( owner ) {
				TrySchedulingOnContext( executingContext );
			}

		// ---------------------------------------------------

			const UTF8Char* const GetHumanReadableName() const override sealed {
				return UTF8L("Bullet Simulate World Task");
			}

			Task* Execute( WorkerContext& /*executingContext*/ ) override sealed {
				_owner._dynamicsWorld.stepSimulation( 1.0f / 60.0f );

				return nullptr;
			}

		// - DATA MEMBERS ------------------------------------

		private:
			BulletWorldView&	_owner;
		};

	// ---

		new(subtaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) SimulateWorldTask( *this, executingContext, visitingTask );
	}

// ---------------------------------------------------

	BulletWorldView& BulletWorldView::GetActiveWorldView() {
		ETRuntimeAssert( nullptr != activeScriptWorldViewForThread );

		return *activeScriptWorldViewForThread;
	}

// ---------------------------------------------------

	void BulletWorldView::ExposeScriptAPI( ScriptAPIRegistrationInitializationVisitor& visitor ) {
		CharacterArmatureComponent::ExposeScriptAPI( visitor );
		TerrainColliderComponent::ExposeScriptAPI( visitor );
		TriggerVolumeComponent::ExposeScriptAPI( visitor );
		// PhysicalSoftBodyComponent::ExposeScriptAPI( visitor );
	}

}	// namespace Physics
}	// namespace Eldritch2