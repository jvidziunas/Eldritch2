/*==================================================================*\
  ConfigurationService.cpp
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
#include <Configuration/ConfigurationService.hpp>
#include <FileSystem/SynchronousFileWriter.hpp>
#include <Utility/Memory/ArenaAllocator.hpp>
#include <Scheduler/CRTPTransientTask.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <FileSystem/ContentProvider.hpp>
#include <Configuration/INIParser.hpp>
#include <Foundation/Application.hpp>
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

	ConfigurationService::ConfigurationService( GameEngine& owningEngine, ContentProvider& contentProvider ) : GameEngineService( owningEngine ), ConfigurationDatabase( GetEngineAllocator() ), _contentProvider( contentProvider ) {}

// ---------------------------------------------------

	const UTF8Char* const ConfigurationService::GetName() const {
		return UTF8L("Configuration Service");
	}

// ---------------------------------------------------

	void ConfigurationService::AcceptInitializationVisitor( ScriptAPIRegistrationInitializationVisitor& /*typeRegistrar*/ ) {
		// register DumpConfigurationToFile() here
	}

// ---------------------------------------------------

	void ConfigurationService::AcceptTaskVisitor( Allocator& subtaskAllocator, Task& initializeEngineTask, WorkerContext& executingContext, const InitializeEngineTaskVisitor ) {
		class PostConfigurationLoadedTask : public CRTPTransientTask<PostConfigurationLoadedTask> {
		// - TYPE PUBLISHING ---------------------------------

		public:
			class PreConfigurationLoadedTask : public Task {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

			public:
				ETInlineHint PreConfigurationLoadedTask( ConfigurationService& host, PostConfigurationLoadedTask& parent, WorkerContext& executingContext, Allocator& subtaskAllocator ) : Task( parent, Scheduler::ContinuationTaskSemantics ),
																																														   _host( host ),
																																														   _subtaskAllocator( subtaskAllocator ) {
					TrySchedulingOnContext( executingContext );
				}

			// ---------------------------------------------------

				ETInlineHint ConfigurationService& GetHost() {
					return _host;
				}

				ETInlineHint Allocator& GetSubtaskAllocator() {
					return _subtaskAllocator;
				}

			// ---------------------------------------------------

				const UTF8Char* const GetHumanReadableName() const override sealed {
					return UTF8L("Perform Pre-Configuration Broadcast Initialization");
				}

				Task* Execute( WorkerContext& executingContext ) override sealed {
					GetHost().BroadcastTaskVisitor( _subtaskAllocator, *this, executingContext, ConfigurationService::PreConfigurationLoadedTaskVisitor() );
					return nullptr;
				}

			// - DATA MEMBERS ------------------------------------

			private:
				ConfigurationService&	_host;
				Allocator&				_subtaskAllocator;
			};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			ETInlineHint PostConfigurationLoadedTask( ConfigurationService& host, Task& initializeEngineTask, WorkerContext& executingContext, Allocator& subtaskAllocator ) : CRTPTransientTask<PostConfigurationLoadedTask>( initializeEngineTask, Scheduler::CodependentTaskSemantics ),
																																											   _preConfigurationLoadedTask( host, *this, executingContext, subtaskAllocator ) {
				TrySchedulingOnContext( executingContext );
			}

		// ---------------------------------------------------

			const UTF8Char* const GetHumanReadableName() const override sealed {
				return UTF8L("Perform Post-Configuration Broadcast Initialization");
			}

			Task* Execute( WorkerContext& executingContext ) override sealed {
				_preConfigurationLoadedTask.GetHost().BroadcastTaskVisitor( _preConfigurationLoadedTask.GetSubtaskAllocator(), *this, executingContext, ConfigurationService::PostConfigurationLoadedTaskVisitor() );
				return nullptr;
			}

		// - DATA MEMBERS ------------------------------------

		private:
			PreConfigurationLoadedTask	_preConfigurationLoadedTask;
		};

	// ---

		new(subtaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) PostConfigurationLoadedTask( *this, initializeEngineTask, executingContext, subtaskAllocator );
	}

// ---------------------------------------------------

	void ConfigurationService::AcceptTaskVisitor( Allocator& subtaskAllocator, Task& preConfigurationLoadInitializationTask, WorkerContext& executingContext, const PreConfigurationLoadedTaskVisitor ) {
		class LoadSerializedConfigurationTask : public CRTPTransientTask<LoadSerializedConfigurationTask> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			// Constructs this LoadSerializedConfigurationTask instance.
			ETInlineHint LoadSerializedConfigurationTask( ConfigurationService& host, Task& preConfigurationLoadInitializationTask, WorkerContext& executingContext ) : CRTPTransientTask<LoadSerializedConfigurationTask>( preConfigurationLoadInitializationTask, Scheduler::CodependentTaskSemantics ),
																																										_host( host ) {
				TrySchedulingOnContext( executingContext );
			}

		// ---------------------------------------------------

			const UTF8Char* const GetHumanReadableName() const override sealed {
				return UTF8L("Load/Broadcast User Configuration");
			}

			Task* Execute( WorkerContext& /*executingContext*/ ) override sealed {
				_host.BroadcastConfigurationToEngine();
				return nullptr;
			}

		// - DATA MEMBERS ------------------------------------

		private:
			ConfigurationService&	_host;
		};

	// ---

		new(subtaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) LoadSerializedConfigurationTask( *this, preConfigurationLoadInitializationTask, executingContext );
	}

// ---------------------------------------------------

	void ConfigurationService::DumpConfigurationToFile() const {
		using FileOverwriteBehavior	= ContentProvider::FileOverwriteBehavior;
		using KnownContentLocation	= ContentProvider::KnownContentLocation;

	// ---

		// Try to create a file writer for the temporary settings file.
		FixedStackAllocator<64u>	temporaryAllocator( UTF8L("ConfigurationService::DumpConfigurationToFile() Temporary Allocator") );

		if( const auto getWriterResult = _contentProvider.CreateSynchronousFileWriter( temporaryAllocator, KnownContentLocation::USER_DOCUMENTS, tempConfigurationFileName, FileOverwriteBehavior::OVERWRITE_IF_FILE_EXISTS ) ) {
			// Dump configuration

			// Commit the temporary settings file by copying the temporary one over the actual.
			_contentProvider.CopyFreeFile( KnownContentLocation::USER_DOCUMENTS, configurationFileName, tempConfigurationFileName, FileOverwriteBehavior::OVERWRITE_IF_FILE_EXISTS );
			// Destroy the file writer...
			temporaryAllocator.Delete( *getWriterResult.object );
			// ... then destroy the temporary file since we no longer need it.
			_contentProvider.DeleteFreeFile( KnownContentLocation::USER_DOCUMENTS, tempConfigurationFileName );
		}
	}

// ---------------------------------------------------

	void ConfigurationService::BroadcastConfigurationToEngine() {
		FixedStackAllocator<64u>	tempAllocator( UTF8L("ConfigurationService::BroadcastConfigurationToEngine() Temporary Allocator") );

		GetLogger()( UTF8L("Loading configuration from file '%s'.") ET_UTF8_NEWLINE_LITERAL, configurationFileName );

		if( const auto getMappedFileResult = _contentProvider.CreateReadableMemoryMappedFile( tempAllocator, ContentProvider::KnownContentLocation::USER_DOCUMENTS, configurationFileName ) ) {
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
			GetLogger( LogMessageType::ERROR )( UTF8L("Error reading configuration file: %s"), getMappedFileResult.resultCode.ToUTF8String() );
		}
	}

}	// namespace Configuration
}	// namespace Eldritch2