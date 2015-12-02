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
#include <Utility/DisposingResult.hpp>
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
using namespace ::std;

namespace Eldritch2 {
namespace FileSystem {

	ETNoAliasHint bool ContentLibrary::ResourceViewFactoryKey::operator==( const ResourceViewFactoryKey& right ) const {
		const auto	size( second - first );

		return (size == (right.second - right.first)) && CompareMemory( first, right.first, size );
	}

// ---------------------------------------------------

	ContentLibrary::ResourceViewKey::ResourceViewKey( const ResourceView& view ) : pair<const UTF8Char*, const type_info*>( view.GetName().GetCharacterArray(), &typeid(view) ) {}

// ---------------------------------------------------

	ContentLibrary::ContentLibrary( ContentProvider& contentProvider, TaskScheduler& scheduler, Allocator& allocator ) : _allocator( allocator, UTF8L("Content Library Package Data Allocator") ),
																														 _deserializationContextAllocator( allocator, UTF8L("Content Library Package Data Allocator") ),
																														 _contentProvider( contentProvider ),
																														 _contentPackageCollectionMutex( scheduler.AllocateReaderWriterUserMutex( _allocator ).object, { _allocator } ),
																														 _resourceViewCollectionMutex( scheduler.AllocateReaderWriterUserMutex( _allocator ).object, { _allocator } ),
																														 _contentPackageCollection( { allocator, UTF8L("Content Library Package Bucket Allocator") } ),
																														 _resourceViewCollection( { allocator, UTF8L("Content Library Resource View Bucket Allocator") } ),
																														 _resourceViewFactoryCollection( { allocator, UTF8L("Content Library Resource View Factory Bucket Allocator") } ),
																														 _loaderThread( new(_allocator, Allocator::AllocationOption::PERMANENT_ALLOCATION) LoaderThread( scheduler, _allocator ) ) {
		// Don't bother launching the thread if we have no synchronization mechanisms (Highly unlikely in practice!)
		if( ETBranchLikelyHint( _contentPackageCollectionMutex && _resourceViewCollectionMutex && _loaderThread ) ) {
			scheduler.Enqueue( *_loaderThread );
		}

		_contentPackageCollection.Reserve( 64u );
		_resourceViewCollection.Reserve( 128u );
		_resourceViewFactoryCollection.Reserve( 16u );
	}

// ---------------------------------------------------

	ContentLibrary::~ContentLibrary() {
		ETRuntimeAssert( _contentPackageCollection.Empty() );
		ETRuntimeAssert( _resourceViewCollection.Empty() );

		if( auto thread = _loaderThread ) {
			thread->EnsureTerminated();
			_allocator.Delete( *thread );
		}
	}

// ---------------------------------------------------

