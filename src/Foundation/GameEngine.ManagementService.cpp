/*==================================================================*\
  GameEngine.ManagementService.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Packages/ResourceViewFactoryPublishingInitializationVisitor.hpp>
#include <Configuration/ConfigurationPublishingInitializationVisitor.hpp>
#include <Scheduler/ThreadScheduler.hpp>
#include <Scheduler/WorkerContext.hpp>
#include <Foundation/GameEngine.hpp>
#include <Foundation/World.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Foundation {

	GameEngine::ManagementService::ManagementService( GameEngine& owningEngine ) : GameEngineService( owningEngine ) {}

// ---------------------------------------------------

	const UTF8Char* const GameEngine::ManagementService::GetName() const {
		return UTF8L("Game Engine Management Service");
	}

// ---------------------------------------------------

	void GameEngine::ManagementService::InitializeEngineAndLaunchFrameLoop( Scheduler::WorkerContext& executingContext ) {
		::MicroProfileSetForceEnable( true );
		::MicroProfileSetEnableAllGroups( true );
		::MicroProfileSetForceMetaCounters( true );

		{	MICROPROFILE_SCOPEI( "Initialization", "Initialization", 0xAAAAAA );
			InvokeInitializationFunction<&GameEngineService::OnEngineInitializationStarted>( executingContext );
			InvokeInitializationFunction<&GameEngineService::OnEngineInitializationCompleted>( executingContext );
		}

		// Log a successful initialization...
		GetLogger( LogMessageType::Message )( UTF8L("Engine initialization complete!") ET_UTF8_NEWLINE_LITERAL );

		// Execute while we have worlds.
		while( 0u != _owningEngine._worldCount.load( ::std::memory_order_consume ) ) {
			::MicroProfileFlip( nullptr );

			{	MICROPROFILE_SCOPEI( "Main", "Frame", 0xAAAAAA );
				{	MICROPROFILE_SCOPEI( "Main", "Engine Service Tick", 0xEEEEEE );
					InvokeTickFunction<&GameEngineService::OnServiceTickStarted>( executingContext );
				}
		
				{	MICROPROFILE_SCOPEI( "Main", "World Tick", 0xFFFFFF );
					InvokeTickFunction<&GameEngineService::OnWorldTickStarted>( executingContext );
				}
			}
		}

		GetLogger( LogMessageType::Message )( UTF8L("Terminating execution.") ET_UTF8_NEWLINE_LITERAL );
		_owningEngine.GetThreadScheduler().FlagForShutdown();
	}

// ---------------------------------------------------

	void GameEngine::ManagementService::OnEngineInitializationStarted( WorkerContext& /*executingContext*/ ) {
		BroadcastInitializationVisitor( ResourceViewFactoryPublishingInitializationVisitor( _owningEngine._contentLibrary ) );
	}

// ---------------------------------------------------

	void GameEngine::ManagementService::OnWorldTickStarted( WorkerContext& executingContext ) {
		WorkerContext::FinishCounter	finishCounter( 0 );

		{	ScopedReaderLock	_( *_owningEngine._worldMutex );
			for( auto& world : _owningEngine._worlds ) {
				world.QueueUpdateTasks( executingContext, finishCounter );
			}
		}	// End of lock scope.
		
		executingContext.WaitForCounter( finishCounter );
	}

// ---------------------------------------------------

	void GameEngine::ManagementService::AcceptInitializationVisitor( ConfigurationPublishingInitializationVisitor& visitor ) {
		visitor.PushSection( UTF8L("Engine") );

		visitor.Register( UTF8L("LogEchoThreshold"), _owningEngine._logEchoThreshold );
	}

}	// namespace Foundation
}	// namespace Eldritch2