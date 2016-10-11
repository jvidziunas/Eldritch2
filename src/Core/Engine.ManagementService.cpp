/*==================================================================*\
  Engine.ManagementService.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Configuration/ConfigurationRegistrar.hpp>
#include <Assets/AssetViewFactoryRegistrar.hpp>
#include <Scheduling/JobFiber.hpp>
#include <Assets/LiveSet.hpp>
#include <Core/Engine.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::Scheduling;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Platform;
using namespace ::Eldritch2::Logging;
using namespace ::Eldritch2::Assets;

namespace Eldritch2 {
namespace Core {

	Engine::ManagementService::ManagementService( Engine& engine ) : Core::EngineService( engine.GetServiceBlackboard() ), _engine( engine ) {}

// ---------------------------------------------------

	Utf8Literal Engine::ManagementService::GetName() const {
		return "Game Engine Management Service";
	}

// ---------------------------------------------------

	void Engine::ManagementService::AcceptVisitor( JobFiber& /*executor*/, const BeginInitializationVisitor ) {
	//	Collect all sub-services and publish them to the blackboard.
		_engine.VisitServices( _engine._blackboard );

	//	Collect all asset view creation event handlers and publish them to the asset library.
		_engine.VisitServices( AssetViewFactoryRegistrar( _engine._assetLibrary ) );
	}

// ---------------------------------------------------

	void Engine::ManagementService::AcceptVisitor( JobFiber& executor, const WorldTickVisitor ) {
		MICROPROFILE_SCOPEI( GetName(), "Tick worlds", 0xF32334 );

		JobBarrier	tickBarrier( 0 );

	//	Garbage collect only if we know a world has shut down-- this is the only time when a change to the live package set takes place.
		if( 0 != _engine.DestroyShutDownWorlds() ) {
			executor.Enqueue( tickBarrier, [this] ( JobFiber& /*executor*/ ) {
				LiveSet	livePackages( _engine.GetAllocator() );

			//	Build a list of all packages still referenced by our world(s).
				_engine.VisitWorlds( livePackages );

				_engine._assetLibrary.DestroyUnreferencedPackages( livePackages );
			} );
		}

		_engine.VisitWorlds( executor, World::TickVisitor() );

		executor.AwaitBarrier( tickBarrier );
	}

// ---------------------------------------------------

	void Engine::ManagementService::AcceptVisitor( ConfigurationRegistrar& visitor ) {
		visitor.BeginSection( "Engine" )
			.Register( "LogThreshold", [this] ( Range<const Utf8Char*> value ) {
				MessageSeverity	threshold;

				_engine.GetLog().SetMuteThreshold( threshold );
			} );

		visitor.BeginSection( "Engine.DefaultWorldProperties" )
			.RegisterDynamicKeySetter( [this] ( Utf8Literal valueName, Range<const Utf8Char*> value ) {
				Allocator&	allocator( _engine.GetAllocator() );

				_engine._worldDefaultProperties.Emplace( Utf8String<>( valueName, { allocator, "World Property Key Allocator" } ),
														 Utf8String<>( value.Begin(), value.End(), { allocator, "World Property String Allocator" } ) );
			} );
	}

}	// namespace Core
}	// namespace Eldritch2