/*==================================================================*\
  GameEngineService.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Configuration/ConfigurationDatabase.hpp>
#include <Foundation/GameEngineService.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Scheduler/ThreadScheduler.hpp>
#include <Packages/ContentPackage.hpp>
#include <Foundation/GameEngine.hpp>
#include <Utility/CountedResult.hpp>
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

	GameEngineService::GameEngineService( GameEngine& owningEngine ) : _owningEngine( owningEngine ) {
		owningEngine._attachedServices.PushFront( *this );
	}

// ---------------------------------------------------

	Allocator& GameEngineService::GetEngineAllocator() {
		return _owningEngine.GetAllocator();
	}

// ---------------------------------------------------

	ErrorCode GameEngineService::AllocateWorldView( Allocator& /*allocator*/, World& /*world*/ ) {
		// Default implementation does nothing.

		return Error::None;
	}

// ---------------------------------------------------

	CountedResult<World> GameEngineService::CreateWorld( const UTF8Char* const worldResourceName ) {
		ObjectHandle<World>	world( new(GetEngineAllocator(), Allocator::AllocationDuration::Normal) World( _owningEngine ), ::Eldritch2::PassthroughReferenceCountingSemantics );

		if( world ) {
			ErrorCode	operationResult( Error::None );

			world->SetProperty( World::GetMainPackageKey(), worldResourceName );

			for( auto& service : _owningEngine._attachedServices ) {
				operationResult = service.AllocateWorldView( world->GetAllocator(), *world );

				if( !operationResult ) {
					GetLogger( LogMessageType::Error )( UTF8L("Error creating world: %s!") ET_UTF8_NEWLINE_LITERAL, operationResult.ToUTF8String() );
					return { operationResult };
				}
			}

			operationResult = world->BeginContentLoad();

			if( !operationResult ) {
				GetLogger( LogMessageType::Error )( UTF8L("Error creating world: %s!") ET_UTF8_NEWLINE_LITERAL, operationResult.ToUTF8String() );
				return { operationResult };
			}

			// Transfer ownership of the world to the result object and thus to outer code.
			return { ::std::move( world ) };
		}
		
		return { Error::OutOfMemory };
	}

// ---------------------------------------------------

	const ContentLibrary& GameEngineService::GetEngineContentLibrary() const {
		return _owningEngine.GetContentLibrary();
	}

// ---------------------------------------------------

	ThreadScheduler& GameEngineService::GetEngineThreadScheduler() const {
		return _owningEngine.GetThreadScheduler();
	}

// ---------------------------------------------------

	void GameEngineService::AcceptInitializationVisitor( ScriptAPIRegistrationInitializationVisitor& /*typeRegistrar*/ ) {
		// Default implementation should not do anything.
	}

// ---------------------------------------------------

	void GameEngineService::AcceptInitializationVisitor( ConfigurationPublishingInitializationVisitor& /*visitor*/ ) {
		// Default implementation should not do anything.
	}

// ---------------------------------------------------

	void GameEngineService::AcceptInitializationVisitor( ResourceViewFactoryPublishingInitializationVisitor& /*visitor*/ ) {
		// Default implementation should not do anything.
	}

// ---------------------------------------------------

	void GameEngineService::AcceptInitializationVisitor( const PostInitializationVisitor ) {
		// Default implementation should not do anything.
	}

// ---------------------------------------------------

	void GameEngineService::AcceptTaskVisitor( WorkerContext& /*executingContext*/, WorkerContext::FinishCounter& /*finishCounter*/, const InitializeEngineTaskVisitor ) {
		// Default implementation should not do anything.
	}

// ---------------------------------------------------

	void GameEngineService::AcceptTaskVisitor( WorkerContext& /*executingContext*/, WorkerContext::FinishCounter& /*finishCounter*/, const PreConfigurationLoadedTaskVisitor ) {
		// Default implementation should not do anything.
	}

// ---------------------------------------------------

	void GameEngineService::AcceptTaskVisitor( WorkerContext& /*executingContext*/, WorkerContext::FinishCounter& /*finishCounter*/, const PostConfigurationLoadedTaskVisitor ) {
		// Default implementation should not do anything.
	}

// ---------------------------------------------------

	void GameEngineService::AcceptTaskVisitor( WorkerContext& /*executingContext*/, WorkerContext::FinishCounter& /*finishCounter*/, const ServiceTickTaskVisitor ) {
		// Default implementation should not do anything.
	}

// ---------------------------------------------------

	void GameEngineService::AcceptTaskVisitor( WorkerContext& /*executingContext*/, WorkerContext::FinishCounter& /*finishCounter*/, const WorldTickTaskVisitor ) {
		// Default implementation should not do anything.
	}

// ---------------------------------------------------

	ErrorCode GameEngineService::LaunchThread( Thread& thread ) {
		return _owningEngine.GetThreadScheduler().Enqueue( thread );
	}

// ---------------------------------------------------

	Logger& GameEngineService::GetLogger( const LogMessageType type ) const {
		return _owningEngine.GetLoggerForMessageType( type );
	}

}	// namespace Foundation
}	// namespace Eldritch2