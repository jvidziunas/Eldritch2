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
#include <Utility/Concurrency/Lock.hpp>
#include <Packages/ContentLibrary.hpp>
#include <Packages/ContentPackage.hpp>
#include <Scheduler/TaskScheduler.hpp>
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

	ContentLibrary::ResourceViewKey::ResourceViewKey( const ResourceView& view ) : Pair<const UTF8Char*, const type_info*>( view.GetName().GetCharacterArray(), &typeid(view) ) {}

// ---------------------------------------------------

	ContentLibrary::ContentLibrary( ContentProvider& contentProvider, TaskScheduler& scheduler, Allocator& allocator ) : _allocator( allocator, UTF8L("Content Library Package Data Allocator") ),
																														 _deserializationContextAllocator( allocator, UTF8L("Content Library Package Data Allocator") ),
																														 _contentProvider( contentProvider ),
																														 _contentPackageDirectoryMutex( scheduler.AllocateReaderWriterUserMutex( _allocator ).object, { _allocator } ),
																														 _resourceViewDirectoryMutex( scheduler.AllocateReaderWriterUserMutex( _allocator ).object, { _allocator } ),
																														 _contentPackageDirectory( { allocator, UTF8L("Content Library Package Bucket Allocator") } ),
																														 _resourceViewDirectory( { allocator, UTF8L("Content Library Resource View Bucket Allocator") } ),
																														 _resourceFactoryDirectory( { allocator, UTF8L("Content Library Resource View Factory Bucket Allocator") } ),
																														 _loaderThread( new(_allocator, Allocator::AllocationOption::PERMANENT_ALLOCATION) LoaderThread( scheduler, _allocator ) ) {
		// Don't bother launching the thread if we have no synchronization mechanisms (Highly unlikely in practice!)
		if( ETBranchLikelyHint( _contentPackageDirectoryMutex && _resourceViewDirectoryMutex && _loaderThread ) ) {
			scheduler.Enqueue( *_loaderThread );
		}

		_contentPackageDirectory.Reserve( 64u );
		_resourceViewDirectory.Reserve( 1024u );
		_resourceFactoryDirectory.Reserve( 32u );
	}

// ---------------------------------------------------

	ContentLibrary::~ContentLibrary() {
		ETRuntimeAssert( _contentPackageDirectory.Empty() );
		ETRuntimeAssert( _resourceViewDirectory.Empty() );

		if( auto thread = _loaderThread ) {
			thread->EnsureTerminated();
			_allocator.Delete( *thread );
		}
	}

// ---------------------------------------------------

	CountedResult<const ContentPackage> ContentLibrary::ResolvePackageByName( const UTF8Char* const packageName ) {
		class DeserializedContentPackage : public ContentPackage {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref DeserializedContentPackage instance.
			ETInlineHint DeserializedContentPackage( const UTF8Char* const name, ContentLibrary& owner, Allocator& allocator ) : ContentPackage( name, owner, allocator ) {
				owner._contentPackageDirectory.Insert( { GetName(), this } );
			}

			~DeserializedContentPackage() = default;

		// ---------------------------------------------------

			void Dispose() override {
				GetContentLibrary()._allocator.Delete( *this );
			}
		};

	// ---

		// Oddball positioning here, but the scoping gymnastics are necessary to prevent lock re-entrancy in the content package destructor.
		ObjectHandle<DeserializedContentPackage>	createdPackage( nullptr );

		{	ScopedLock	_( *_contentPackageDirectoryMutex );
			const auto	packageCandidate( _contentPackageDirectory.Find( packageName ) );
		
			if( packageCandidate != _contentPackageDirectory.End() ) {
				// Yes, we already have loaded this package. Increment the reference count and return it.
				return { ObjectHandle<ContentPackage>( *packageCandidate->second ) };
			}

			createdPackage = new(_allocator, Allocator::AllocationOption::PERMANENT_ALLOCATION) DeserializedContentPackage( packageName, *this, _allocator );
		}	// End of lock scope.

		if( createdPackage ) {
			const auto	beginLoadResult( _loaderThread->BeginLoad( *createdPackage ) );

			if( beginLoadResult ) {
				// This is the first external reference to the package, so we want the passthrough reference counting semantics.
				return { ::std::move( createdPackage ) };
			}

			return { beginLoadResult };
		}
		
		return { Error::OUT_OF_MEMORY };
	}

// ---------------------------------------------------

	CountedResult<const ContentPackage> ContentLibrary::CreatePackageForEditorWorld() {
		class EditorPackage : public ContentPackage {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref EditorPackage instance.
			ETInlineHint EditorPackage( ContentLibrary& owningLibrary, Allocator& allocator ) : ContentPackage( UTF8L("<Editor Package>"), owningLibrary, allocator ) {
				// No content to load, so just skip directly to the published state.
				UpdateResidencyStateOnLoaderThread( ResidencyState::PUBLISHED );
			}

			~EditorPackage() = default;

		// ---------------------------------------------------

			void Dispose() override sealed {
				GetContentLibrary()._allocator.Delete( *this );
			}
		};

	// ---

		if( auto newPackage = new(_allocator, Allocator::AllocationOption::PERMANENT_ALLOCATION) EditorPackage( *this, _allocator ) ) {
			return { ObjectHandle<const ContentPackage>( newPackage, ::Eldritch2::PassthroughReferenceCountingSemantics ) };
		}

		return { Error::OUT_OF_MEMORY };
	}

}	// namespace FileSystem
}	// namespace Eldritch2

#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif