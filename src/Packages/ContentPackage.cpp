/*==================================================================*\
  ContentPackage.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <FileSystem/ReadableMemoryMappedFile.hpp>
#include <Utility/Memory/ArenaAllocator.hpp>
#include <FileSystem/ContentProvider.hpp>
#include <Packages/ContentPackage.hpp>
#include <Packages/ContentLibrary.hpp>
//------------------------------------------------------------------//
#include <Packages/PackageHeader_generated.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace FileSystem {

	ContentPackage::ContentPackage( const UTF8Char* const name, ContentLibrary& library, Allocator& allocator ) : ReferenceCountable( 0u ),
																												  _allocator( allocator, UTF8L("Package Allocator") ),
																												  _name( name, { _allocator, UTF8L("Package Name Allocator") } ),
																												  _owningLibrary( library ),
																												  _residencyState( ResidencyState::Loading ),
																												  _referencedPackages( { _allocator, UTF8L("Package Dependency Collection Allocator") } ),
																												  _tableOfContentsFile( nullptr, { _allocator } ),
																												  _streamingDataFile( nullptr, { _allocator } ) {}

// ---------------------------------------------------

	ContentPackage::~ContentPackage() {
		for( const ResourceView& view : GetExports() ) {
			view.DetachFromPackage( *this, _owningLibrary );
		}

		// Caching.
		auto&	allocator( GetAllocator() );

		// It's not necessary to maintain a lock on the content table while we delete resources
		// (they've already been unpublished, we're the only thing that can see them)
		_exportedResources.ClearAndDispose( [&allocator] ( ResourceView& currentResource ) {
			allocator.Delete( currentResource );
		} );

		// Remove the package from the shared library.
		{	ScopedLock	_( *GetContentLibrary()._contentPackageDirectoryMutex );
			_owningLibrary._contentPackageDirectory.Erase( GetName() );
		}
	}

// ---------------------------------------------------

	ErrorCode ContentPackage::DeserializeDependencies() {
		if( nullptr == _tableOfContentsFile ) {
			FixedStackAllocator<256u>	tempAllocator( UTF8L("ContentPackage::DeserializeDependencies() Temporary Allocator") );
			UTF8String<>				packageName( _name.GetLength() + StringLength( FlatBuffers::PackageHeaderExtension() ), { tempAllocator, UTF8L("Package Name Allocator") } );

			packageName.Append( _name ).Append( FlatBuffers::PackageHeaderExtension() );

			const auto	createTableOfContentsResult( GetContentLibrary().GetContentProvider().CreateReadableMemoryMappedFile( GetAllocator(), ContentProvider::KnownContentLocation::PackageDirectory, packageName.AsCString() ) );

			if( createTableOfContentsResult ) {
				_tableOfContentsFile.reset( createTableOfContentsResult.object );
			} else {
				UpdateResidencyStateOnLoaderThread( ResidencyState::Failed );
				return createTableOfContentsResult.resultCode;
			}
		}

		const auto	data( _tableOfContentsFile->GetAddressForFileByteOffset( 0u ) );

		// Ensure we have data and that there is at minimum a marker.
		if( nullptr == data || !FlatBuffers::PackageHeaderBufferHasIdentifier( data ) ) {
			// Publish load failure.
			UpdateResidencyStateOnLoaderThread( ResidencyState::Failed );

			return Error::InvalidParameter;
		}

		// Loop through the list of additional content the package signifies itself as requiring.
		if( const auto imports = FlatBuffers::GetPackageHeader( data )->Imports() ) {
			for( auto currentImport : *imports ) {
				auto	packageResolutionResult( _owningLibrary.ResolvePackageByName( currentImport->c_str() ) );

				if( !packageResolutionResult ) {
					return packageResolutionResult.resultCode;
				}

				_referencedPackages.PushBack( ::std::move( packageResolutionResult.object ) );
			}
		}

		return Error::None;
	}

// ---------------------------------------------------

	ErrorCode ContentPackage::DeserializeContent() {
		FixedStackAllocator<256u>	tempAllocator( UTF8L("ContentPackage::DeserializeContent() Temporary Allocator") );

		// This should have been created before deserialization began. If this hasn't been created yet, someone's been ignoring documentation/error codes!
		if( nullptr == _tableOfContentsFile ) {
			return Error::InvalidObjectState;
		}

		if( const auto exports = FlatBuffers::GetPackageHeader( _tableOfContentsFile->GetAddressForFileByteOffset( 0u ) )->Exports() ) {
			const auto	createDataFileResult( GetContentLibrary().GetContentProvider().CreateReadableMemoryMappedFile( tempAllocator, ContentProvider::KnownContentLocation::PackageDirectory, _name.AsCString() ) );

			if( !createDataFileResult ) {
				UpdateResidencyStateOnLoaderThread( ResidencyState::Failed );
				return createDataFileResult.resultCode;
			}

			const auto&	dataFile( *createDataFileResult.object );

			for( auto currentExport : *exports ) {
				for( const auto& factory : GetContentLibrary().GetFactoriesForResourceType( currentExport->Type()->c_str() ) ) {
					const auto	result( factory.AllocateResourceView( GetAllocator(), currentExport->Name()->c_str() ) );

					if( !result ) {
						UpdateResidencyStateOnLoaderThread( ResidencyState::Failed );
						return result;
					}

					if( result.object ) {
						_exportedResources.PushFront( *result.object );
						result.object->AttachToPackage( dataFile.TryGetStructureArrayAtOffset<char>( currentExport->Offset(), currentExport->Length() ), *this, _owningLibrary );
					}
				}
			}
		}

		// Broadcast the new residency state.
		UpdateResidencyStateOnLoaderThread( ResidencyState::Published );

		return Error::None;
	}

}	// namespace FileSystem
}	// namespace Eldritch2