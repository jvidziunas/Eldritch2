/*==================================================================*\
  GameEngine.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Math/StandardLibrary.hpp>
#include <Scheduler/ThreadScheduler.hpp>
#include <Scheduler/WorkerContext.hpp>
#include <System/SystemInterface.hpp>
#include <Foundation/GameEngine.hpp>
#include <Logging/Logger.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2::System;
using namespace ::Eldritch2;

#if( ET_COMPILER_IS_MSVC )
#	pragma warning( push )
//	Disable 'conditional expression is constant' warning
#		pragma warning( disable : 4127 )
#endif

namespace Eldritch2 {
namespace Foundation {

	GameEngine::GameEngine( SystemInterface& systemInterface, ThreadScheduler& scheduler, ContentProvider& contentProvider, Allocator& allocator ) : _allocator( allocator, UTF8L("Game Engine Allocator") ),
																																					 _logger( contentProvider, UTF8L("Logs") ET_UTF8_DIR_SEPARATOR UTF8L("Eldritch2.log"), _allocator ),
																																					 _systemInterface( systemInterface ),
																																					 _scheduler( scheduler ),
																																					 _contentLibrary( contentProvider, scheduler, _allocator ),
																																					 _logEchoThreshold( LogMessageType::VerboseWarning ),
																																					 _worldArenaSizeInBytes( 16u * 1024u * 1024u ),
																																					 _worldMutex( scheduler.AllocateReaderWriterUserMutex( _allocator ).object, { _allocator } ),
																																					 _worldCount( 0u ),
																																					 _managementService( *this ) {}

// ---------------------------------------------------

	int GameEngine::ApplicationEntryPoint() {
		const auto	threadCount( Max<size_t>( GetSystemInterface().GetThreadCount() - 1u, 1u ) );

		GetThreadScheduler().Bootstrap( threadCount, { &_managementService, [] ( void* managementService, WorkerContext& executingContext ) {
			static_cast<ManagementService*>(managementService)->InitializeEngineAndLaunchFrameLoop( executingContext );
		} } );

		return 0;
	}

// ---------------------------------------------------

	Logger&	GameEngine::GetLoggerForMessageType( const LogMessageType type ) const {
		class NullLogger : public Logger {
		public:
			void	Write( const UTF8Char* const /*string*/, const size_t /*lengthInOctets*/ ) override sealed {}
		};

	// ---

		static NullLogger	nullLogger;

		return (static_cast<LogMessageType>(_logEchoThreshold) <= type) ? static_cast<Logger&>(_logger) : static_cast<Logger&>(nullLogger);
	}

// ---------------------------------------------------

	void GameEngine::NotifyOfNewWorld( World& world ) {
		{	ScopedLock	_( *_worldMutex );

			_worlds.PushFront( world );
			_worldCount.fetch_add( 1u, ::std::memory_order_relaxed );
		}	// End of lock scope.
		
		GetLoggerForMessageType( LogMessageType::Message )( UTF8L("Attached world {}.") ET_UTF8_NEWLINE_LITERAL, static_cast<void*>(&world) );
	}

// ---------------------------------------------------

	void GameEngine::NotifyOfWorldDestruction( World& world ) {
		{	ScopedLock	_( *_worldMutex );

			_worlds.Erase( _worlds.IteratorTo( world ) );
			_worldCount.fetch_sub( 1u, ::std::memory_order_acq_rel );
		}	// End of lock scope.

		GetLoggerForMessageType( LogMessageType::Message )( UTF8L("Detached world {}.") ET_UTF8_NEWLINE_LITERAL, static_cast<void*>(&world) );
	}

}	// namespace Foundation
}	// namespace Eldritch2

#if( ET_COMPILER_IS_MSVC )
//	Enable 'conditional expression is constant' warning
#	pragma warning( pop )
#endif