	DisposingResult<ContentPackage> ContentLibrary::ResolvePackageByName( const UTF8Char* const packageName ) {
		class DeserializedContentPackage : public ContentPackage {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref DeserializedContentPackage instance.
			ETInlineHint DeserializedContentPackage( const UTF8Char* const name, ContentLibrary& owner, Allocator& allocator ) : ContentPackage( name, owner, allocator ) {}

			//! Destroys this @ref DeserializedContentPackage instance.
			~DeserializedContentPackage() {
				// Cache this.
				auto&	contentLibrary( GetLibrary() );
				
				// Erase all the views from the shared library.
				{	ScopedLock	_( *contentLibrary._resourceViewCollectionMutex );
					// Caching.
					auto&	resourceViewLibrary( contentLibrary._resourceViewCollection );

					for( const auto& view : GetExportedResourceCollection() ) {
						const auto findViewResult( resourceViewLibrary.Find( { view } ) );

						// Ensure we're not accidentally erasing another resource.
						if( findViewResult != resourceViewLibrary.End() && findViewResult->second == &view ) {
							resourceViewLibrary.Erase( findViewResult );
						}
					}
				}

				// Remove the package from the shared library.
				{	ScopedLock	_( *contentLibrary._contentPackageCollectionMutex );
					contentLibrary._contentPackageCollection.Erase( GetName() );
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

			ErrorCode AddContent( const UTF8Char* const name, const UTF8Char* const typeName, const Range<const char*> data ) override sealed {
				auto&		factoryLibrary( GetLibrary()._resourceViewFactoryCollection );
				const auto	findFactoriesResult( factoryLibrary.Find( { typeName, FindEndOfString( typeName ) } ) );
				ErrorCode	result( Error::NONE );

				if( findFactoriesResult != factoryLibrary.End() ) {
					auto&	viewCollection( GetLibrary()._resourceViewCollection );
					auto&	viewMutex( *GetLibrary()._resourceViewCollectionMutex );
					auto&	resourceAllocator( GetAllocator() );

					// Loop through all the resource view factories...
					for( const auto& factory : findFactoriesResult->second ) {
						// ... attempting to create their respective resources...
						auto	createResourceResult( factory.first( resourceAllocator, name, factory.second ) );

						if( createResourceResult && createResourceResult->UpdateFromByteStream( data ) ) {
							// ... and publishing them to the content library.
							ScopedLock	_( viewMutex );
							auto&		resource( *createResourceResult.release() );

							viewCollection.Insert( { { resource }, &resource } );
						} else {
							result = Error::INVALID_PARAMETER;
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

	// ---

		using AllocationOption	= Allocator::AllocationOption;

	// ---

		// Oddball positioning here, but the scoping gymnastics are necessary to prevent lock re-entrancy in the content package destructor.
		InstancePointer<DeserializedContentPackage>	createdPackage( nullptr, { _allocator } );

		{	ScopedLock	packageTableLock( *_contentPackageCollectionMutex );
			const auto	candidate( _contentPackageCollection.Find( packageName ) );
		
			if( candidate != _contentPackageCollection.End() ) {
				// Yes, we already have loaded this package. Increment the reference count and return it.
				return { ObjectHandle<ContentPackage>( *candidate->second ) };
			}

			createdPackage.reset( new(_allocator, AllocationOption::PERMANENT_ALLOCATION) DeserializedContentPackage( packageName, *this, _allocator ) );

			if( createdPackage ) {
				_contentPackageCollection.Insert( { createdPackage->GetName(), createdPackage.get() } );
			}
		}

		if( createdPackage ) {
			auto	beginLoadResult( _loaderThread->BeginLoad( *createdPackage ) );

			if( beginLoadResult ) {
				// This is the first external reference to the package, so we want the passthrough reference counting semantics.
				return { ObjectHandle<ContentPackage>( createdPackage.release(), ::Eldritch2::PassthroughReferenceCountingSemantics ) };
			}

			return { beginLoadResult };
		}
		
		return { Error::OUT_OF_MEMORY };
	}

// ---------------------------------------------------

	DisposingResult<ContentPackage> ContentLibrary::CreatePackageForEditorWorld() {
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

			ErrorCode AddDependency( const UTF8Char* const dependencyName ) override sealed {
				const auto	dereferenceResult( GetLibrary().ResolvePackageByName( dependencyName ) );

				if( dereferenceResult ) {
					GetDependencies().PushBack( move( dereferenceResult.object ) );
				}

				return dereferenceResult.resultCode;
			}

			ErrorCode AddContent( const UTF8Char* const /*name*/, const UTF8Char* const /*typeName*/, const Range<const char*> /*data*/ ) override sealed {
				return Error::OPERATION_NOT_SUPPORTED;
			}

			void Dispose() override sealed {
				GetLibrary()._allocator.Delete( *this );
			}
		};

		using AllocationOption = Allocator::AllocationOption;

	// ---

		if( auto newPackage = new(_allocator, AllocationOption::PERMANENT_ALLOCATION) EditorPackage( *this, _allocator ) ) {
			return { ObjectHandle<ContentPackage>( newPackage, ::Eldritch2::PassthroughReferenceCountingSemantics ) };
		}

		return { Error::OUT_OF_MEMORY };
	}

}	// namespace FileSystem
}	// namespace Eldritch2

#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif