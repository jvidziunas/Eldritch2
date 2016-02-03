/*==================================================================*\
  EngineService.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Packages/ResourceViewFactoryPublishingInitializationVisitor.hpp>
#include <Physics/PhysX/EngineService.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Scheduler/ThreadScheduler.hpp>
#include <Scheduler/WorkerContext.hpp>
#include <Physics/PhysX/WorldView.hpp>
#include <Foundation/GameEngine.hpp>
//------------------------------------------------------------------//
#include <characterkinematic/PxControllerManager.h>
#include <microprofile/microprofile.h>
#include <common/PxTolerancesScale.h>
#include <foundation/PxFoundation.h>
#include <pxtask/PxTask.h>
#include <PxSceneDesc.h>
#include <PxPhysics.h>
#include <PxScene.h>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
#if( ET_DEBUG_MODE_ENABLED )
	ET_LINK_LIBRARY("PhysX3ExtensionsDEBUG.lib")
	ET_LINK_LIBRARY("PhysX3CommonDEBUG_x64.lib")
	ET_LINK_LIBRARY("PhysX3DEBUG_x64.lib")
	ET_LINK_LIBRARY("PhysX3GpuDEBUG_x64.lib")
	ET_LINK_LIBRARY("PhysX3CharacterKinematicDEBUG_x64.lib")
#else
	ET_LINK_LIBRARY("PhysX3Extensions.lib")
	ET_LINK_LIBRARY("PhysX3Common_x64.lib")
	ET_LINK_LIBRARY("PhysX3_x64.lib")
	ET_LINK_LIBRARY("PhysX3Gpu_x64.lib")
	ET_LINK_LIBRARY("PhysX3CharacterKinematic_x64.lib")
#endif
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Physics;
using namespace ::Eldritch2;

using namespace ::physx;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	EngineService::EngineService( GameEngine& engine ) : GameEngineService( engine ), _articulatedBodyViewFactory( engine.GetAllocator() ) {}

// ---------------------------------------------------

	EngineService::~EngineService() {}

// ---------------------------------------------------

	const UTF8Char* const EngineService::GetName() const {
		return UTF8L("PhysX Engine");
	}

// ---------------------------------------------------

	ErrorCode EngineService::AllocateWorldView( Allocator& allocator, World& world ) {
		if( !_physics ) {
			return Error::InvalidObjectState;
		}

		PxSceneDesc	sceneDesc( _physics->getTolerancesScale() );

		sceneDesc.filterShader	= &WorldView::FilterShader;
		sceneDesc.cpuDispatcher = this;

		UniquePointer<PxScene>				scene( _physics->createScene( sceneDesc ) );
		UniquePointer<PxControllerManager>	controllerManager( PxCreateControllerManager( *scene ) );

		if( scene && controllerManager ) {
			return new(allocator, Allocator::AllocationDuration::Normal) PhysX::WorldView( ::std::move( scene ), ::std::move( controllerManager ), world ) ? Error::None : Error::OutOfMemory;
		}
	
		return Error::Unspecified;
	}

// ---------------------------------------------------

	void EngineService::OnEngineConfigurationBroadcast( WorkerContext& /*executingContext*/ ) {
		GetLogger()( UTF8L("Creating PhysX SDK objects.") ET_UTF8_NEWLINE_LITERAL );

		_foundation.reset( ::PxCreateFoundation( PX_PHYSICS_VERSION, *this, *this ) );

		if( !_foundation ) {
			GetLogger( LogMessageType::Error )( UTF8L("Error creating PhysX foundation object!") ET_UTF8_NEWLINE_LITERAL );
			return;
		}

		_physics.reset( ::PxCreatePhysics( PX_PHYSICS_VERSION, ::PxGetFoundation(), PxTolerancesScale() ) );
		
		if( !_physics ) {
			GetLogger( LogMessageType::Error )( UTF8L("Error creating PhysX object!") ET_UTF8_NEWLINE_LITERAL );

			return;
		}

		GetLogger()( UTF8L("PhysX SDK objects created successfully.") ET_UTF8_NEWLINE_LITERAL );
	}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( ScriptAPIRegistrationInitializationVisitor& /*visitor*/ ) {}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( ConfigurationPublishingInitializationVisitor& visitor ) {
		_articulatedBodyViewFactory.AcceptInitializationVisitor( visitor );
	}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( ResourceViewFactoryPublishingInitializationVisitor& visitor ) {
		visitor.PublishFactory( ArticulatedBodyViewFactory::GetSerializedDataTag(), _articulatedBodyViewFactory );
	}

// ---------------------------------------------------

	void* EngineService::allocate( size_t sizeInBytes, const char* /*typeName*/, const char* /*filename*/, int /*line*/ ) {
		return GetEngineAllocator().Allocate( sizeInBytes, 16u, Allocator::AllocationDuration::Normal );
	}

// ---------------------------------------------------

	void EngineService::deallocate( void* ptr ) {
		if( ptr ) {
			GetEngineAllocator().Deallocate( ptr, ::Eldritch2::AlignedDeallocationSemantics );
		}
	}

// ---------------------------------------------------

	void EngineService::reportError( PxErrorCode::Enum code, const char* message, const char* file, int line ) {
		if( code & PxErrorCode::Enum::eDEBUG_WARNING ) {
			GetLogger( LogMessageType::VerboseWarning )( UTF8L("PhysX debug warning ({0}({1}): {2}") ET_UTF8_NEWLINE_LITERAL, file, line, message );
		}

		if( code & PxErrorCode::Enum::ePERF_WARNING ) {
			GetLogger( LogMessageType::Warning )( UTF8L("PhysX performance warning ({0}({1}): {2}") ET_UTF8_NEWLINE_LITERAL, file, line, message );
		}
	}

// ---------------------------------------------------

	PxU32 EngineService::getWorkerCount() const {
		return static_cast<PxU32>(GetGameEngine().GetThreadScheduler().GetMaximumTaskParallelism());
	}

// ---------------------------------------------------

	void EngineService::submitTask( PxBaseTask& task ) {
		auto	ExecuteTask( [] ( void* task, WorkerContext& /*executingContext*/ ) {
			MICROPROFILE_SCOPEI( "PhysX Physics Engine", "Scene update task", 0x22FF22 );

			static_cast<PxBaseTask*>(task)->run();
			static_cast<PxBaseTask*>(task)->release();
		} );

		if( const auto workerContext = WorkerContext::GetActiveWorkerContext() ) {
			workerContext->Enqueue( _dummyCounter, { &task, ExecuteTask } );
		} else {
			ExecuteTask( &task, *workerContext );
		}
	}



}	// namespace PhysX
}	// namespace Eldritch2
}	// namespace Physics

