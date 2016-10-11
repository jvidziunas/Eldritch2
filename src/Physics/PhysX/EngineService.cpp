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
#include <Physics/PhysX/AssetViews/SkeletalColliderView.hpp>
#include <Physics/PhysX/AssetViews/ArmatureView.hpp>
#include <Configuration/ConfigurationRegistrar.hpp>
#include <Physics/PhysX/AssetViews/TerrainView.hpp>
#include <Assets/AssetViewFactoryRegistrar.hpp>
#include <Physics/PhysX/EngineService.hpp>
#include <Physics/PhysX/WorldService.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Scheduling/ThreadScheduler.hpp>
#include <Scheduling/JobFiber.hpp>
#include <Core/Engine.hpp>
//------------------------------------------------------------------//
#include <characterkinematic/PxControllerManager.h>
#include <microprofile/microprofile.h>
#include <common/PxTolerancesScale.h>
#include <cooking/PxCooking.h>
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
	ET_LINK_LIBRARY("PhysX3CookingDEBUG_x64.lib")
	ET_LINK_LIBRARY("PhysX3DEBUG_x64.lib")
	ET_LINK_LIBRARY("PhysX3GpuDEBUG_x64.lib")
	ET_LINK_LIBRARY("PhysX3CharacterKinematicDEBUG_x64.lib")
#else
	ET_LINK_LIBRARY("PhysX3Extensions.lib")
	ET_LINK_LIBRARY("PhysX3Common_x64.lib")
	ET_LINK_LIBRARY("PhysX3_x64.lib")
	ET_LINK_LIBRARY("PhysX3Cooking_x64.lib")
	ET_LINK_LIBRARY("PhysX3Gpu_x64.lib")
	ET_LINK_LIBRARY("PhysX3CharacterKinematic_x64.lib")
#endif
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::Scheduling;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Logging;
using namespace ::Eldritch2::Assets;
using namespace ::Eldritch2::Core;
using namespace ::physx;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace {

	enum : size_t {
		RequiredAlignment	= 16u,
		PaddingInBytes		= RequiredAlignment
	};

}	// anonymous namespace

	EngineService::EngineService( const Engine& engine ) : Core::EngineService( engine.GetServiceBlackboard() ), _allocator( engine.GetAllocator(), "PhysX Root Allocator" ), _log( engine.GetLog() ) {}

// ---------------------------------------------------

	EngineService::~EngineService() {}

// ---------------------------------------------------

	Utf8Literal EngineService::GetName() const {
		return "PhysX Engine";
	}

