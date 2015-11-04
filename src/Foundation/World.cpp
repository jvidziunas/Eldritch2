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
#include <Packages/ContentPackage.hpp>
#include <Foundation/GameEngine.hpp>
#include <Utility/ErrorCode.hpp>
#include <Foundation/World.hpp>
#include <Logging/Logger.hpp>
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

	World::World( ObjectHandle<ContentPackage>&& package, GameEngine& owningEngine ) : _allocator( owningEngine._allocator, UTF8L("World Allocator") ),
																					   _viewAllocator( _allocator, owningEngine._worldViewAllocationHintInBytes, Allocator::AllocationOption::PERMANENT_ALLOCATION, UTF8L("World View Allocator") ),
																					   _owningEngine( owningEngine ),
																					   _package( move( package ) ),
																					   _keyValuePairs( { _allocator, UTF8L( "World Key-Value Pair Table Allocator" ) } ),
																					   _isPaused( 1u ),
																					   _isLoaded( 0u ),
																					   _lastError( Error::NONE ) {
		owningEngine._tickingWorlds.PushBack( *this );

		if( const auto instantiateViewsResult = owningEngine.InstantiateViewsForWorld( _viewAllocator, *this ) ) {
			owningEngine.GetLoggerForMessageType( LogMessageType::MESSAGE )( UTF8L("Loading world '%p'.") ET_UTF8_NEWLINE_LITERAL, static_cast<void*>(this) );
		} else {
			owningEngine.GetLoggerForMessageType( LogMessageType::ERROR )( UTF8L("Error instantiating view objects for world '%p': %s.") ET_UTF8_NEWLINE_LITERAL, static_cast<void*>(this), instantiateViewsResult.ToUTF8String() );
			_lastError = instantiateViewsResult;
		}
	}

// ---------------------------------------------------

	World::~World() {
		_owningEngine->GetLoggerForMessageType( LogMessageType::MESSAGE )( UTF8L("Destroying world '%p'.") ET_UTF8_NEWLINE_LITERAL, static_cast<void*>(this) );
		DeleteViews();
	}

// ---------------------------------------------------

	UTF8String<> World::GetValueForKey( Allocator& resultAllocator, const UTF8Char* const key, const UTF8Char* defaultValue ) {
		return { defaultValue, FindEndOfString( defaultValue ), { resultAllocator, UTF8L("World Key-Value Pair Allocator") } };
	}

// ---------------------------------------------------

	void World::SetKeyValuePair( const UTF8Char* const key, const UTF8Char* const value ) {

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

		// ---------------------------------------------------

			const UTF8Char* const GetHumanReadableName() const override sealed {
				return UTF8L("Tick World Task");
			}

			Task* Execute( WorkerContext& executingContext ) override sealed {
				WorldView::FrameTickTaskVisitor	visitor;

				for( WorldView& currentView : _worldReference->_attachedViews ) {
					currentView.AcceptTaskVisitor( _frameTaskAllocator, executingContext, *this, visitor );
				}

				return nullptr;
			}

			void Finalize( WorkerContext& executingContext ) override {
				if( _worldReference->GetLastError() ) {
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
						_worldReference->_lastError = Error::UNSPECIFIED;
						break;
					}	// case ResidencyState::FAILED
				}	// switch( _worldReference->GetRootPackage().GetResidencyState() )

				return nullptr;
			}

			void Finalize( WorkerContext& executingContext ) override {
				if( _worldReference->GetLastError() ) {
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

	void World::Dispose() {
		_owningEngine->_allocator.Delete( *this );
	}

// ---------------------------------------------------

	void World::DeleteViews() {
		{	// Broadcast a delete message to all the views so they can clean any shared resources.
			const WorldView::DeletionPreparationVisitor	visitor;

			for( WorldView& view : _attachedViews ) {
				view.AcceptViewVisitor( visitor );
			}
		}
		

		{	// Invoke the destructor and deallocate all currently-attached views.
			Allocator&	viewAllocator( _viewAllocator );

			_attachedViews.ClearAndDispose( [&viewAllocator] ( WorldView& view ) {
				viewAllocator.Delete( view );
			} );
		}
	}

}	// namespace Foundation
}	// namespace Eldritch2