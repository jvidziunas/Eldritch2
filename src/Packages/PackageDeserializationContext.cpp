/*==================================================================*\
  PackageDeserializationContext.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Packages/PackageDeserializationContext.hpp>
#include <FileSystem/ContentProvider.hpp>
#include <Utility/CountedResult.hpp>
#include <Packages/ContentLibrary.hpp>
#include <Packages/ContentPackage.hpp>
#include <Utility/Result.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#include <Packages/PackageHeader_generated.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;
using namespace ::std;

namespace {

	using ResidencyState = ContentPackage::ResidencyState;

// ---

	Range<const char*> GetFileContents( ReadableMemoryMappedFile* const file ) {
		return file ? file->TryGetStructureArrayAtOffset<const char>( 0u, file->GetAccessibleRegionSizeInBytes() ) : Range<const char*>::EmptySet();
	}

}	// anonymous namespace

namespace Eldritch2 {
namespace FileSystem {

	PackageDeserializationContext::PackageDeserializationContext( const ObjectHandle<ContentPackage>& package ) : _packageReference( package ),
																												  _backingFileAllocator( UTF8L("Deserialization Context File Allocator") ),
																												  _tableOfContentsFile( nullptr, { _backingFileAllocator } ) {}

// ---------------------------------------------------

	ErrorCode PackageDeserializationContext::DeserializeDependencies() {
		using namespace ::Eldritch2::FileSystem::FlatBuffers;

	// ---

		if( nullptr == _tableOfContentsFile ) {
			const auto	createTableOfContentsResult( CreateBackingFile( _backingFileAllocator, UTF8L(".e2toc") ) );

			if( createTableOfContentsResult ) {
				_tableOfContentsFile.reset( createTableOfContentsResult.object );
			} else {
				GetBoundPackage().UpdateResidencyStateOnLoaderThread( ResidencyState::Failed );
				return createTableOfContentsResult.resultCode;
			}
		}

		const auto	data( _tableOfContentsFile->GetAddressForFileByteOffset( 0u ) );

		// Ensure we have data and that there is at minimum a marker.
		if( nullptr == data || !PackageHeaderBufferHasIdentifier( data ) ) {
			// Publish load failure.
			GetBoundPackage().UpdateResidencyStateOnLoaderThread( ResidencyState::Failed );

			return Error::InvalidParameter;
		}
		
		// Loop through the list of additional content the package signifies itself as requiring.
		if( auto imports = GetPackageHeader( data )->Imports() ) {
			for( auto currentImport : *imports ) {
				const auto	packageResolutionResult( GetContentLibrary().ResolvePackageByName( currentImport->c_str() ) );

				if( !packageResolutionResult ) {
					return packageResolutionResult.resultCode;
				}

				GetBoundPackage().GetDependencies().PushBack( ::std::move( packageResolutionResult.object ) );
			}
		}

		return Error::None;
	}

// ---------------------------------------------------

	ErrorCode PackageDeserializationContext::DeserializeContent() {
		using namespace ::Eldritch2::FileSystem::FlatBuffers;

		// This should have been created before deserialization began. If this hasn't been created yet, someone's been ignoring documentation/error codes!
		ETRuntimeAssert( nullptr != _tableOfContentsFile );

	// ---

		 if( const auto exports = GetPackageHeader( _tableOfContentsFile->GetAddressForFileByteOffset( 0u ) )->Exports() ) {
			const auto	dataFile( CreateBackingFile( _backingFileAllocator, UTF8L("") ) );

			if( !dataFile ) {
				GetBoundPackage().UpdateResidencyStateOnLoaderThread( ResidencyState::Failed );
				return dataFile;
			}

			for( auto currentExport : *exports ) {
				for( auto& currentFactory : GetContentLibrary().GetFactoriesForResourceType( currentExport->Type()->c_str() ) ) {
					const auto	result( currentFactory.AllocateResourceView( GetBoundPackage().GetAllocator(),
																			 GetBoundPackage().GetContentLibrary(),
																			 GetBoundPackage(),
																			 currentExport->Name()->c_str(),
																			 dataFile.object->TryGetStructureArrayAtOffset<const char>( currentExport->Offset(), currentExport->Length() ) ) );

					if( !result ) {
						GetBoundPackage().UpdateResidencyStateOnLoaderThread( ResidencyState::Failed );
						return result;
					}
				}
			}
		}

		// Broadcast the new residency state (either published or failed) depending on whether or not the load was successful.
		GetBoundPackage().UpdateResidencyStateOnLoaderThread( ResidencyState::Published );

		return Error::None;
	}

// ---------------------------------------------------

	Result<ReadableMemoryMappedFile> PackageDeserializationContext::CreateBackingFile( Allocator& allocator, const UTF8Char* const suffix ) {
		const size_t						nameSizeInBytes( StringLength( GetBoundPackage().GetName() ) + StringLength( suffix ) );
		UTF8String<ExternalArenaAllocator>	fileName( { _alloca( nameSizeInBytes + 33u ), nameSizeInBytes + 33u, UTF8L("ContentPackage::CreateBackingFile() Temporary Allocator") } );

		fileName.Reserve( nameSizeInBytes );
		fileName.Append( GetBoundPackage().GetName() ).Append( suffix ? suffix : UTF8L("") );

		return GetContentLibrary().GetContentProvider().CreateReadableMemoryMappedFile( allocator, ContentProvider::KnownContentLocation::PackageDirectory, fileName.GetCharacterArray() );
	}

// ---------------------------------------------------

	ContentLibrary& PackageDeserializationContext::GetContentLibrary() {
		return _packageReference->GetContentLibrary();
	}

}	// namespace FileSystem
}	// namespace Eldritch2