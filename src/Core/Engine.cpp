/*==================================================================*\
  Engine.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduling/JobExecutor.hpp>
#include <Core/PropertyRegistrar.hpp>
#include <Scheduling/JobSystem.hpp>
#include <Core/IniParser.hpp>
#include <Core/Engine.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Core {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Scripting;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;

namespace {

	enum : size_t {
		HighParallelismSplit	= 1u,
		LowParallelismSplit		= 2u
	};

// ---

	ETInlineHint ETPureFunctionHint float32 AsMilliseconds( uint64 microseconds ) {
		static constexpr float32	MicrosecondsPerMillisecond = 1000.0f;

		return microseconds / MicrosecondsPerMillisecond;
	}

}	// anonymous namespace

	Engine::Engine(
		JobSystem& scheduler
	) : _allocator( "Game Engine Allocator" ),
		_services(),
		_packageProvider(),
		_log(),
		_shouldRun( true ),
		_worlds( MallocAllocator( "Game Engine World List Allocator" ) ),
		_components( MallocAllocator( "Game Engine Component List Allocator" ) ) {
	//	Create the bootstrap set of services.
		_services.Publish<PackageDatabase>( _packageProvider.GetPackageDatabase() );
		_services.Publish<AssetDatabase>( _packageProvider.GetAssetDatabase() );
		_services.Publish<FileSystem>( _packageProvider.GetFileSystem() );
		_services.Publish<JobSystem>( scheduler );
		_services.Publish<Engine>( *this );
	}

// ---------------------------------------------------

	ErrorCode Engine::CreateWorld( JobExecutor& executor ) {
		UniquePointer<World> world( CreateUnique<World>( _allocator, GetBlackboard(), GetLog() ) );

		ET_FAIL_UNLESS( world ? Error::None : Error::OutOfMemory );
		ET_FAIL_UNLESS( world->BindResources( executor, _components.Begin(), _components.End() ) );

	//	Add the world to the global collection.
		{	Lock	_( _worldsMutex );
			_worlds.Append( eastl::move( world ) );
		}	// End of lock scope.

		return Error::None;
	}

// ---------------------------------------------------

	void Engine::TickWorlds( JobExecutor& executor ) {
		{	MICROPROFILE_SCOPEI( "Engine/WorldManagement", "Garbage collect worlds", 0x3422F3 );
			Lock	_( _worldsMutex );

			executor.ForEach<HighParallelismSplit>( _worlds.Begin(), _worlds.End(), [this] ( JobExecutor& executor, UniquePointer<World>& world ) {
				if (world->ShouldShutDown()) {			
				/*	Note that there will (temporarily) be a null pointer in the world collection until these entries are removed at the end
				 *	of the sweep phase-- see below. */
					world->FreeResources( executor );
					world.Reset();
				}
			} );

		//	Remove all null entries from the worlds collection.
			_worlds.EraseIf( [] ( UniquePointer<World>& world ) { return world == nullptr; } );

			if (_worlds.IsEmpty()) {
				_log.Write( MessageType::Message, "No running worlds, shutting down engine." UTF8_NEWLINE );
				return SetShouldShutDown();
			}

		}	// End of profile scope.
		{	MICROPROFILE_SCOPEI( "Engine/WorldManagement", "Tick worlds", 0xF32334 );
			executor.ForEach<HighParallelismSplit>( _worlds.Begin(), _worlds.End(), [this] ( JobExecutor& executor, UniquePointer<World>& world ) {
				world->Tick( executor );
			} );
		}	// End of profile scope.
	}

// ---------------------------------------------------

	ErrorCode Engine::ApplyConfiguration( const Utf8Char* const path ) {
		MappedFile iniFile;
		{	const ErrorCode result( iniFile.Open( MappedFile::Read, _packageProvider.GetFileSystem().GetAbsolutePath( KnownDirectory::UserDocuments, path ).AsCString() ) );
			 if (Failed( result )) {
				_log.Write( MessageType::Warning, "Error opening {}, skipping configuration." UTF8_NEWLINE, path );

				return result;
			}
		}

		iniFile.Prefetch( MappedFile::StartOfFile, MappedFile::EndOfFile );

	//	Give the disk time to page in the configuration data by registering client properties.
		PropertyDatabase properties;
	
		{	PropertyRegistrar registrar( properties );
			for (EngineComponent* component : _components) {
				component->AcceptVisitor( registrar );
			}
		}

		ApplyIni( iniFile.GetRangeAtOffset<const Utf8Char>( MappedFile::StartOfFile, iniFile.GetSizeInBytes() ),
			[&] ( const Utf8Char* const group, const Utf8Char* const name, const Range<const Utf8Char*>& value ) -> bool {
				return properties.SetValue( group, name, value );
			},
			[&] ( const Utf8Char* const group, const Utf8Char* const name, const Range<const Utf8Char*>& /*value*/ ) {
				_log.Write( MessageType::Warning, "Unknown configuration key '[{}] {}'." UTF8_NEWLINE, group, name );
			}
		);

		return Error::None;
	}

