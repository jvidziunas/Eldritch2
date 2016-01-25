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
using namespace ::std;

#if( ET_COMPILER_IS_MSVC )
#	pragma warning( push )
//	Disable 'conditional expression is constant' warning
#		pragma warning( disable : 4127 )
#endif

namespace Eldritch2 {
namespace Foundation {

	GameEngine::GameEngine( SystemInterface& systemInterface, ThreadScheduler& scheduler, ContentProvider& contentProvider, Allocator& allocator ) : _allocator( allocator, UTF8L("Game Engine Allocator") ),
																																					 _logger( contentProvider, UTF8L("Eldritch2.log") ),
																																					 _systemInterface( systemInterface ),
																																					 _scheduler( scheduler ),
																																					 _contentLibrary( contentProvider, scheduler, _allocator ),
																																					 _logEchoThreshold( LogMessageType::VerboseWarning ),
																																					 _taskArenaPerThreadAllocationSizeInBytes( 1024u * 1024u ),
																																					 _worldArenaSizeInBytes( 16u * 1024u * 1024u ),
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

		return (_logEchoThreshold <= type) ? static_cast<Logger&>(_logger) : static_cast<Logger&>(nullLogger);
	}

// ---------------------------------------------------

	size_t GameEngine::CalculateFrameArenaSizeInBytes() const {
		return (_taskArenaPerThreadAllocationSizeInBytes * _scheduler.GetMaximumTaskParallelism()) + 1024u;
	}

// ---------------------------------------------------

	void GameEngine::ClearAttachedServices() {
		_attachedServices.Clear();
	}

// ---------------------------------------------------

	void GameEngine::Dispose() {
		GetLoggerForMessageType( LogMessageType::Message )( UTF8L("Terminating execution.") ET_UTF8_NEWLINE_LITERAL );
		_scheduler.FlagForShutdown();
	}

}	// namespace Foundation
}	// namespace Eldritch2

#if( ET_COMPILER_IS_MSVC )
//	Enable 'conditional expression is constant' warning
#	pragma warning( pop )
#endif