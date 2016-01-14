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
#include <Scheduler/CRTPTransientTask.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Scheduler/TaskScheduler.hpp>
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

	World::World( GameEngine& owningEngine ) : _allocator( owningEngine.GetAllocator(), owningEngine._worldArenaSizeInBytes, Allocator::AllocationOption::PERMANENT_ALLOCATION, UTF8L("World Allocator") ),
											   _allocationCheckpoint( _allocator.CreateCheckpoint() ),
											   _owningEngine( owningEngine ),
											   _package( nullptr ),
											   _propertyMutex( owningEngine.GetTaskScheduler().AllocateReaderWriterUserMutex( _allocator ).object, { _allocator } ),
											   _properties( { _allocator, UTF8L("World Key-Value Pair Table Allocator") } ),
											   _isPaused( 1u ),
											   _isLoaded( 0u ),
											   _fatalErrorCount( 0u ) {
		if( nullptr == _propertyMutex ) {
			owningEngine.GetLoggerForMessageType( LogMessageType::ERROR )( UTF8L("Error allocating mutex for world '%p': %s.") ET_UTF8_NEWLINE_LITERAL, static_cast<void*>(this), ErrorCode( Error::OUT_OF_MEMORY ).ToUTF8String() );
			RaiseFatalError();
		}

		if( !EncounteredFatalError() ) {
			owningEngine._tickingWorlds.PushBack( *this );
		}	
	}

// ---------------------------------------------------

	World::~World() {
		_owningEngine->GetLoggerForMessageType( LogMessageType::MESSAGE )( UTF8L("Destroying world '%p'.") ET_UTF8_NEWLINE_LITERAL, static_cast<void*>(this) );
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
			return Error::INVALID_OBJECT_STATE;
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

	void World::QueueUpdateTasks( Allocator& frameTaskAllocator, WorkerContext& executingContext, Task& frameWorldUpdatesCompleteTask ) {
		class TickWorldTask : public CRTPTransientTask<TickWorldTask> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------
		
		public:
			//!	Constructs this @ref TickWorldTask instance.
			ETInlineHint TickWorldTask( World& world, WorkerContext& executingContext, Task& updatesCompleteTask, Allocator& frameTaskAllocator ) : CRTPTransientTask<TickWorldTask>( updatesCompleteTask, Scheduler::CodependentTaskSemantics ),
																																					_worldReference( world ),
																																					_frameTaskAllocator( frameTaskAllocator ) {
				TrySchedulingOnContext( executingContext );
			}

			~TickWorldTask() = default;

		// ---------------------------------------------------

			const UTF8Char* const GetHumanReadableName() const override sealed {
				return UTF8L("Tick World Task");
			}

			Task* Execute( WorkerContext& executingContext ) override sealed {
				for( WorldView& currentView : _worldReference->_attachedViews ) {
					currentView.AcceptTaskVisitor( _frameTaskAllocator, executingContext, *this, WorldView::FrameTickTaskVisitor() );
				}

				return nullptr;
			}

			void Finalize( WorkerContext& executingContext ) override {
				if( !_worldReference->EncounteredFatalError() ) {
					_worldReference->_owningEngine->_tickingWorlds.PushBack( *_worldReference.Release() );
				}

				CRTPTransientTask<TickWorldTask>::Finalize( executingContext );
			}

		// - DATA MEMBERS ------------------------------------

		private:
			// This handle ensures the world will not be deleted until the task has finished executing.
			ObjectHandle<World>	_worldReference;
			Allocator&			_frameTaskAllocator;
		};

	// ---

		class TryFinalizeLoadTask : public CRTPTransientTask<TryFinalizeLoadTask> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------
		public:
			//!	Constructs this @ref FinalizeLoadTask instance.
			ETInlineHint TryFinalizeLoadTask( World& world, WorkerContext& executingContext, Task& updatesCompleteTask ) : CRTPTransientTask<TryFinalizeLoadTask>( updatesCompleteTask, Scheduler::CodependentTaskSemantics ),
																														   _worldReference( world ) {
				TrySchedulingOnContext( executingContext );
			}

		// ---------------------------------------------------

			const UTF8Char* const GetHumanReadableName() const override sealed {
				return UTF8L("Try Finalize World Load Task");
			}

			Task* Execute( WorkerContext& /*executingContext*/ ) override sealed {
				using ResidencyState = ContentPackage::ResidencyState;

			// ---

				if( _worldReference->_package ) {
					switch( _worldReference->GetRootPackage().GetResidencyState() ) {
						case ResidencyState::PUBLISHED: {
							const WorldView::LoadFinalizationVisitor	visitor;

							for( WorldView& currentView : _worldReference->_attachedViews ) {
								currentView.AcceptViewVisitor( visitor );
							}

							_worldReference->_isLoaded = 1u;
							break;
						}	// case ResidencyState::PUBLISHED

						case ResidencyState::FAILED: {
							_worldReference->RaiseFatalError();
							break;
						}	// case ResidencyState::FAILED
					}	// switch( _worldReference->GetRootPackage().GetResidencyState() )
				} else {
					_worldReference->RaiseFatalError();
				}

				return nullptr;
			}

			void Finalize( WorkerContext& executingContext ) override {
				if( !_worldReference->EncounteredFatalError() ) {
					_worldReference->_owningEngine->_tickingWorlds.PushBack( *_worldReference.Release() );
				}

				CRTPTransientTask<TryFinalizeLoadTask>::Finalize( executingContext );
			}

		// - DATA MEMBERS ------------------------------------

		private:
			// This handle ensures the world will not be deleted until the task has finished executing.
			ObjectHandle<World>	_worldReference;
		};

	// ---

		if( IsLoaded() ) {
			new(frameTaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) TickWorldTask( *this, executingContext, frameWorldUpdatesCompleteTask, frameTaskAllocator );
		} else {
			new(frameTaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) TryFinalizeLoadTask( *this, executingContext, frameWorldUpdatesCompleteTask );
		}
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
		{	// Broadcast a delete message to all the views so they can clean any shared resources.
			const WorldView::DeletionPreparationVisitor	visitor;

			for( WorldView& view : _attachedViews ) {
				view.AcceptViewVisitor( visitor );
			}
		}
		

		{	// Invoke the destructor and deallocate all currently-attached views.
			auto&	allocator( _allocator );

			_attachedViews.ClearAndDispose( [&allocator] ( WorldView& view ) {
				allocator.Delete( view );
			} );

			allocator.RestoreCheckpoint( _allocationCheckpoint );
		}
	}

}	// namespace Foundation
}	// namespace Eldritch2