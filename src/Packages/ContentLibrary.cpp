/*==================================================================*\
  ContentLibrary.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/DisposingResultPair.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Utility/Concurrency/Lock.hpp>
#include <Packages/ContentLibrary.hpp>
#include <Packages/ContentPackage.hpp>
#include <Scheduler/TaskScheduler.hpp>
#include <Packages/LoaderThread.hpp>
#include <Utility/ResultPair.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;
using namespace ::std;

namespace Eldritch2 {
namespace FileSystem {

	bool ContentLibrary::ResourceViewFactoryKey::operator==( const ResourceViewFactoryKey& right ) const {
		const auto	size( second - first );

		return (size == (right.second - right.first)) && CompareMemory( first, right.first, size );
	}

// ---------------------------------------------------

	ContentLibrary::ContentLibrary( ContentProvider& contentProvider, TaskScheduler& scheduler, Allocator& allocator ) : _allocator( allocator, UTF8L("Content Library Package Data Allocator") ),
																														 _deserializationContextAllocator( allocator, UTF8L("Content Library Package Data Allocator") ),
																														 _contentProvider( contentProvider ),
																														 _contentPackageLibraryMutex( scheduler.AllocateReaderWriterUserMutex( _allocator ).object ),
																														 _resourceViewLibraryMutex( scheduler.AllocateReaderWriterUserMutex( _allocator ).object ),
																														 _contentPackageLibrary( 64u, allocator, UTF8L("Content Library Package Bucket Allocator") ),
																														 _resourceViewLibrary( 512u, allocator, UTF8L("Content Library Resource View Bucket Allocator") ),
																														 _resourceViewFactoryLibrary( 32u, allocator, UTF8L("Content Library Resource View Factory Bucket Allocator") ),
																														 _loaderThread( new(_allocator, Allocator::AllocationOption::PERMANENT_ALLOCATION) LoaderThread( scheduler, _allocator ) ) {
		// Don't bother launching the thread if we have no synchronization mechanisms (Highly unlikely in practice!)
		if( ETBranchLikelyHint( _contentPackageLibraryMutex && _resourceViewLibraryMutex && _loaderThread ) ) {
			scheduler.Enqueue( *_loaderThread );
		}
	}

// ---------------------------------------------------

	ContentLibrary::~ContentLibrary() {
		ETRuntimeAssert( _contentPackageLibrary.Empty() );
		ETRuntimeAssert( _resourceViewLibrary.Empty() );

		if( auto* const thread = _loaderThread ) {
			thread->EnsureTerminated();
			_allocator.Delete( *thread );
		}

		_allocator.Delete( _contentPackageLibraryMutex, ::Eldritch2::AlignedDeallocationSemantics );
		_allocator.Delete( _resourceViewLibraryMutex, ::Eldritch2::AlignedDeallocationSemantics );
	}

// ---------------------------------------------------

	DisposingResultPair<ContentPackage> ContentLibrary::ResolvePackageByName( const UTF8Char* const packageName ) {
		using AllocationOption = Allocator::AllocationOption;

	// ---

		ScopedLock	packageTableLock( _contentPackageLibraryMutex );
		const auto	candidate( _contentPackageLibrary.Find( packageName ) );

		if( candidate != _contentPackageLibrary.End() ) {
			// Yes, we already have loaded this package. Increment the reference count and return it.
			return { ObjectHandle<ContentPackage>( candidate->second ), Errors::NONE };
		}

		// Try to create a new package. It will add itself to our collection.
		if( auto* const newPackage = new(_allocator, AllocationOption::PERMANENT_ALLOCATION) ContentPackage( packageName, *this, _allocator ) ) {
			if( auto* const newDeserializer = new(_deserializationContextAllocator, AllocationOption::TEMPORARY_ALLOCATION) DeserializationContext( ObjectHandle<ContentPackage>( newPackage ) ) ) {
				_loaderThread->AddDeserializationContext( *newDeserializer );

				return { ObjectHandle<ContentPackage>( newPackage, ::Eldritch2::PassthroughReferenceCountingSemantics ), Errors::NONE };
			}
			
			_allocator.Delete( *newPackage );
		}
		
		return { ObjectHandle<ContentPackage>( nullptr ), Errors::OUT_OF_MEMORY };
	}

// ---------------------------------------------------

	DisposingResultPair<ContentPackage> ContentLibrary::CreatePackageForEditorWorld() {
		using AllocationOption = Allocator::AllocationOption;

	// ---

		auto* const	newPackage( new(_allocator, AllocationOption::PERMANENT_ALLOCATION) ContentPackage( *this, _allocator ) );

		return { ObjectHandle<ContentPackage>( newPackage, ::Eldritch2::PassthroughReferenceCountingSemantics ), newPackage ? Errors::NONE : Errors::OUT_OF_MEMORY };
	}

}	// namespace FileSystem
}	// namespace Eldritch2

#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif