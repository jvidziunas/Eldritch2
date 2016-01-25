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
#include <Configuration/ConfigurationPublishingInitializationVisitor.hpp>
#include <Scripting/ScriptAPIRegistrationInitializationVisitor.hpp>
#include <FileSystem/ReadableMemoryMappedFile.hpp>
#include <FileSystem/SynchronousFileWriter.hpp>
#include <Utility/Memory/ArenaAllocator.hpp>
#include <Configuration/EngineService.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <FileSystem/ContentProvider.hpp>
#include <Configuration/INIParser.hpp>
#include <Foundation/GameEngine.hpp>
#include <Utility/ErrorCode.hpp>
#include <Build.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2;

#define CONFIGURATION_EXTENSION ".cfg"

namespace {

	static const UTF8Char	configurationFileName[]					= UTF8_PROJECT_NAME UTF8L( CONFIGURATION_EXTENSION );
	static const UTF8Char	tempConfigurationFileName[]				= UTF8_PROJECT_NAME UTF8L( "." ) UTF8L( CONFIGURATION_EXTENSION );
	static const UTF8Char	userConfigurationFileFormatString[]		= UTF8L("Profiles") ET_UTF8_DIR_SEPARATOR UTF8L("%s") ET_UTF8_DIR_SEPARATOR UTF8_PROJECT_NAME UTF8L( CONFIGURATION_EXTENSION );
	static const UTF8Char	tempUserConfigurationFileFormatString[]	= UTF8L("Profiles") ET_UTF8_DIR_SEPARATOR UTF8L("%s") ET_UTF8_DIR_SEPARATOR UTF8L(".") UTF8_PROJECT_NAME UTF8L( CONFIGURATION_EXTENSION );

}	// anonymous namespace

namespace Eldritch2 {
namespace Configuration {

	EngineService::EngineService( GameEngine& owningEngine, ContentProvider& contentProvider ) : GameEngineService( owningEngine ), ConfigurationDatabase( GetEngineAllocator() ), _contentProvider( contentProvider ) {}

// ---------------------------------------------------

	const UTF8Char* const EngineService::GetName() const {
		return UTF8L("Configuration Service");
	}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( ScriptAPIRegistrationInitializationVisitor& /*typeRegistrar*/ ) {
		// register DumpConfigurationToFile() here
	}

// ---------------------------------------------------

	void EngineService::AcceptTaskVisitor( WorkerContext& executingContext, WorkerContext::FinishCounter& finishCounter, const InitializeEngineTaskVisitor ) {
		executingContext.Enqueue( finishCounter, { this, [] ( void* service, WorkerContext& executingContext ) {
			{	WorkerContext::FinishCounter	preConfigurationLoadedFinishCounter( 0 );
				static_cast<EngineService*>(service)->BroadcastTaskVisitor( executingContext, preConfigurationLoadedFinishCounter, EngineService::PreConfigurationLoadedTaskVisitor() );

				executingContext.WaitForCounter( preConfigurationLoadedFinishCounter );
			}

			{	WorkerContext::FinishCounter	postConfigurationLoadedFinishCounter( 0 );
				static_cast<EngineService*>(service)->BroadcastTaskVisitor( executingContext, postConfigurationLoadedFinishCounter, EngineService::PostConfigurationLoadedTaskVisitor() );

				executingContext.WaitForCounter( postConfigurationLoadedFinishCounter );
			}
		} } );
	}

// ---------------------------------------------------

	void EngineService::AcceptTaskVisitor( WorkerContext& executingContext, WorkerContext::FinishCounter& finishCounter, const PreConfigurationLoadedTaskVisitor ) {
		executingContext.Enqueue( finishCounter, { this, [] ( void* service, WorkerContext& /*executingContext*/ ) {
			static_cast<EngineService*>(service)->BroadcastConfigurationToEngine();
		} } );
	}

// ---------------------------------------------------

	void EngineService::DumpConfigurationToFile() const {
		using FileOverwriteBehavior	= ContentProvider::FileOverwriteBehavior;
		using KnownContentLocation	= ContentProvider::KnownContentLocation;

	// ---

		// Try to create a file writer for the temporary settings file.
		FixedStackAllocator<64u>	temporaryAllocator( UTF8L("EngineService::DumpConfigurationToFile() Temporary Allocator") );

		if( const auto getWriterResult = _contentProvider.CreateSynchronousFileWriter( temporaryAllocator, KnownContentLocation::UserDocuments, tempConfigurationFileName, FileOverwriteBehavior::OverwriteIfFileExists ) ) {
			// Dump configuration

			// Commit the temporary settings file by copying the temporary one over the actual.
			_contentProvider.CopyFreeFile( KnownContentLocation::UserDocuments, configurationFileName, tempConfigurationFileName, FileOverwriteBehavior::OverwriteIfFileExists );
			// Destroy the file writer...
			temporaryAllocator.Delete( *getWriterResult.object );
			// ... then destroy the temporary file since we no longer need it.
			_contentProvider.DeleteFreeFile( KnownContentLocation::UserDocuments, tempConfigurationFileName );
		}
	}

// ---------------------------------------------------

	void EngineService::BroadcastConfigurationToEngine() {
		FixedStackAllocator<64u>	tempAllocator( UTF8L("EngineService::BroadcastConfigurationToEngine() Temporary Allocator") );

		GetLogger()( UTF8L("Loading configuration from file '%s'.") ET_UTF8_NEWLINE_LITERAL, configurationFileName );

		if( const auto getMappedFileResult = _contentProvider.CreateReadableMemoryMappedFile( tempAllocator, ContentProvider::KnownContentLocation::UserDocuments, configurationFileName ) ) {
			ConfigurationDatabase&							database( *this );
			ConfigurationPublishingInitializationVisitor	visitor( database );
			ReadableMemoryMappedFile&						mappedFile( *getMappedFileResult.object );

			// Haul the whole file into memory.
			mappedFile.PrefetchRangeForRead( 0u, mappedFile.GetAccessibleRegionSizeInBytes() );
			// While we're waiting for the disk to do its thing, register all the variables from the game engine we're attached to.
			BroadcastInitializationVisitor( visitor );

			ParseINI( mappedFile.TryGetStructureArrayAtOffset<UTF8Char>( 0u, mappedFile.GetAccessibleRegionSizeInBytes() ), [&database] ( const Range<const UTF8Char*>& section, const Range<const UTF8Char*>& name, const Range<const UTF8Char*>& value ) {
				database.SetValue( section, name, value );
			} );

			GetLogger()( UTF8L("Configuration loaded successfully.") ET_UTF8_NEWLINE_LITERAL );

			tempAllocator.Delete( *getMappedFileResult.object );
		} else {
			GetLogger( LogMessageType::Error )( UTF8L("Error reading configuration file: %s"), getMappedFileResult.resultCode.ToUTF8String() );
		}
	}

}	// namespace Configuration
}	// namespace Eldritch2