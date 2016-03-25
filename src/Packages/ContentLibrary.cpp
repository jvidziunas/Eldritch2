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
#include <Utility/Memory/InstanceNew.hpp>
#include <Scheduler/ThreadScheduler.hpp>
#include <Utility/Concurrency/Lock.hpp>
#include <Packages/ContentLibrary.hpp>
#include <Packages/ContentPackage.hpp>
#include <Utility/CountedResult.hpp>
#include <Packages/LoaderThread.hpp>
#include <Packages/ResourceView.hpp>
#include <Utility/Result.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace FileSystem {

	ContentLibrary::ContentLibrary( ContentProvider& contentProvider, ThreadScheduler& scheduler, Allocator& allocator ) : _allocator( allocator, UTF8L("Content Library Package Data Allocator") ),
																														   _contentProvider( contentProvider ),
																														   _contentPackageDirectoryMutex( scheduler.AllocateReaderWriterUserMutex( _allocator ).object, { _allocator } ),
																														   _resourceViewDirectoryMutex( scheduler.AllocateReaderWriterUserMutex( _allocator ).object, { _allocator } ),
																														   _contentPackageDirectory( { allocator, UTF8L("Content Library Package Bucket Allocator") } ),
																														   _resourceViewDirectory( { allocator, UTF8L("Content Library Resource View Bucket Allocator") } ),
																														   _resourceFactoryDirectory( { allocator, UTF8L("Content Library Resource View Factory Bucket Allocator") } ),
																														   _orphanContentNotifier( contentProvider, _allocator ),
																														   _loaderThread( new(_allocator, Allocator::AllocationDuration::Normal) LoaderThread( scheduler, _allocator ) ) {
		// Launch the thread if we have synchronization mechanisms.
		if( ETBranchLikelyHint( _contentPackageDirectoryMutex && _resourceViewDirectoryMutex && _loaderThread ) ) {
			scheduler.Enqueue( *_loaderThread );
		}

		_orphanContentFactories.PushFront( _orphanContentNotifier );

		_contentPackageDirectory.Reserve( 64u );
		_resourceViewDirectory.Reserve( 1024u );
	}

// ---------------------------------------------------

	ContentLibrary::~ContentLibrary() {
		_orphanContentFactories.Clear();

		if( auto thread = _loaderThread ) {
			thread->EnsureTerminated();
			_allocator.Delete( *thread );
		}

		// Test *after* tearing down the loader thread, as internally it keeps packages alive.

		ETRuntimeAssert( _contentPackageDirectory.Empty() );
		ETRuntimeAssert( _resourceViewDirectory.Empty() );
	}

// ---------------------------------------------------

	CountedResult<const ContentPackage> ContentLibrary::ResolvePackageByName( const UTF8Char* const packageName ) {
		class DeserializedContentPackage : public ContentPackage {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref DeserializedContentPackage instance.
			ETInlineHint DeserializedContentPackage( const UTF8Char* const name, ContentLibrary& library, Allocator& allocator ) : ContentPackage( name, library, allocator ) {
				library._contentPackageDirectory.Insert( { GetName(), this } );
			}

			~DeserializedContentPackage() = default;

		// ---------------------------------------------------

			void Dispose() override {
				_owningLibrary._allocator.Delete( *this );
			}
		};

	// ---

		// Oddball positioning here, but the scoping gymnastics are necessary to prevent lock re-entrancy in the content package destructor.
		ObjectHandle<DeserializedContentPackage>	createdPackage;

		{	ScopedLock	_( *_contentPackageDirectoryMutex );
			const auto	packageCandidate( _contentPackageDirectory.Find( packageName ) );
		
			if( packageCandidate != _contentPackageDirectory.End() ) {
				// Yes, we already have loaded this package. Increment the reference count and return it.
				return { ObjectHandle<ContentPackage>( *packageCandidate->second ) };
			}

			createdPackage = new(_allocator, Allocator::AllocationDuration::Normal) DeserializedContentPackage( packageName, *this, _allocator );
		}	// End of lock scope.

		if( createdPackage ) {
			const auto	beginLoadResult( _loaderThread->BeginLoad( *createdPackage ) );

			if( beginLoadResult ) {
				// This is the first external reference to the package, so we want the passthrough reference counting semantics.
				return { ::std::move( createdPackage ) };
			}

			return { beginLoadResult };
		}
		
		return { Error::OutOfMemory };
	}

// ---------------------------------------------------

	CountedResult<const ContentPackage> ContentLibrary::CreatePackageForEditorWorld() {
		class EditorPackage : public ContentPackage {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref EditorPackage instance.
			ETInlineHint EditorPackage( ContentLibrary& library, Allocator& allocator ) : ContentPackage( UTF8L("<Editor Package>"), library, allocator ) {
				// No content to load, so just skip directly to the published state.
				UpdateResidencyStateOnLoaderThread( ResidencyState::Published );
			}

			~EditorPackage() = default;

		// ---------------------------------------------------

			void Dispose() override sealed {
				_owningLibrary._allocator.Delete( *this );
			}
		};

	// ---

		if( auto newPackage = new(_allocator, Allocator::AllocationDuration::Normal) EditorPackage( *this, _allocator ) ) {
			return { static_cast<ContentPackage*>(newPackage) };
		}

		return { Error::OutOfMemory };
	}

}	// namespace FileSystem
}	// namespace Eldritch2

#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif