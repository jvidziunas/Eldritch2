/*==================================================================*\
  Engine.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Platform/SynchronousFileAppender.hpp>
#include <Assets/AssetPackageLoaderThread.hpp>
#include <Platform/PlatformInterface.hpp>
#include <Scheduling/ThreadScheduler.hpp>
#include <Platform/ContentProvider.hpp>
#include <Scheduling/JobFiber.hpp>
#include <Core/Engine.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
#include <EASTL/iterator.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::Scheduling;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Platform;
using namespace ::Eldritch2::Logging;
using namespace ::Eldritch2::Assets;

namespace Eldritch2 {
namespace Core {
namespace {

	static ETInlineHint ETPureFunctionHint SynchronousFileAppender CreateLogFile( ContentProvider& provider, const Utf8Char* const fileName ) {
		auto	createAppenderResult( provider.CreateSynchronousFileAppender( WellKnownDirectory::Logs, fileName ) );

		ETRuntimeVerification( createAppenderResult );

		return eastl::move( *createAppenderResult );
	}

// ---------------------------------------------------

	static ETInlineHint UniquePointer<EngineService*[]> MakeHeapCopy( Allocator& allocator, std::initializer_list<std::reference_wrapper<EngineService>> services ) {
		const auto	result( static_cast<EngineService**>( allocator.Allocate( sizeof(EngineService*) * services.size() ) ) );

		eastl::transform( services.begin(), services.end(), result, [] ( std::reference_wrapper<EngineService> service ) { return eastl::addressof( service.get() ); } );

		return UniquePointer<EngineService*[]>( result, { services.size(), allocator } );
	}

// ---------------------------------------------------

	static Result<UniquePointer<UniquePointer<WorldService>[]>> CreateWorldServices( Allocator& allocator, const World& world, UniquePointer<EngineService*[]>& services ) {
		const auto	tempBegin( static_cast<UniquePointer<WorldService>*>(_alloca( sizeof(UniquePointer<WorldService>) * services.GetSize() )) );
		auto		tempEnd( tempBegin );

	//	It would be nice to use eastl::transform for this, but at the same time we would like to support early-out. Can we instead use a stateful predicate?
		for( EngineService* service : services ) {
			auto	allocateServiceResult( service->CreateWorldService( world.GetAllocator(), world ) );

			if( !allocateServiceResult ) {
			//	Ensure we destroy any created services so we don't leak memory.
				eastl::destruct( tempBegin, tempEnd );
				return allocateServiceResult.GetErrorCode();
			}

			new( tempEnd++ ) UniquePointer<WorldService>( eastl::move( *allocateServiceResult ) );
		}

	//	eastl::remove_if doesn't support move semantics yet, so we need to rely on the std implementation.
		const auto		copyEnd( std::remove_if( tempBegin, tempEnd, [] ( UniquePointer<WorldService>& service ) { return !service; } ) );
		const auto		count( static_cast<size_t>(eastl::distance( tempBegin, copyEnd )) );
		const auto		result( static_cast<UniquePointer<WorldService>*>( allocator.Allocate( sizeof(UniquePointer<WorldService>) * count ) ) );
		if( result ) {
			eastl::uninitialized_move( tempBegin, copyEnd, result );
		}
		
	//	Ensure we destroy any created services so we don't leak memory.
		eastl::destruct( tempBegin, tempEnd );

		if( !result ) {
			return Error::OutOfMemory;
		}

		return UniquePointer<UniquePointer<WorldService>[]>( result, { count, allocator } );
	}

// ---------------------------------------------------

	static UniquePointer<AssetPackageLoader> CreatePackageLoader( Allocator& allocator, ContentProvider& contentProvider ) {
		return MakeUnique<AssetPackageLoaderThread>( allocator, contentProvider, allocator );
	}

}	// anonymous namespace

	Engine::Engine( PlatformInterface& platformInterface, ThreadScheduler& scheduler, Allocator& allocator ) : _allocator( allocator, "Game Engine Allocator" ),
																											   _contentProvider( _allocator ),
																											   _log( CreateLogFile( _contentProvider, "EngineLog.txt" ) ),
																											   _assetLibrary( _contentProvider, CreatePackageLoader( _allocator, _contentProvider ), _allocator ),
																											   _worldDefaultProperties( { _allocator, "Game Engine Default World Properties Bucket Allocator" } ),
																											   _worlds( { _allocator, "Game Engine Ticking World List Allocator" } ),
																											   _hasShutDown( false ),
																											   _hasInitialized( false ),
																											   _blackboard( _allocator ) {
	//	Create the bootstrap set of services.
		_blackboard.Publish<Platform::ContentProvider>( _contentProvider );
		_blackboard.Publish<Assets::AssetLibrary>( _assetLibrary );
		_blackboard.Publish<Scheduling::ThreadScheduler>( scheduler );
		_blackboard.Publish<Platform::PlatformInterface>( platformInterface );
		_blackboard.Publish<Core::Engine>( *this );
	}

// ---------------------------------------------------

	ErrorCode Engine::CreateWorld( std::initializer_list<Pair<const Utf8Char*, const Utf8Char*>> customProperties ) {
		MICROPROFILE_SCOPEI( "Game engine", "Create New World", 0xAAAAAA );

		UniquePointer<World>	world( MakeUnique<World>( _allocator, *this ) );
		if( !world ) {
			return Error::OutOfMemory;
		}

		auto	createServicesResult( CreateWorldServices( _allocator, *world, _services ) );
		if( !createServicesResult ) {
			_log( MessageSeverity::Error, "Unable to create services for world {}: {}!" ET_UTF8_NEWLINE_LITERAL, static_cast<const void*>(world.Get()), GetErrorString( createServicesResult.GetErrorCode() ) );
			return createServicesResult;
		}

	//	Transfer ownership of the new services to the world.
		world->AttachAndInitializeWith( eastl::move( *createServicesResult ) );

	/*	Dispatch the default/shared properties, and then dispatch the custom property set in the arguments. The ordering is
	 *	important here, as we want the custom set to override any default(s) in the event the same property is specified in
	 *	both collections. */
		for( const auto& property : _worldDefaultProperties ) {
			world->SetProperty( property.first, property.second );
		}
		world->SetProperties( customProperties );

		_log( MessageSeverity::Message, "Created world {} successfully!" ET_UTF8_NEWLINE_LITERAL, static_cast<const void*>(world.Get()) );

	//	Finally, add the world to our collection!
		{	ScopedWriteLock	_( _worldMutex );

			_worlds.PushBack( eastl::move( world ) );

		}	// End of lock scope.

		return Error::None;
	}

