/*==================================================================*\
  World.cpp
  ------------------------------------------------------------------
  Purpose:
  Encapsulates all functionality and bookkeeping associated with
  running a game 'world'-- a global environment that contains a
  number of entities and their respective collections of constituent
  components.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/InstanceNew.hpp>
#include <Scheduler/ThreadScheduler.hpp>
#include <Foundation/GameEngine.hpp>
#include <Utility/ErrorCode.hpp>
#include <Foundation/World.hpp>
#include <Logging/Logger.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Foundation {

	World::World( GameEngine& owningEngine, const UTF8Char* const resourceName ) : _allocator( owningEngine.GetAllocator(), owningEngine.GetWorldArenaSizeInBytes(), Allocator::AllocationDuration::Normal, UTF8L("World Allocator") ),
																				   _allocationCheckpoint( _allocator.CreateCheckpoint() ),
																				   _engine( owningEngine ),
																				   _contentLibrary( owningEngine.GetContentLibrary() ),
																				   _package( nullptr ),
																				   _propertyMutex( owningEngine.GetThreadScheduler().AllocateReaderWriterUserMutex( _allocator ).object, { _allocator } ),
																				   _properties( { _allocator, UTF8L("World Key-Value Pair Table Allocator") } ),
																				   _isPaused( 1u ),
																				   _isLoaded( 0u ),
																				   _executeFlag( true ),
																				   _managementView( *this ) {
		owningEngine.NotifyOfNewWorld( *this );

		if( nullptr == _propertyMutex ) {
			owningEngine.GetLoggerForMessageType( LogMessageType::Error )( UTF8L("Error allocating mutex for world '{}': {}.") ET_UTF8_NEWLINE_LITERAL, static_cast<void*>(this), ErrorCode( Error::OutOfMemory ).ToUTF8String() );
			RaiseFatalError();
		}

		if( CanExecute() ) {
			SetProperty( GetMainPackageKey(), resourceName );

			if( auto initiateLoadResult = _contentLibrary.ResolvePackageByName( resourceName ) ) {
				_package = ::std::move( initiateLoadResult.object );
			} else {
				RaiseFatalError();
			}
		}
	}

// ---------------------------------------------------

	World::~World() {
		// Broadcast a delete message to all the views so they can clean any shared resources.
		_managementView.PrepareForWorldShutdown();

		_attachedViews.Erase( _attachedViews.IteratorTo( _managementView ) );

		// Invoke the destructor and deallocate all currently-attached views.
		{	auto&	allocator( _allocator );

			_attachedViews.ClearAndDispose( [&allocator] ( WorldView& view ) {
				allocator.Delete( view );
			} );

			allocator.RestoreCheckpoint( _allocationCheckpoint );
		}

		_engine.NotifyOfWorldDestruction( *this );
	}

// ---------------------------------------------------

	World::Property World::GetPropertyByKey( Allocator& resultAllocator, const UTF8Char* const rawKey, const UTF8Char* const defaultValue ) const {
		static const UTF8Char	returnedStringAllocatorName[] = UTF8L("World Property String Allocator");

		ETRuntimeAssert( nullptr != rawKey );
		ETRuntimeAssert( nullptr != defaultValue );

	// ---

		{	ScopedReaderLock	_( *_propertyMutex );
			auto				pairCandidate( _properties.Find( rawKey ) );

			if( pairCandidate != _properties.End() ) {
				return { pairCandidate->second, { resultAllocator, returnedStringAllocatorName } };
			}
		}	// End of lock scope.

		return { defaultValue, FindEndOfString( defaultValue ), { resultAllocator, returnedStringAllocatorName } };
	}

// ---------------------------------------------------

	void World::SetProperty( const UTF8Char* const rawKey, const UTF8Char* const rawValue ) {
		// Can't be const, resources may be moved out of the container.
		PropertyKey	key( rawKey, FindEndOfString( rawKey ), { _allocator, UTF8L("World Key String Allocator") } );

		{	ScopedLock	_( *_propertyMutex );
			auto		pairCandidate( _properties.Find( key ) );

			if( pairCandidate != _properties.End() ) {
				pairCandidate->second.Assign( rawValue );
			} else {
				_properties.Insert( { ::std::move( key ), { rawValue, FindEndOfString( rawValue ), { _allocator, UTF8L("World Property String Allocator") } } } );
			}
		}	// End of lock scope.
	}

// ---------------------------------------------------

	void World::QueueUpdateTasks( WorkerContext& executingContext, WorkerContext::FinishCounter& finishCounter ) {
		// Add a reference that will be released by the update functions below when they complete/exit scope.
		AddReference();

		if( IsLoaded() ) {
			executingContext.Enqueue( finishCounter, { this, [] ( void* world, WorkerContext& executingContext ) {
				// This handle ensures the world will not be deleted until the task has finished executing.
				ObjectHandle<World>	worldReference( *static_cast<World*>(world), ::Eldritch2::PassthroughReferenceCountingSemantics );

				static_cast<World*>(world)->_managementView.ExecuteFrame( executingContext );
			} } );

			return;
		}

		executingContext.Enqueue( finishCounter, { this, [] ( void* world, WorkerContext& executingContext ) {
			// This handle ensures the world will not be deleted until the task has finished executing.
			ObjectHandle<World>	worldReference( *static_cast<World*>(world), ::Eldritch2::PassthroughReferenceCountingSemantics );

			static_cast<World*>(world)->_managementView.TryFinalizeLoad( executingContext );
		} } );
	}

// ---------------------------------------------------

	Logger& World::GetLogger( LogMessageType type ) const {
		return _engine.GetLoggerForMessageType( type );
	}

// ---------------------------------------------------

	bool World::IsLoaded() const {
		return 0u != _isLoaded;
	}

// ---------------------------------------------------

	bool World::IsPaused() const {
		return 0u != _isPaused;
	}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const World::GetMainPackageKey() {
		return UTF8L("PackageName");
	}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const World::GetRulesKey() {
		return UTF8L("Rules");
	}

// ---------------------------------------------------

	void World::Dispose() {
		_engine.GetAllocator().Delete( *this );
	}

}	// namespace Foundation
}	// namespace Eldritch2