// ---------------------------------------------------

	void Engine::SweepPackages( size_t collectionLimit ) {
		_packageProvider.GetPackageDatabase().DestroyGarbage( collectionLimit, _packageProvider.GetAssetDatabase() );
	}

// ---------------------------------------------------

	void Engine::BuildAssetApi() {
	//	Collect all asset view creation event handlers and publish them to the asset library.
		AssetApiBuilder	apiBuilder;

		for (EngineComponent* component : _components) {
			component->AcceptVisitor( apiBuilder );
		}

		_packageProvider.GetAssetDatabase().BindResources( eastl::move( apiBuilder.GetFactories() ) );
	}

// ---------------------------------------------------

	void Engine::ScanPackages() {
		Stopwatch	timer;

		_packageProvider.ScanPackages();

		_log.Write( MessageType::Message, "Rebuilt package listing in {:.2f}ms." UTF8_NEWLINE, AsMilliseconds( timer.GetDuration() ) );
	}

// ---------------------------------------------------

	int Engine::BootOnCaller( JobExecutor& executor ) {
		MicroProfileFlip( nullptr );
		
		if (Failed( _packageProvider.BindResources() )) {
			return -1;
		}
		ET_AT_SCOPE_EXIT( _packageProvider.FreeResources() );

		_packageProvider.GetFileSystem().Move( KnownDirectory::Logs, "EngineLog.old.txt", "EngineLog.txt" );

		_log.BindResources( _packageProvider.GetFileSystem().GetAbsolutePath( KnownDirectory::Logs, "EngineLog.txt" ).AsCString() );
		ET_AT_SCOPE_EXIT( _log.FreeResources() );

		_log.Write( MessageType::Message, "\t======================================================" UTF8_NEWLINE );
		_log.Write( MessageType::Message, "\t| INITIALIZING APPLICATION                           |" UTF8_NEWLINE );
		_log.Write( MessageType::Message, "\t======================================================" UTF8_NEWLINE );

		InitializeComponents( executor );
		CreateBootWorld( executor );

		while (ShouldRun()) {
			MicroProfileFlip( nullptr );
			RunFrame( executor );
		}

		_log.Write( MessageType::Message, "\t======================================================" UTF8_NEWLINE );
		_log.Write( MessageType::Message, "\t| TERMINATING APPLICATION                            |" UTF8_NEWLINE );
		_log.Write( MessageType::Message, "\t======================================================" UTF8_NEWLINE );

		return 0;
	}

// ---------------------------------------------------

	void Engine::InitializeComponents( JobExecutor& executor ) {
		MICROPROFILE_SCOPEI( "Engine/Initialization", "Initialization", 0xAAAAAA );
		Stopwatch	timer;

	//	Complete all initialization steps on each of the attached services.
		executor.ForEach<HighParallelismSplit>( _components.Begin(), _components.End(), [] ( JobExecutor& executor, EngineComponent* component ) {
			component->AcceptVisitor( executor, EngineComponent::InitializationVisitor() );
		} );
		executor.ForEach<HighParallelismSplit>( _components.Begin(), _components.End(), [] ( JobExecutor& executor, EngineComponent* component ) {
			component->AcceptVisitor( executor, EngineComponent::LateInitializationVisitor() );
		} );

		_log.Write( MessageType::Message, "Engine initialization complete in {:.2f}ms." UTF8_NEWLINE, AsMilliseconds( timer.GetDuration() ) );
		_log.Write( MessageType::Message, "\t======================================================" UTF8_NEWLINE );
	}

// ---------------------------------------------------

	void Engine::CreateBootWorld( JobExecutor& executor ) {
		const ErrorCode	result( CreateWorld( executor ) );
		if (Failed( result )) {
			_log.Write( MessageType::Error, "Failed to create boot world: {}!" UTF8_NEWLINE, AsCString( result ) );
		}
	}

// ---------------------------------------------------

	void Engine::RunFrame( JobExecutor& executor ) {
		MICROPROFILE_SCOPEI( "Engine/Frame", "Frame", 0xAAAAAA );

	//	Engine component tick is run in sequence before world tick; we want all shared state to be updated *before* world tick happens.
		{	MICROPROFILE_SCOPEI( "Engine/Frame", "Engine Component Tick", 0xEEEEEE );
			executor.ForEach<LowParallelismSplit>( _components.Begin(), _components.End(), [] ( JobExecutor& executor, EngineComponent* component ) {
				component->AcceptVisitor( executor, EngineComponent::ServiceTickVisitor() );
			} );
		}	// End of profile scope.

		{	MICROPROFILE_SCOPEI( "Engine/Frame", "World Tick", 0xFFFFFF );
			executor.ForEach<LowParallelismSplit>( _components.Begin(), _components.End(), [] ( JobExecutor& executor, EngineComponent* component ) {
				component->AcceptVisitor( executor, EngineComponent::WorldTickVisitor() );
			} );
		}	// End of profile scope.
	}

}	// namespace Core
}	// namespace Eldritch2