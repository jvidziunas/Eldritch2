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
		//! This is responsible for keeping the engine alive for the duration of initialization.
		ObjectHandle<GameEngine>	engineReference( _owningEngine, ::Eldritch2::PassthroughReferenceCountingSemantics );

		{	MICROPROFILE_SCOPEI( "Initialization", "Initialization", 0xAAAAAAAA );
			{	WorkerContext::FinishCounter	initializationCounter( 0 );
				BroadcastTaskVisitor( executingContext, initializationCounter, GameEngineService::InitializeEngineTaskVisitor() );

				// Wait for all initialization tasks to complete.
				executingContext.WaitForCounter( initializationCounter );
			}

			BroadcastInitializationVisitor( GameEngineService::PostInitializationVisitor() );
		}

		// Log a successful initialization...
		GetLogger( LogMessageType::Message )(UTF8L( "Engine initialization complete!" ) ET_UTF8_NEWLINE_LITERAL);

		// ... flip the initialization 'frame'...
		::MicroProfileFlip( nullptr );

		// ... and begin the frame loop.
		while( !engineReference.IsSoleReferenceToObject() ) {
			{	MICROPROFILE_SCOPEI( "Main", "Frame", 0xAAAAAAAA );
				{	MICROPROFILE_SCOPEI( "Main", "Engine Service Tick", 0xEEEEEEEE );
					WorkerContext::FinishCounter	tickServicesCounter( 0 );
					BroadcastTaskVisitor( executingContext, tickServicesCounter, GameEngineService::ServiceTickTaskVisitor() );

					executingContext.WaitForCounter( tickServicesCounter );
				}
		
				{	MICROPROFILE_SCOPEI( "Main", "World Tick", 0xFFFFFFFF );
					WorkerContext::FinishCounter	tickWorldsCounter( 0 );
					BroadcastTaskVisitor( executingContext, tickWorldsCounter, GameEngineService::WorldTickTaskVisitor() );

					executingContext.WaitForCounter( tickWorldsCounter );
				}
			}

			::MicroProfileFlip( nullptr );
		}
	}

// ---------------------------------------------------

	void GameEngine::ManagementService::AcceptTaskVisitor( WorkerContext& executingContext, WorkerContext::FinishCounter& finishCounter, const InitializeEngineTaskVisitor ) {
		executingContext.Enqueue( finishCounter, { this, [] ( void* managementService, WorkerContext& /*executingContext*/ ) {
			static_cast<ManagementService*>(managementService)->BroadcastInitializationVisitor( ResourceViewFactoryPublishingInitializationVisitor( static_cast<ManagementService*>(managementService)->_owningEngine._contentLibrary ) );
		} } );
	}

// ---------------------------------------------------

	void GameEngine::ManagementService::AcceptTaskVisitor( WorkerContext& executingContext, WorkerContext::FinishCounter& finishCounter, const WorldTickTaskVisitor ) {
		_owningEngine._tickingWorlds.ClearAndDispose( [&executingContext, &finishCounter] ( World& tickingWorld ) {
			tickingWorld.QueueUpdateTasks( executingContext, finishCounter );
		} );
	}

// ---------------------------------------------------

	void GameEngine::ManagementService::AcceptInitializationVisitor( ConfigurationPublishingInitializationVisitor& visitor ) {
		visitor.PushSection( UTF8L("Engine") );

		visitor.Register( UTF8L("LogEchoThreshold"), _owningEngine._logEchoThreshold ).Register( UTF8L("TaskArenaPerThreadAllocationSizeInBytes"), _owningEngine._taskArenaPerThreadAllocationSizeInBytes );
	}

}	// namespace Foundation
}	// namespace Eldritch2