// ---------------------------------------------------

	void Engine::InitializeWithServices( JobFiber& executor, std::initializer_list<std::reference_wrapper<EngineService>> services ) {
		MICROPROFILE_SCOPEI( "Game engine", "Initialization", 0xAAAAAA );

	//	We should only ever initialize once.
		ETRuntimeAssert( false == _hasInitialized.load( std::memory_order_acquire ) );
		
	//	Allocate a permanent collection of service references, as the initializer_list will only last until the end of scope.
		_services = MakeHeapCopy( _allocator, services );

	//	Broadcast the various initialization visitor objects to each of the attached services.
		VisitServices( executor, EngineService::BeginInitializationVisitor() );
		VisitServices( executor, EngineService::InitializationCompleteVisitor() );

	//	At this point, we are done initializing this Engine and can call the full set of methods.
		_hasInitialized.store( true, std::memory_order_release );
	
		_log( MessageSeverity::Message, "Engine initialization complete!" ET_UTF8_NEWLINE_LITERAL );
	}

// ---------------------------------------------------

	void Engine::EnterMainLoopOnCaller( JobFiber& executor ) {
		while( !HasShutDown() ) {
			MicroProfileFlip( nullptr );

			MICROPROFILE_SCOPEI( "Game engine", "Frame", 0xAAAAAA );

			{	MICROPROFILE_SCOPEI( "Game engine", "Engine Service Tick", 0xEEEEEE );
				VisitServices( executor, EngineService::ServiceTickVisitor() );
			}	// End of profile scope.

			{	MICROPROFILE_SCOPEI( "Game engine", "World Tick", 0xFFFFFF );
				VisitServices( executor, EngineService::WorldTickVisitor() );
			}	// End of profile scope.
		}

		_log( MessageSeverity::Message, "Terminating execution." ET_UTF8_NEWLINE_LITERAL );
	}

// ---------------------------------------------------

	size_t Engine::DestroyShutDownWorlds() {
		MICROPROFILE_SCOPEI( "Game engine", "Garbage collect worlds", 0x3422F3 );

		ScopedWriteLock	_( _worldMutex );

		const auto	initialCount( _worlds.GetSize() );
		const auto	inactiveWorlds( _worlds.RemoveIf( [] ( const UniquePointer<World>& world ) { return world->HasShutDown(); } ) );

		_worlds.Erase( inactiveWorlds, _worlds.End() );

		return initialCount - _worlds.GetSize();
	}

}	// namespace Core
}	// namespace Eldritch2