// ---------------------------------------------------

	Result<Eldritch2::UniquePointer<Core::WorldService>> EngineService::CreateWorldService( Allocator& allocator, const World& world ) {
		PxSceneDesc	descriptor( _physics->getTolerancesScale() );

		descriptor.filterShader		= &WorldService::FilterShader;
		descriptor.cpuDispatcher	= this;

	//	Create the core PhysX scene object, responsible for simulation and communication across the various actors, etc. in the physics scene.
		UniquePointer<PxScene>	scene( _physics->createScene( descriptor ) );
		if( !scene ) {
			return Error::Unspecified;
		}

	//	PxControllerManagers are used to create and simulate PhysX character controllers.
		UniquePointer<PxControllerManager>	controllerManager( PxCreateControllerManager( *scene ) );
		if( !controllerManager ) {
			return Error::Unspecified;
		}

		auto result( MakeUnique<PhysX::WorldService>( allocator, eastl::move( scene ), eastl::move( controllerManager ), world ) );
		if( !result ) {
			return Error::OutOfMemory;
		}
	
		return eastl::move( result );
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( JobFiber& /*fiber*/, const ConfigurationBroadcastVisitor ) {
		_log( MessageSeverity::Message, "Creating PhysX SDK objects." ET_UTF8_NEWLINE_LITERAL );

	//	Create the PxFoundation loader object responsible for loading the various PhysX modules.
		UniquePointer<PxFoundation>	foundation( PxCreateFoundation( PX_PHYSICS_VERSION, *this, *this ) );
		if( !foundation ) {
			_log( MessageSeverity::Error, "Error creating PhysX foundation object!" ET_UTF8_NEWLINE_LITERAL );
			return;
		}

	//	Create the core PhysX object used to create scenes, actors, shapes, etc.
		UniquePointer<PxPhysics>	physics( PxCreatePhysics( PX_PHYSICS_VERSION, *foundation, PxTolerancesScale() ) );
		if( !physics ) {
			_log( MessageSeverity::Error, "Error creating PhysX object!" ET_UTF8_NEWLINE_LITERAL );
			return;
		}

	//	We will additionally need to create a PxCooking instance to load heightfield shapes.
		PxCookingParams	cookingParameters( physics->getTolerancesScale() );

		UniquePointer<PxCooking>	cooking( PxCreateCooking( PX_PHYSICS_VERSION, physics->getFoundation(), cookingParameters ) );
		if( !cooking ) {
			_log( MessageSeverity::Error, "Error creating PhysX cooking object!" ET_UTF8_NEWLINE_LITERAL );
			return;
		}

		_log( MessageSeverity::Message, "PhysX SDK objects created successfully." ET_UTF8_NEWLINE_LITERAL );

	//	Commit changes to the service.
		_foundation = eastl::move( foundation );
		_physics	= eastl::move( physics );
		_cooking	= eastl::move( cooking );
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( ApiRegistrar& registrar ) {
		WorldService::RegisterScriptApi( registrar );
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( ConfigurationRegistrar& registrar ) {
		registrar.BeginSection( "PhysX" )
			.Register( "MaximumTaskCount", _taskCount );
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( AssetViewFactoryRegistrar& registrar ) {
		registrar.Publish( AssetViews::ArmatureView::GetExtension(), [] ( Allocator& allocator, const AssetLibrary& library, const Utf8Char* const name, Range<const char*> rawBytes ) {
				return AssetViews::ArmatureView::CreateView( allocator, library, name, rawBytes );
			} )
			.Publish( AssetViews::SkeletalColliderView::GetExtension(), [this] ( Allocator& allocator, const AssetLibrary& library, const Utf8Char* const name, Range<const char*> rawBytes ) {
				return AssetViews::SkeletalColliderView::CreateView( allocator, *_physics, library, name, rawBytes );
			} )
			.Publish( AssetViews::TerrainView::GetExtension(), [this] ( Allocator& allocator, const AssetLibrary& library, const Utf8Char* const name, Range<const char*> rawBytes ) {
				return AssetViews::TerrainView::CreateView( allocator, *_cooking, *_physics, library, name, rawBytes );
			} );
	}

// ---------------------------------------------------

	void* EngineService::allocate( size_t sizeInBytes, const char* /*typeName*/, const char* /*filename*/, int /*line*/ ) {
		void* const	result( _allocator.Allocate( sizeInBytes + PaddingInBytes, RequiredAlignment, 0u, AllocationDuration::Normal ) );

		if( !result ) {
			return nullptr;
		}

		static_cast<size_t*>(result)[PaddingInBytes/sizeof(size_t) - 1] = sizeInBytes;

		return static_cast<char*>(result) + PaddingInBytes;
	}

// ---------------------------------------------------

	void EngineService::deallocate( void* memory ) {
		if( ETBranchUnlikelyHint( !memory ) ) {
			return;
		}

		_allocator.Deallocate( static_cast<char*>(memory) - PaddingInBytes, static_cast<size_t*>(memory)[-1] );
	}

// ---------------------------------------------------

	void EngineService::reportError( PxErrorCode::Enum code, const char* message, const char* file, int line ) {
		if( code & PxErrorCode::Enum::eDEBUG_WARNING ) {
			_log( MessageSeverity::Warning, "PhysX debug warning ({}({}): {}" ET_UTF8_NEWLINE_LITERAL, file, line, message );
			return;
		}

		if( code & PxErrorCode::Enum::ePERF_WARNING ) {
			_log( MessageSeverity::Warning, "PhysX performance warning ({}({}): {}" ET_UTF8_NEWLINE_LITERAL, file, line, message );
			return;
		}

		_log( MessageSeverity::Error, "PhysX error ({}{}): {}" ET_UTF8_NEWLINE_LITERAL, file, line, message );
	}

// ---------------------------------------------------

	PxU32 EngineService::getWorkerCount() const {
		return _taskCount;
	}

// ---------------------------------------------------

	void EngineService::submitTask( PxBaseTask& task ) {
		GetActiveJobFiber().Enqueue( _simulateBarrier, [&task] ( JobFiber& /*fiber*/ ) {
			MICROPROFILE_SCOPEI( "PhysX", task.getName(), 0x76b900 );
			task.run();
			task.release();
		} );
	}

}	// namespace PhysX
}	// namespace Eldritch2
}	// namespace Physics

