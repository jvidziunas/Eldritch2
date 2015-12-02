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
#include <Scheduler/TaskScheduler.hpp>
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

	GameEngine::GameEngine( SystemInterface& systemInterface, TaskScheduler& scheduler, ContentProvider& contentProvider, Allocator& allocator ) : _allocator( allocator, UTF8L("Game Engine Allocator") ),
																																				   _logger( contentProvider, UTF8L("Eldritch2.log") ),
																																				   _systemInterface( systemInterface ),
																																				   _scheduler( scheduler ),
																																				   _worldViewFactories( 0u, { _allocator, UTF8L("Game Engine World View Factory Collection Allocator") } ),
																																				   _worldViewAllocationHintInBytes( 16u ),
																																				   _contentLibrary( contentProvider, scheduler, _allocator ),
																																				   _logEchoThreshold( LogMessageType::VERBOSE_WARNING ),
																																				   _taskArenaPerThreadAllocationSizeInBytes( 1024u * 1024u ),
																																				   _worldArenaSizeInBytes( 16u * 1024u * 1024u ),
																																				   _managementService( *this ) {}

// ---------------------------------------------------

	int GameEngine::ApplicationEntryPoint() {
		_managementService.BootstrapEngine( Max<size_t>( _systemInterface.GetThreadCount() - 1u, 1u ) );

		return 0;
	}

// ---------------------------------------------------

	Logger&	GameEngine::GetLoggerForMessageType( const LogMessageType type ) {
		class NullLogger : public Logger {
		public:
			void	Write( const UTF8Char* const /*string*/, const size_t /*lengthInOctets*/ ) override sealed {}
		};

	// ---

		static NullLogger	nullLogger;

		return (_logEchoThreshold <= type) ? static_cast<Logger&>(_logger) : static_cast<Logger&>(nullLogger);
	}

// ---------------------------------------------------

	ErrorCode GameEngine::InstantiateViewsForWorld( Allocator& allocator, World& world ) {
		for( const auto& viewFactory : _worldViewFactories ) {
			const auto	result( viewFactory.factoryFunction( allocator, world, viewFactory.userParameter ) );

			if( !result ) {
				return result;
			}
		}

		return Error::NONE;
	}

// ---------------------------------------------------

	size_t GameEngine::CalculateFrameArenaSizeInBytes() const {
		return (_taskArenaPerThreadAllocationSizeInBytes * _scheduler.GetMaximumTaskParallelism()) + 1024u;
	}

// ---------------------------------------------------

	void GameEngine::ClearAttachedServices() {
		_worldViewFactories.Clear( ::Eldritch2::ReleaseMemorySemantics );
		_attachedServices.Clear();
	}

// ---------------------------------------------------

	void GameEngine::Dispose() {
		GetLoggerForMessageType( LogMessageType::MESSAGE )( UTF8L("Terminating execution.") ET_UTF8_NEWLINE_LITERAL );
		_scheduler.FlagForShutdown();
	}

}	// namespace Foundation
}	// namespace Eldritch2

#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif