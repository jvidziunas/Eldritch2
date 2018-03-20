/*==================================================================*\
  PhysxEngineComponent.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/PhysxEngineComponent.hpp>
#include <Physics/PhysX/PhysxWorldComponent.hpp>
#include <Core/Engine.hpp>
//------------------------------------------------------------------//
ET_PUSH_COMPILER_WARNING_STATE()
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
	ET_SET_MSVC_WARNING_STATE( disable : 6326 )
#	include <foundation/PxFoundationVersion.h>
#	include <common/PxTolerancesScale.h>
#	include <foundation/PxFoundation.h>
#	include <PxPhysicsVersion.h>
#	include <PxPhysics.h>
ET_POP_COMPILER_WARNING_STATE()
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_DEBUGGABLE_LIBRARY( "PxFoundationDEBUG_x64.lib",				"PxFoundation_x64.lib" );
ET_LINK_DEBUGGABLE_LIBRARY( "PhysX3ExtensionsDEBUG.lib",				"PhysX3Extensions.lib" );
ET_LINK_DEBUGGABLE_LIBRARY( "PhysX3CommonDEBUG_x64.lib",				"PhysX3Common_x64.lib" );
ET_LINK_DEBUGGABLE_LIBRARY( "PhysX3DEBUG_x64.lib",						"PhysX3_x64.lib" );
ET_LINK_DEBUGGABLE_LIBRARY( "PhysX3GpuDEBUG_x64.lib",					"PhysX3Gpu_x64.lib" );
ET_LINK_DEBUGGABLE_LIBRARY( "PhysX3CharacterKinematicDEBUG_x64.lib",	"PhysX3CharacterKinematic_x64.lib" );
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;
	using namespace ::physx;

	PhysxEngineComponent::PhysxEngineComponent( const Blackboard& services, Log& log ) : EngineComponent( services ), _log( log ), _allocator( "PhysX Root Allocator" ) {}

// ---------------------------------------------------

	Result<UniquePointer<WorldComponent>> PhysxEngineComponent::CreateWorldComponent( Allocator& allocator, const World& world ) {
		UniquePointer<WorldComponent> physxComponent( CreateUnique<PhysxWorldComponent>( allocator, world ) );
		if (!physxComponent) {
			return Error::OutOfMemory;
		}
	
		return eastl::move( physxComponent );
	}

// ---------------------------------------------------

	void PhysxEngineComponent::AcceptVisitor( JobExecutor& /*executor*/, const InitializationVisitor ) {
		MICROPROFILE_SCOPEI( "Engine/Initialization/PhysX", "PhysX SDK creation", 0x76b900 );

	//	Create the PxFoundation loader object responsible for loading the various PhysX modules.
		if (!PxCreateFoundation( PX_FOUNDATION_VERSION, _allocator, _log )) {
			_log.Write( MessageType::Error, "Error creating PhysX foundation object!" UTF8_NEWLINE );
			FindService<Engine>().SetShouldShutDown();
			return;
		}

	//	Create the core PhysX object used to create scenes, actors, shapes, etc.
		if (!PxCreatePhysics( PX_PHYSICS_VERSION, PxGetFoundation(), PxTolerancesScale() )) {
			_log.Write( MessageType::Error, "Error creating PhysX object!" UTF8_NEWLINE );
			FindService<Engine>().SetShouldShutDown();
			return;
		}

		_log.Write( MessageType::Message, "PhysX SDK initialized successfully." UTF8_NEWLINE );
	}

}	// namespace PhysX
}	// namespace Eldritch2
}	// namespace Physics