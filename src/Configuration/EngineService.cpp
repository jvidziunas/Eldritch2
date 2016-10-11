/*==================================================================*\
  EngineService.cpp
  ------------------------------------------------------------------
  Purpose:
  Lightweight wrapper over an INI file parser and query system.
  Engine subsystems can request (or set) the value of one or more
  properties in the bound file in order to save user preferences.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Configuration/ConfigurationRegistrar.hpp>
#include <Configuration/ConfigurationDatabase.hpp>
#include <Configuration/IniParserMixin.hpp>
#include <Configuration/EngineService.hpp>
#include <Platform/MemoryMappedFile.hpp>
#include <Platform/ContentProvider.hpp>
#include <Scripting/ApiRegistrar.hpp>
#include <Scheduling/JobFiber.hpp>
#include <Utility/Result.hpp>
#include <Core/Engine.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scheduling;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Platform;
using namespace ::Eldritch2::Logging;
using namespace ::Eldritch2::Core;

namespace Eldritch2 {
namespace Configuration {
namespace {

	static const Utf8Char	configurationFileName[] = "Eldritch2.ini";

}	// anonymous namespace

	EngineService::EngineService( const Engine& engine ) : Core::EngineService( engine.GetServiceBlackboard() ), _allocator( engine.GetAllocator(), "Configuration Service Root Allocator" ), _log( engine.GetLog() ) {}

// ---------------------------------------------------

	Utf8Literal EngineService::GetName() const {
		return "Configuration Service";
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( JobFiber& executor, const BeginInitializationVisitor ) {
		const Result<MemoryMappedFile>	getConfigurationFileResult( LocateService<ContentProvider>().OpenMemoryMappedFile( WellKnownDirectory::UserDocuments, configurationFileName ) );
		if( !getConfigurationFileResult ) {
			_log( MessageSeverity::Error, "Error opening configuration file '{}': {}", configurationFileName, GetErrorString( getConfigurationFileResult ) );
			return;
		}

		Engine&	engine( LocateService<Engine>() );

		IniParserMixin<ConfigurationDatabase>	database( [&] ( const Utf8Char* const sectionName, const Utf8Char* const valueName, const Range<const Utf8Char*>& /*value*/ ) {
			_log( MessageSeverity::Error, "Unknown configuration key '[{}] {}'." ET_UTF8_NEWLINE_LITERAL, sectionName, valueName );
		}, _allocator );

	//	Haul the whole file into memory.
		getConfigurationFileResult->PrefetchRange( 0u, getConfigurationFileResult->GetMappedRegionSizeInBytes() );

	//	While we're waiting for the disk to do its thing, register all the variables from the game engine we're attached to.
		engine.VisitServices( ConfigurationRegistrar( database ) );

		database.ApplyIni( getConfigurationFileResult->TryGetStructureArrayAtOffset<const Utf8Char>( 0u, getConfigurationFileResult->GetMappedRegionSizeInBytes() ) );

		_log( MessageSeverity::Error, "Configuration loaded successfully." ET_UTF8_NEWLINE_LITERAL );

		engine.VisitServices( executor, ConfigurationBroadcastVisitor() );
	}

}	// namespace Configuration
}	// namespace Eldritch2