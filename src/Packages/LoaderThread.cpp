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

	LoaderThread::LoaderThread( ThreadScheduler& scheduler, Allocator& allocator ) : _allocator( allocator, UTF8L("Loader Thread Allocator") ),
																					 _loadSemaphore( scheduler.AllocateSemaphore( _allocator, 0u, 128u ).object, { _allocator } ),
																					 _executionBehavior( ExecutionBehavior::Continue ) {}

// ---------------------------------------------------

	LoaderThread::~LoaderThread() {
		auto&	threadAllocator( _allocator );

		_outstandingLoadList.ClearAndDispose( [&threadAllocator] ( PackageDeserializationContext& context ) {
			threadAllocator.Delete( context );
		} );
	}

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
		InstancePointer<PackageDeserializationContext> context( new(_allocator, Allocator::AllocationDuration::Temporary) PackageDeserializationContext( { package }, _allocator ), { _allocator } );

		if( nullptr == context ) {
			return Error::OutOfMemory;
		}

		_initializationQueue.PushBack( *context.release() );

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

		auto&	threadAllocator( _allocator );
		auto&	loadList( _outstandingLoadList );

		if( !_loadSemaphore ) {
			return;
		}

		while( (_loadSemaphore->Acquire(), ExecutionBehavior::Continue == _executionBehavior.load( ::std::memory_order_consume )) ) {
			// Initialize any new deserialization, and add to the pending list.
			_initializationQueue.PopFrontAndDispose( [&threadAllocator, &loadList] ( PackageDeserializationContext& context ) {
				MICROPROFILE_SCOPEI("Content Load", "Initialize load operation", 0xFFFFFF );

				if( context.DeserializeDependencies() ) {
					loadList.PushFront( context );
				} else {
					threadAllocator.Delete( context );
				}
			} );

			// This should execute even if the new addition failed initialization; this will help the failure notification propagate out to other queued packages so they can abort.
			loadList.RemoveAndDisposeIf( [] ( const PackageDeserializationContext& context ) {
				bool	dependenciesLoaded( true );

				for( const auto& referencedPackage : context.GetBoundPackage().GetDependencies() ) {
					switch( referencedPackage->GetResidencyState() ) {
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
			}, [&threadAllocator] ( PackageDeserializationContext& context ) {
				MICROPROFILE_SCOPEI( "Content Load", "Deserialize content", 0xCCCCFF );
				context.DeserializeContent();
				threadAllocator.Delete( context );
			} );
		}
	}

}	// namespace FileSystem
}	// namespace Eldritch2