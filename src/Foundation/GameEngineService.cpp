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
#include <Utility/DisposingResult.hpp>
#include <Utility/Memory/InstanceNew.hpp>
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
using namespace ::std;

namespace {

	enum : size_t {
		MESSAGE_BUFFER_SIZE = 256u
	};

}	// anonymous namespace

namespace Eldritch2 {
namespace Foundation {

	GameEngineService::GameEngineService( GameEngine& owningEngine ) : _owningEngine( owningEngine ) {
		owningEngine._attachedServices.PushFront( *this );
	}

// ---------------------------------------------------

	Allocator& GameEngineService::GetEngineAllocator() {
		return _owningEngine._allocator;
	}

// ---------------------------------------------------

	DisposingResult<World> GameEngineService::CreateWorld( const UTF8Char* const worldResourceName ) {
		auto	createWorldPackageResult( _owningEngine._contentLibrary.ResolvePackageByName( worldResourceName ) );

		if( createWorldPackageResult ) {
			if( ObjectHandle<World>	world { new(GetEngineAllocator(), Allocator::AllocationOption::PERMANENT_ALLOCATION) World( move( createWorldPackageResult.object ), _owningEngine ), ::Eldritch2::PassthroughReferenceCountingSemantics } ) {
				if( world->GetLastError() ) {
					// Transfer ownership of the world to the result object and thus to outer code.
					return { move( world ) };
				} else {
					return { world->GetLastError() };
				}
			}

			return { Error::OUT_OF_MEMORY };
		}

		return { createWorldPackageResult.resultCode };
	}

// ---------------------------------------------------

	DisposingResult<World> GameEngineService::CreateEditorWorld() {
		auto	createWorldPackageResult( _owningEngine._contentLibrary.CreatePackageForEditorWorld() );

		if( createWorldPackageResult ) {
			if( ObjectHandle<World>	world { new(GetEngineAllocator(), Allocator::AllocationOption::PERMANENT_ALLOCATION) World( move( createWorldPackageResult.object ), _owningEngine ), ::Eldritch2::PassthroughReferenceCountingSemantics } ) {
				if( world->GetLastError() ) {
					// Transfer ownership of the world to the result object and thus to outer code.
					return { move( world ) };
				} else {
					return { world->GetLastError() };
				}
			}

			return { Error::OUT_OF_MEMORY };
		}

		return { createWorldPackageResult.resultCode };
	}

// ---------------------------------------------------

	const ContentLibrary& GameEngineService::GetEngineContentLibrary() const {
		return _owningEngine._contentLibrary;
	}

// ---------------------------------------------------

	TaskScheduler& GameEngineService::GetEngineTaskScheduler() const {
		return _owningEngine._scheduler;
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

	void GameEngineService::AcceptInitializationVisitor( WorldViewFactoryPublishingInitializationVisitor& /*visitor*/ ) {
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
		return _owningEngine._scheduler.Enqueue( thread );
	}

// ---------------------------------------------------

	Logger& GameEngineService::GetLogger( const LogMessageType type ) const {
		return _owningEngine.GetLoggerForMessageType( type );
	}

}	// namespace Foundation
}	// namespace Eldritch2