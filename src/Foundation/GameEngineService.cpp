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
#include <Utility/DisposingResultPair.hpp>
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

	DisposingResultPair<World> GameEngineService::CreateWorld( const UTF8Char* const worldResourceName ) {
		DisposingResultPair<World>	result { nullptr, Error::UNSPECIFIED };

		if( auto worldContent = _owningEngine._contentLibrary.ResolvePackageByName( worldResourceName ) ) {
			if( ObjectHandle<World>	world { new(GetEngineAllocator(), Allocator::AllocationOption::PERMANENT_ALLOCATION) World( ::std::move( worldContent.object ), _owningEngine ), ::Eldritch2::PassthroughReferenceCountingSemantics } ) {
				if( world->GetLastError() ) {
					// Transfer ownership of the world to the result object and thus to outer code.
					result.object		= ::std::move( world );
					result.resultCode	= Error::NONE;
				} else {
					result.resultCode = world->GetLastError();
				}
			} else {
				result.resultCode = Error::OUT_OF_MEMORY;
			}
		}

		if( !result.resultCode ) {
			FormatAndLogError( UTF8L("Unable to create world '%s': %s.") ET_UTF8_NEWLINE_LITERAL, worldResourceName, result.resultCode.ToUTF8String() );
		}
		
		return result;
	}

// ---------------------------------------------------

	DisposingResultPair<World> GameEngineService::CreateEditorWorld() {
		DisposingResultPair<World>	result { nullptr, Error::UNSPECIFIED };

		if( auto worldContent = _owningEngine._contentLibrary.CreatePackageForEditorWorld() ) {
			if( ObjectHandle<World>	world { new(GetEngineAllocator(), Allocator::AllocationOption::PERMANENT_ALLOCATION) World( ::std::move( worldContent.object ), _owningEngine ), ::Eldritch2::PassthroughReferenceCountingSemantics } ) {
				if( world->GetLastError() ) {
					result.object = ::std::move( world );
				}

				result.resultCode = world->GetLastError();
			} else {
				result.resultCode = Error::OUT_OF_MEMORY;
			}
		}

		if( !result.resultCode ) {
			FormatAndLogError( UTF8L("Unable to create editor world: %s.") ET_UTF8_NEWLINE_LITERAL, result.resultCode.ToUTF8String() );
		}

		return result;
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

	void GameEngineService::FormatAndLogString( ETFormatStringHint( const UTF8Char* str ), ... ) const {
		UTF8Char	errorString[MESSAGE_BUFFER_SIZE];
		va_list		args;

		va_start( args, str );
			PrintFormatted( errorString, str, args );
		va_end( args );

		_owningEngine.GetLoggerForMessageType( GameEngine::LogMessageType::VERBOSE_WARNING ).WriteString( errorString );
	}

// ---------------------------------------------------

	void GameEngineService::FormatAndLogWarning( ETFormatStringHint( const UTF8Char* str ), ... ) const {
		UTF8Char	errorString[MESSAGE_BUFFER_SIZE];
		va_list		args;

		va_start( args, str );
			PrintFormatted( errorString, str, args );
		va_end( args );

		_owningEngine.GetLoggerForMessageType( GameEngine::LogMessageType::WARNING ).WriteString( errorString );
	}

// ---------------------------------------------------

	void GameEngineService::FormatAndLogVerboseWarning( ETFormatStringHint( const UTF8Char* str ), ... ) const {
		UTF8Char	errorString[MESSAGE_BUFFER_SIZE];
		va_list		args;

		va_start( args, str );
			PrintFormatted( errorString, str, args );
		va_end( args );

		_owningEngine.GetLoggerForMessageType( GameEngine::LogMessageType::VERBOSE_WARNING ).WriteString( errorString );
	}

// ---------------------------------------------------

	void GameEngineService::FormatAndLogError( ETFormatStringHint( const UTF8Char* str ), ... ) const {
		UTF8Char	errorString[MESSAGE_BUFFER_SIZE];
		va_list		args;

		va_start( args, str );
			PrintFormatted( errorString, str, args );
		va_end( args );

		_owningEngine.GetLoggerForMessageType( GameEngine::LogMessageType::ERROR ).WriteString( errorString );
	}

}	// namespace Foundation
}	// namespace Eldritch2