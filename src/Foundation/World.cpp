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
#include <Packages/ContentPackage.hpp>
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
using namespace ::std;

namespace Eldritch2 {
namespace Foundation {

	World::World( GameEngine& owningEngine ) : _allocator( owningEngine.GetAllocator(), owningEngine._worldArenaSizeInBytes, Allocator::AllocationDuration::Normal, UTF8L("World Allocator") ),
											   _allocationCheckpoint( _allocator.CreateCheckpoint() ),
											   _owningEngine( owningEngine ),
											   _package( nullptr ),
											   _propertyMutex( owningEngine.GetThreadScheduler().AllocateReaderWriterUserMutex( _allocator ).object, { _allocator } ),
											   _properties( { _allocator, UTF8L("World Key-Value Pair Table Allocator") } ),
											   _isPaused( 1u ),
											   _isLoaded( 0u ),
											   _fatalErrorCount( 0u ) {
		if( nullptr == _propertyMutex ) {
			owningEngine.GetLoggerForMessageType( LogMessageType::Error )( UTF8L("Error allocating mutex for world '%p': %s.") ET_UTF8_NEWLINE_LITERAL, static_cast<void*>(this), ErrorCode( Error::OutOfMemory ).ToUTF8String() );
			RaiseFatalError();
		}

		if( !EncounteredFatalError() ) {
			owningEngine._tickingWorlds.PushBack( *this );
		}	
	}

// ---------------------------------------------------

	World::~World() {
		_owningEngine->GetLoggerForMessageType( LogMessageType::Message )( UTF8L("Destroying world '%p'.") ET_UTF8_NEWLINE_LITERAL, static_cast<void*>(this) );
		Reset();
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
				_properties.Insert( { move( key ), { rawValue, FindEndOfString( rawValue ), { _allocator, UTF8L("World Property String Allocator") } } } );
			}
		}	// End of lock scope.
	}

// ---------------------------------------------------

	ErrorCode World::BeginContentLoad() {
		FixedStackAllocator<96u>	temp( UTF8L("World::BeginContentLoad() Temporary Allocator") );

		// Calling this function multiple at multiple points doesn't constitute a fatal error per se, but we don't want to do this more than once.
		if( (nullptr != _package) || EncounteredFatalError() ) {
			return Error::InvalidObjectState;
		}

		const auto	resourceName( GetPropertyByKey( temp, GetMainPackageKey(), UTF8L("") ) );
		auto		initiateLoadResult( _owningEngine->GetContentLibrary().ResolvePackageByName( resourceName.GetCharacterArray() ) );

		if( initiateLoadResult ) {
			_package = ::std::move( initiateLoadResult.object );
		} else {
			RaiseFatalError();
		}
		
		return initiateLoadResult.resultCode;
	}

// ---------------------------------------------------

	void World::QueueUpdateTasks( WorkerContext& executingContext, WorkerContext::FinishCounter& finishCounter ) {
		using ResidencyState = ContentPackage::ResidencyState;

	// ---

		auto	TickWorldTask( [] ( void* world, WorkerContext& executingContext ) {
			// This handle ensures the world will not be deleted until the task has finished executing.
			ObjectHandle<World>	worldReference( static_cast<World*>(world), ::Eldritch2::PassthroughReferenceCountingSemantics );

			{	WorkerContext::FinishCounter	tickFinishCounter( 0 );
				for( auto& currentView : worldReference->_attachedViews ) {
					currentView.AcceptTaskVisitor( executingContext, tickFinishCounter, WorldView::FrameTickTaskVisitor() );
				}

				executingContext.WaitForCounter( tickFinishCounter );
			}

			if( !worldReference->EncounteredFatalError() ) {
				auto&	worldTemp( *worldReference.Release() );

				worldTemp._owningEngine->_tickingWorlds.PushBack( worldTemp );
			}
		} );

	// ---

		auto	TryFinalizeLoadTask( [] ( void* world, WorkerContext& /*executingContext*/ ) {
			// This handle ensures the world will not be deleted until the task has finished executing.
			ObjectHandle<World>	worldReference( static_cast<World*>(world), ::Eldritch2::PassthroughReferenceCountingSemantics );

			if( worldReference->_package ) {
				switch( worldReference->GetRootPackage().GetResidencyState() ) {
					case ResidencyState::Published: {
						const WorldView::LoadFinalizationVisitor	visitor;

						for( WorldView& currentView : worldReference->_attachedViews ) {
							currentView.AcceptViewVisitor( visitor );
						}

						worldReference->_isLoaded = 1u;
						break;
					}	// case ResidencyState::Published

					case ResidencyState::Failed: {
						worldReference->RaiseFatalError();
						break;
					}	// case ResidencyState::Failed
				}	// switch( _worldReference->GetRootPackage().GetResidencyState() )
			} else {
				worldReference->RaiseFatalError();
			}

			if( !worldReference->EncounteredFatalError() ) {
				auto&	worldTemp( *worldReference.Release() );

				worldTemp._owningEngine->_tickingWorlds.PushBack( worldTemp );
			}
		} );

	// ---

		executingContext.Enqueue( finishCounter, { this, (IsLoaded() ? static_cast<WorkerContext::WorkFunction>(TickWorldTask) : static_cast<WorkerContext::WorkFunction>(TryFinalizeLoadTask)) } );
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

	ETNoAliasHint const ::Eldritch2::UTF8Char* const World::GetMainPackageKey() {
		return UTF8L("PackageName");
	}

// ---------------------------------------------------

	void World::Dispose() {
		_owningEngine->_allocator.Delete( *this );
	}

// ---------------------------------------------------

	void World::Reset() {
		// Broadcast a delete message to all the views so they can clean any shared resources.
		{	const WorldView::DeletionPreparationVisitor	visitor;

			for( WorldView& view : _attachedViews ) {
				view.AcceptViewVisitor( visitor );
			}
		}
		

		// Invoke the destructor and deallocate all currently-attached views.
		{	auto&	allocator( _allocator );

			_attachedViews.ClearAndDispose( [&allocator] ( WorldView& view ) {
				allocator.Delete( view );
			} );

			allocator.RestoreCheckpoint( _allocationCheckpoint );
		}
	}

}	// namespace Foundation
}	// namespace Eldritch2