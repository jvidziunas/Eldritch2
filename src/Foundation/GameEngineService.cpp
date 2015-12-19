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
#include <Utility/CountedResult.hpp>
#include <Scheduler/TaskScheduler.hpp>
#include <Packages/ContentPackage.hpp>
#include <Foundation/GameEngine.hpp>
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

		return Error::NONE;
	}

// ---------------------------------------------------

	CountedResult<World> GameEngineService::CreateWorld( const UTF8Char* const worldResourceName ) {
		ObjectHandle<World>	world( new(GetEngineAllocator(), Allocator::AllocationOption::PERMANENT_ALLOCATION) World( _owningEngine ), ::Eldritch2::PassthroughReferenceCountingSemantics );

		if( world ) {
			ErrorCode	operationResult( Error::NONE );

			world->SetProperty( World::GetMainPackageKey(), worldResourceName );

			for( auto& service : _owningEngine._attachedServices ) {
				operationResult = service.AllocateWorldView( world->GetAllocator(), *world );

				if( !operationResult ) {
					GetLogger( LogMessageType::ERROR )( UTF8L("Error creating world: %s!") ET_UTF8_NEWLINE_LITERAL, operationResult.ToUTF8String() );
					return { operationResult };
				}
			}

			operationResult = world->BeginContentLoad();

			if( !operationResult ) {
				GetLogger( LogMessageType::ERROR )( UTF8L("Error creating world: %s!") ET_UTF8_NEWLINE_LITERAL, operationResult.ToUTF8String() );
				return { operationResult };
			}

			// Transfer ownership of the world to the result object and thus to outer code.
			return { ::std::move( world ) };
		}
		
		return { Error::OUT_OF_MEMORY };
	}

// ---------------------------------------------------

	const ContentLibrary& GameEngineService::GetEngineContentLibrary() const {
		return _owningEngine.GetContentLibrary();
	}

// ---------------------------------------------------

	TaskScheduler& GameEngineService::GetEngineTaskScheduler() const {
		return _owningEngine.GetTaskScheduler();
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

	void GameEngineService::AcceptTaskVisitor( Allocator& /*subtaskAllocator*/, Task& /*visitingTask*/, WorkerContext& /*executingContext*/, const InitializeEngineTaskVisitor ) {
		// Default implementation should not do anything.
	}

// ---------------------------------------------------

	void GameEngineService::AcceptTaskVisitor( Allocator& /*subtaskAllocator*/, Task& /*visitingTask*/, WorkerContext& /*executingContext*/, const PreConfigurationLoadedTaskVisitor ) {
		// Default implementation should not do anything.
	}

// ---------------------------------------------------

	void GameEngineService::AcceptTaskVisitor( Allocator& /*subtaskAllocator*/, Task& /*visitingTask*/, WorkerContext& /*executingContext*/, const PostConfigurationLoadedTaskVisitor ) {
		// Default implementation should not do anything.
	}

// ---------------------------------------------------

	void GameEngineService::AcceptTaskVisitor( Allocator& /*subtaskAllocator*/, Task& /*visitingTask*/, WorkerContext& /*executingContext*/, const ServiceTickTaskVisitor ) {
		// Default implementation should not do anything.
	}

// ---------------------------------------------------

	void GameEngineService::AcceptTaskVisitor( Allocator& /*subtaskAllocator*/, Task& /*visitingTask*/, WorkerContext& /*executingContext*/, const WorldTickTaskVisitor ) {
		// Default implementation should not do anything.
	}

// ---------------------------------------------------

	ErrorCode GameEngineService::LaunchThread( Thread& thread ) {
		return _owningEngine.GetTaskScheduler().Enqueue( thread );
	}

// ---------------------------------------------------

	Logger& GameEngineService::GetLogger( const LogMessageType type ) const {
		return _owningEngine.GetLoggerForMessageType( type );
	}

}	// namespace Foundation
}	// namespace Eldritch2