/*==================================================================*\
  DeserializationContext.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/ReadableMemoryMappedFile.hpp>
#include <Packages/DeserializationContext.hpp>
#include <Utility/DisposingResultPair.hpp>
#include <Utility/MessagePackReader.hpp>
#include <Packages/ContentProvider.hpp>
#include <Packages/ContentLibrary.hpp>
#include <Packages/ContentPackage.hpp>
#include <Utility/ResultPair.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;
using namespace ::std;

namespace Eldritch2 {
namespace FileSystem {

	DeserializationContext::DeserializationContext( ObjectHandle<ContentPackage>&& package ) : _packageReference( ::std::move( package ) ), _allocator( UTF8L("Deserialization Context File Allocator") ), _file( nullptr ) {
		using PackageCollection = decltype(_packageReference->_owningLibrary._contentPackageLibrary);

	// ---

		auto&	contentLibrary( _packageReference->_owningLibrary );

		// ETRuntimeAssert( lock.IsAttachedTo( contentLibrary._resourceViewLibraryMutex ) );

		contentLibrary._contentPackageLibrary.Insert( PackageCollection::ValueType( _packageReference->_name.GetCharacterArray(), static_cast<ContentPackage*>(_packageReference) ) );
	}

// ---------------------------------------------------

	DeserializationContext::~DeserializationContext() {
		_allocator.Delete( _file );
	}

// ---------------------------------------------------

	ErrorCode DeserializationContext::DeserializeDependencies() {
		auto	InstantiateBackingFile( [] ( ContentProvider& contentProvider, DeserializationContext& context ) -> ErrorCode {
			UTF8Char	fileName[128u];

			const auto createTableOfContentsResult = contentProvider.CreateReadableMemoryMappedFile( context._allocator, ContentProvider::KnownContentLocation::PACKAGE_DIRECTORY, fileName );
			if( createTableOfContentsResult ) {
				context._file = createTableOfContentsResult.object;
			}

			return createTableOfContentsResult;
		} );
		auto	DeserializeDependenciesImplementation( [] ( ContentLibrary& library, DeserializationContext& context, ReadableMemoryMappedFile& file ) -> ErrorCode {
			MessagePackReader::ArrayHeader	header;
			MessagePackReader				reader( file.TryGetStructureArrayAtOffset<char>( 0u, file.GetAccessibleRegionSizeInBytes() ) );
			auto&							referencedPackages( context.GetBoundPackage()._referencedPackages );

			if( reader( header ) ) {
				referencedPackages.SetCapacity( header.arraySizeInElements );

				// Keep assembling import records from the data stream.
				for( MessagePackReader::InPlaceString dependencyName; 0u != header.arraySizeInElements; --header.arraySizeInElements ) {
					ErrorCode	parseError( reader( dependencyName ) ? Errors::NONE : Errors::INVALID_PARAMETER );

					if( parseError ) {
						if( const auto resolvePackageResult = library.ResolvePackageByName( dependencyName.first ) ) {
							referencedPackages.PushBack( ::std::move( resolvePackageResult.object ) );
						} else {
							parseError = resolvePackageResult.resultCode;
						}
					}

					if( !parseError ) {
						return parseError;
					}

				}

				return Errors::NONE;
			}

			return Errors::INVALID_PARAMETER;
		} );

		auto	result( InstantiateBackingFile( GetBoundPackage()._owningLibrary._contentProvider, *this ) );

		if( result ) {
			result = DeserializeDependenciesImplementation( GetBoundPackage()._owningLibrary, *this, *_file );
		}

		if( !result ) {
			GetBoundPackage()._residencyState.store( ContentPackage::ResidencyState::FAILED, memory_order_release );
		}
		
		return result;
	}

// ---------------------------------------------------

	ErrorCode DeserializationContext::DeserializeContent() {
		using ResourceViewLibrary	= decltype(GetBoundPackage()._owningLibrary._resourceViewLibrary);
		using ViewFactoryLibrary	= decltype(GetBoundPackage()._owningLibrary._resourceViewFactoryLibrary);
		using ResidencyState		= ContentPackage::ResidencyState;

		struct SerializedExportRecord {
			ETInlineHint operator ResourceView::Initializer() const {
				return { name, typeName, serializedAsset };
			}

			ETInlineHint bool Serialize( MessagePackReader& reader ) {
				MessagePackReader::ArrayHeader header;

				return (reader( header ) && header.arraySizeInElements == 3u) ? reader( name, typeName, serializedAsset ) : false;
			}

		// - DATA MEMBERS ------------------------------------

			MessagePackReader::InPlaceString	name;
			MessagePackReader::TypeString		typeName;
			MessagePackReader::InPlaceBulkData	serializedAsset;
		};

	// ---

		auto	DeserializeContentImplementation( [] ( DeserializationContext& context ) -> ErrorCode {
			auto	AllocateViewsForExport( [] ( Allocator& allocator, const ResourceView::Initializer& initializer, ViewFactoryLibrary& viewFactoryLibrary ) -> ErrorCode {
				const auto	candidate( viewFactoryLibrary.Find( ViewFactoryLibrary::KeyType( initializer.typeName.first, initializer.typeName.onePastLast ) ) );

				if( candidate != viewFactoryLibrary.End() ) {
					for( auto& factory : candidate->second ) {
						const auto	result( factory.factoryFunction( allocator, initializer, factory.parameter ) );

						if( !result ) {
							return result;
						}
					}
				}

				return Errors::NONE;
			} );

			MessagePackReader::ArrayHeader	header;
			MessagePackReader				reader;
			auto&							viewFactoryLibrary( context.GetBoundPackage()._owningLibrary._resourceViewFactoryLibrary );

			if( reader( header ) ) {
				// Keep assembling import records from the data stream
				for( SerializedExportRecord exportRecord; 0u != header.arraySizeInElements; --header.arraySizeInElements ) {
					ErrorCode	parseError( reader( exportRecord ) ? Errors::NONE : Errors::INVALID_PARAMETER );

					if( parseError ) {
						parseError = AllocateViewsForExport( context.GetBoundPackage()._allocator, exportRecord, viewFactoryLibrary );
					}

					if( !parseError ) {
						return parseError;
					}
				}

				return Errors::NONE;
			}

			return Errors::INVALID_PARAMETER;
		} );

		auto&		package( GetBoundPackage() );
		auto&		contentLibrary( package._owningLibrary );
		const auto	deserializeContentResult( DeserializeContentImplementation( *this ) );

		if( deserializeContentResult ) {
			ScopedLock	_( contentLibrary._resourceViewLibraryMutex );
			auto&		viewLibrary( contentLibrary._resourceViewLibrary );

			for( const ResourceView& view : package.GetExportedResourceCollection() ) {
				viewLibrary.Insert( ResourceViewLibrary::ValueType( view.GetName().GetCharacterArray(), &view ) );
			}
		} else {
			package.DeleteContent();
		}

		// Broadcast the new residency state (either published or failed) depending on whether or not the load was successful.
		package._residencyState.store( (deserializeContentResult ? ResidencyState::PUBLISHED : ResidencyState::FAILED), memory_order_release );

		return deserializeContentResult;
	}

// ---------------------------------------------------

	void DeserializationContext::UnpublishPackage( ContentPackage& package ) {
		ScopedLock	_( package._owningLibrary._contentPackageLibraryMutex );

		package._owningLibrary._contentPackageLibrary.Erase( package._name.GetCharacterArray() );
	}

}	// namespace FileSystem
}	// namespace Eldritch2