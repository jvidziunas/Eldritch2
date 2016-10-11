/*==================================================================*\
  EngineService.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/EngineService.hpp>
#include <Core/WorldService.hpp>
#include <Utility/Result.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::Scheduling;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Assets;
using namespace ::Eldritch2::Core;

namespace Eldritch2 {
namespace Core {

	Result<UniquePointer<WorldService>> EngineService::CreateWorldService( Allocator& /*allocator*/, const World& /*world*/ ) {
	//	Default implementation does nothing.
		return UniquePointer<WorldService>( nullptr );
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( JobFiber& /*fiber*/, const BeginInitializationVisitor ) {
	//	Default implementation should not do anything.
		MICROPROFILE_SCOPEI( GetName(), "Basic initialization", 0xBBBBBB );
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( JobFiber& /*fiber*/, const ConfigurationBroadcastVisitor ) {
	//	Default implementation should not do anything.
		MICROPROFILE_SCOPEI( GetName(), "Configuration broadcast", 0xBBBBBB );
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( ApiRegistrar& /*typeRegistrar*/ ) {
	//	Default implementation should not do anything.
		MICROPROFILE_SCOPEI( GetName(), "Script API registration", 0xBBBBBB );
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( ConfigurationRegistrar& /*visitor*/ ) {
	//	Default implementation should not do anything.
		MICROPROFILE_SCOPEI( GetName(), "Configuration registration", 0xBBBBBB );
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( AssetViewFactoryRegistrar& /*visitor*/ ) {
	//	Default implementation should not do anything.
		MICROPROFILE_SCOPEI( GetName(), "Asset view factory registration", 0xBBBBBB );
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( ServiceBlackboard& /*blackboard*/ ) {
	//	Default implementation should not do anything.
		MICROPROFILE_SCOPEI( GetName(), "Shared service registration", 0xBBBBBB );
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( JobFiber& /*fiber*/, const InitializationCompleteVisitor ) {
	//	Default implementation should not do anything.
		MICROPROFILE_SCOPEI( GetName(), "Initialization finalization", 0xBBBBBB );
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( JobFiber& /*fiber*/, const ServiceTickVisitor ) {
	//	Default implementation should not do anything.
		MICROPROFILE_SCOPEI( GetName(), "Service tick", 0xBBBBBB );
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( JobFiber& /*fiber*/, const WorldTickVisitor ) {
	//	Default implementation should not do anything.
		MICROPROFILE_SCOPEI( GetName(), "World tick", 0xBBBBBB );
	}

}	// namespace Core
}	// namespace Eldritch2