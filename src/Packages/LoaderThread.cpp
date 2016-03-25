/*==================================================================*\
  LoaderThread.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Concurrency/UserSemaphore.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Scheduler/ThreadScheduler.hpp>
#include <Utility/Concurrency/Lock.hpp>
#include <Packages/ContentPackage.hpp>
#include <Packages/LoaderThread.hpp>
#include <Utility/Result.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace FileSystem {

	LoaderThread::LoaderThread( ThreadScheduler& scheduler, Allocator& allocator ) : _initializationQueue( { allocator, UTF8L("Loader Thread Initialization Queue Allocator") } ),
																					 _outstandingLoadList( { allocator, UTF8L("Loader Thread Outstanding Load List") } ),
																					 _loadSemaphore( scheduler.AllocateSemaphore( allocator, 0u, 128u ).object, { allocator } ),
																					 _initializationMutex( scheduler.AllocateUserMutex( allocator ).object, { allocator } ),
																					 _executionBehavior( ExecutionBehavior::Continue ) {}

// ---------------------------------------------------

	LoaderThread::~LoaderThread() {}

// ---------------------------------------------------

	void LoaderThread::RequestGracefulShutdown() {
		_executionBehavior.store( ExecutionBehavior::Terminate, ::std::memory_order_release );

		// Mark the semaphore so the thread knows to wake up.
		if( _loadSemaphore ) {
			_loadSemaphore->IncreaseCount();
		}
	}

// ---------------------------------------------------

	ErrorCode LoaderThread::BeginLoad( ContentPackage& package ) {
		{	ScopedLock	_( *_initializationMutex );
			_initializationQueue.PushBack( { package } );
		}	// end of lock scope

		// Mark the semaphore so the thread knows to wake up.
		if( _loadSemaphore ) {
			_loadSemaphore->IncreaseCount();
		}

		return Error::None;
	}

// ---------------------------------------------------

	const UTF8Char* const LoaderThread::GetHumanReadableName() const {
		return UTF8L("Package Loader Thread");
	}

// ---------------------------------------------------

	void LoaderThread::Run() {
		using ResidencyState = ContentPackage::ResidencyState;

	// ---

		if( !_loadSemaphore ) {
			return;
		}

		while( (_loadSemaphore->Acquire(), ExecutionBehavior::Continue == _executionBehavior.load( ::std::memory_order_consume )) ) {
			ObjectHandle<ContentPackage>	newPackage( nullptr );
			// Initialize any new deserialization, and add to the pending list.
			{	ScopedLock	_( *_initializationMutex );
				if( _initializationQueue ) {
					newPackage = _initializationQueue.Back();
					_initializationQueue.PopBack();
				}
			}

			if( newPackage != nullptr ) {
				MICROPROFILE_SCOPEI( "Content Load", "Initialize load operation", 0xFFFFFF );

				if( newPackage->DeserializeDependencies() ) {
					_outstandingLoadList.EmplaceBack( ::std::move( newPackage ) );
				}
			}

			// This should execute even if the new addition failed initialization; this will help the failure notification propagate out to other queued packages so they can abort.
			const auto pendingCompletes( _outstandingLoadList.RemoveIf( [] ( const ObjectHandle<ContentPackage>& package ) {
				bool	dependenciesLoaded( true );

				for( const auto& dependency : package->GetDependencies() ) {
					switch( dependency->GetResidencyState() ) {
						case ResidencyState::Failed: {
							// We have a load failure. Early out by returning true.
							return true;
						}	// case ResidencyState::Failed

						case ResidencyState::Loading: {
							// This dependency is unloaded. Mark this, but continue looping through the other dependencies so we can catch/propagate actual
							// failures instead of silently stopping on the first unloaded dependency we encounter.
							dependenciesLoaded = false;
							continue;
						}	// case ResidencyState::Loading
					};	// switch( referencedPackage->GetResidencyState() )
				}

				return dependenciesLoaded;
			} ) );

			_outstandingLoadList.EraseAndDispose( pendingCompletes, _outstandingLoadList.End(), [] ( ObjectHandle<ContentPackage>& package ) {
				MICROPROFILE_SCOPEI( "Content Load", "Deserialize content", 0xCCCCFF );

				package->DeserializeContent();
			} );
		}
	}

}	// namespace FileSystem
}	// namespace Eldritch2