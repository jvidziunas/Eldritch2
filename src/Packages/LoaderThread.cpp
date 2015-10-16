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
#include <Scheduler/TaskScheduler.hpp>
#include <Packages/ContentPackage.hpp>
#include <Packages/LoaderThread.hpp>
#include <Utility/Result.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;
using namespace ::std;

namespace Eldritch2 {
namespace FileSystem {

	LoaderThread::LoaderThread( TaskScheduler& scheduler, Allocator& allocator ) : _allocator( allocator, UTF8L("Loader Thread Allocator") ),
																				   _loadSemaphore( scheduler.AllocateSemaphore( _allocator, 0u, 128u ).object, { _allocator } ),
																				   _executionBehavior( ExecutionBehavior::CONTINUE ) {}

// ---------------------------------------------------

	LoaderThread::~LoaderThread() {
		auto&	threadAllocator( _allocator );

		_outstandingLoadList.ClearAndDispose( [&threadAllocator] ( PackageDeserializationContext& context ) {
			threadAllocator.Delete( context );
		} );
	}

// ---------------------------------------------------

	void LoaderThread::RequestGracefulShutdown() {
		_executionBehavior.store( ExecutionBehavior::TERMINATE, memory_order_release );

		// Mark the semaphore so the thread knows to wake up.
		if( _loadSemaphore ) {
			_loadSemaphore->IncreaseCount();
		}
	}

// ---------------------------------------------------

	ErrorCode LoaderThread::BeginLoad( ContentPackage& package ) {
		using AllocationOption	= Allocator::AllocationOption;

	// ---

		if( PackageDeserializationContext* context { new(_allocator, AllocationOption::TEMPORARY_ALLOCATION) PackageDeserializationContext( { package } ) } ) {
			_initializationQueue.PushBack( *context );

			// Mark the semaphore so the thread knows to wake up.
			if( _loadSemaphore ) {
				_loadSemaphore->IncreaseCount();
			}

			return Error::NONE;
		}

		return Error::OUT_OF_MEMORY;
	}

// ---------------------------------------------------

	const UTF8Char* const LoaderThread::GetHumanReadableName() const {
		return UTF8L("Package Loader Thread");
	}

// ---------------------------------------------------

	ErrorCode LoaderThread::Run() {
		using ResidencyState = ContentPackage::ResidencyState;

	// ---

		auto&	threadAllocator( _allocator );
		auto&	loadList( _outstandingLoadList );

		if( !_loadSemaphore ) {
			return Error::INVALID_OBJECT_STATE;
		}

		while( (_loadSemaphore->Acquire(), ExecutionBehavior::CONTINUE == _executionBehavior.load( memory_order_acquire )) ) {
			// Initialize any new deserialization, and add to the 
			_initializationQueue.PopAndDisposeFront( [&threadAllocator, &loadList] ( PackageDeserializationContext& context ) {
				if( context.DeserializeDependencies() ) {
					loadList.PushFront( context );
				} else {
					threadAllocator.Delete( context );
				}
			} );

			// This should execute even if the new addition failed initialization; this will help the failure notification propagate out to other queued packages so they can abort.
			loadList.RemoveAndDisposeIf( [] ( const PackageDeserializationContext& context ) {
				bool	dependenciesLoaded( true );

				for( const auto& referencedPackage : context.GetBoundPackage().GetReferencedPackageCollection() ) {
					switch( referencedPackage->GetResidencyState() ) {
						case ResidencyState::FAILED: {
							// We have a load failure. Early out by returning true.
							return true;
						}	// case ResidencyState::FAILED

						case ResidencyState::LOADING: {
							// This dependency is unloaded. Mark this, but continue looping through the other dependencies so we can catch/propagate actual
							// failures instead of silently stopping on the first unloaded dependency we encounter.
							dependenciesLoaded = false;
							continue;
						}	// case ResidencyState::LOADING
					};	// switch( referencedPackage->GetResidencyState() )
				}

				return dependenciesLoaded;
			}, [&threadAllocator] ( PackageDeserializationContext& context ) {
				context.DeserializeContent();
				threadAllocator.Delete( context );
			} );
		}

		return Error::NONE;
	}

}	// namespace FileSystem
}	// namespace Eldritch2