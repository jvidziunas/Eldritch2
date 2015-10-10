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
#include <Utility/Memory/InstanceDeleters.hpp>
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
#include <memory>
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
		class DeserializedContentPackage : public ContentPackage {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref DeserializedContentPackage instance.
			ETInlineHint DeserializedContentPackage( const UTF8Char* const name, ContentLibrary& owner, Allocator& allocator ) : ContentPackage( name, owner, allocator ) {}

			//! Destroys this @ref DeserializedContentPackage instance.
			~DeserializedContentPackage() {
				auto&	contentLibrary( GetLibrary() );
				// Erase all the views from the shared library.
				{	ScopedLock	_( *contentLibrary._resourceViewLibraryMutex );
					auto&	resourceViewLibrary( contentLibrary._resourceViewLibrary );

					for( const auto& view : GetExportedResourceCollection() ) {
						const auto findViewResult( resourceViewLibrary.Find( view.GetName().GetCharacterArray() ) );

						if( findViewResult != resourceViewLibrary.End() && findViewResult->second == &view ) {
							resourceViewLibrary.Erase( findViewResult );
						}
					}
				}

				// Remove the package from the shared library.
				{	ScopedLock	_( *contentLibrary._contentPackageLibraryMutex );
					contentLibrary._contentPackageLibrary.Erase( GetName() );
				}
			}

		// ---------------------------------------------------

			ErrorCode AddDependency( const UTF8Char* const dependencyName ) override sealed {
				const auto	dereferenceResult( GetLibrary().ResolvePackageByName( dependencyName ) );

				if( dereferenceResult ) {
					GetDependencies().PushBack( move( dereferenceResult.object ) );
				}

				return dereferenceResult.resultCode;
			}

			ErrorCode AddContent( const ResourceView::Initializer& sourceAssetData ) override sealed {
				auto&		factoryLibrary( GetLibrary()._resourceViewFactoryLibrary );
				const auto	findFactoriesResult( factoryLibrary.Find( { sourceAssetData.name.first, sourceAssetData.name.onePastLast } ) );
				ErrorCode	result( Errors::NONE );

				if( findFactoriesResult != factoryLibrary.End() ) {
					auto&	viewLibrary( GetLibrary()._resourceViewLibrary );
					auto&	viewMutex( *GetLibrary()._resourceViewLibraryMutex );
					auto&	resourceAllocator( GetAllocator() );

					for( const auto& factory : findFactoriesResult->second ) {
						if( const auto createResourceResult = factory.factoryFunction( resourceAllocator, sourceAssetData, factory.parameter ) ) {
							ScopedLock	_( viewMutex );
							viewLibrary.Insert( { createResourceResult.object->GetName().GetCharacterArray(), createResourceResult.object } );
						} else {
							result = createResourceResult.resultCode;
							break;
						}
					}
				}

				return result;
			}

			void Dispose() override {
				GetLibrary()._allocator.Delete( *this );
			}
		};

		using AllocationOption	= Allocator::AllocationOption;
		using PackageHandle		= unique_ptr<ContentPackage, InstanceDeleter>;

	// ---

		ScopedLock	packageTableLock( *_contentPackageLibraryMutex );
		const auto	candidate( _contentPackageLibrary.Find( packageName ) );

		if( candidate != _contentPackageLibrary.End() ) {
			// Yes, we already have loaded this package. Increment the reference count and return it.
			return { { candidate->second }, Errors::NONE };
		}

		if( PackageHandle package { new(_allocator, AllocationOption::PERMANENT_ALLOCATION) DeserializedContentPackage( packageName, *this, _allocator ), { _allocator } } ) {
			auto	beginLoadResult( _loaderThread->BeginLoad( *package ) );

			if( beginLoadResult ) {
				_contentPackageLibrary.Insert( { package->GetName(), package.get() } );
				// This is the first external reference to the package, so we want the passthrough reference counting semantics.
				return { { *package.release(), ::Eldritch2::PassthroughReferenceCountingSemantics }, Errors::NONE };
			}

			return { { nullptr }, beginLoadResult };
		}
		
		return { { nullptr }, Errors::OUT_OF_MEMORY };
	}

// ---------------------------------------------------

	DisposingResultPair<ContentPackage> ContentLibrary::CreatePackageForEditorWorld() {
		class EditorPackage : public ContentPackage {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			// Constructs this @ref EditorPackage instance.
			ETInlineHint EditorPackage( ContentLibrary& owningLibrary, Allocator& allocator ) : ContentPackage( UTF8L("<Editor Package>"), owningLibrary, allocator ) {
				// No content to load, so just skip directly to the published state.
				UpdateResidencyStateOnLoaderThread( ResidencyState::PUBLISHED );
			}

			~EditorPackage() = default;

		// ---------------------------------------------------

			ErrorCode AddDependency( const UTF8Char* const dependencyName ) override sealed {
				const auto	dereferenceResult( GetLibrary().ResolvePackageByName( dependencyName ) );

				if( dereferenceResult ) {
					GetDependencies().PushBack( move( dereferenceResult.object ) );
				}

				return dereferenceResult.resultCode;
			}

			ErrorCode AddContent( const ResourceView::Initializer& /*sourceAssetData*/ ) override sealed {
				return Errors::OPERATION_NOT_SUPPORTED;
			}

			void Dispose() override sealed {
				GetLibrary()._allocator.Delete( *this );
			}
		};

		using AllocationOption = Allocator::AllocationOption;

	// ---

		auto* const	newPackage( new(_allocator, AllocationOption::PERMANENT_ALLOCATION) EditorPackage( *this, _allocator ) );

		return { { newPackage, ::Eldritch2::PassthroughReferenceCountingSemantics }, newPackage ? Errors::NONE : Errors::OUT_OF_MEMORY };
	}

}	// namespace FileSystem
}	// namespace Eldritch2

#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif