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
#include <Packages/ContentLibrary.hpp>
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
		owningEngine.NotifyOfNewService( *this );
	}

// ---------------------------------------------------

	GameEngineService::~GameEngineService() {
		_owningEngine.NotifyOfServiceDestruction( *this );
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
		if( ObjectHandle<World>	world{ new(GetEngineAllocator(), Allocator::AllocationDuration::Normal) World( _owningEngine, worldResourceName ), ::Eldritch2::PassthroughReferenceCountingSemantics } ) {
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

	ContentProvider& GameEngineService::GetEngineContentProvider() const {
		return GetEngineContentLibrary().GetContentProvider();
	}

// ---------------------------------------------------

	ThreadScheduler& GameEngineService::GetEngineThreadScheduler() const {
		return _owningEngine.GetThreadScheduler();
	}

// ---------------------------------------------------

	void GameEngineService::OnEngineInitializationStarted( WorkerContext& /*executingContext*/ ) {
		// Default implementation should not do anything.
	}

// ---------------------------------------------------

	void GameEngineService::OnEngineConfigurationBroadcast( WorkerContext& /*executingContext*/ ) {
		// Default implementation should not do anything.
	}

// ---------------------------------------------------

	void GameEngineService::AcceptInitializationVisitor( ScriptApiRegistrationInitializationVisitor& /*typeRegistrar*/ ) {
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

	void GameEngineService::OnEngineInitializationCompleted( WorkerContext& /*executingContext*/ ) {
		// Default implementation should not do anything.
	}

// ---------------------------------------------------

	void GameEngineService::OnServiceTickStarted( WorkerContext& /*executingContext*/ ) {
		// Default implementation should not do anything.
	}

// ---------------------------------------------------

	void GameEngineService::OnWorldTickStarted( WorkerContext& /*executingContext*/ ) {
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