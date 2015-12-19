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
#include <FileSystem/ReadableMemoryMappedFile.hpp>
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
																												  _allocator( UTF8L("Deserialization Context File Allocator") ),
																												  _file( nullptr ) {}

// ---------------------------------------------------

	PackageDeserializationContext::~PackageDeserializationContext() {
		_allocator.Delete( _file );
	}

// ---------------------------------------------------

	ErrorCode PackageDeserializationContext::OpenFile() {
		if( _file ) {
			return Error::NONE;
		}

		const auto	createFileResult( CreateBackingFile( _allocator, UTF8L(".eldritch2package") ) );

		if( createFileResult ) {
			_file = createFileResult.object;
		} else {
			GetBoundPackage().UpdateResidencyStateOnLoaderThread( ResidencyState::FAILED );
		}

		return createFileResult.resultCode;
	}

// ---------------------------------------------------

	ErrorCode PackageDeserializationContext::DeserializeDependencies() {
		// This should have been created before deserialization began. If this hasn't been created yet, someone's been ignoring documentation/error codes!
		ETRuntimeAssert( nullptr != _file );

	// ---

		ErrorCode	result( Error::NONE );
		
		if( auto data = _file->GetAddressForFileByteOffset( 0u ) ) {
			if( auto imports = FlatBuffers::HeaderBufferHasIdentifier( data ) ? FlatBuffers::GetHeader( data )->imports() : nullptr ) {
				for( auto import : *imports ) {
					if( auto packageResolutionResult = GetContentLibrary().ResolvePackageByName( import->c_str() ) ) {
						GetBoundPackage().GetDependencies().PushBack( ::std::move( packageResolutionResult.object ) );
					} else {
						result = packageResolutionResult.resultCode;
						break;
					}
				}
			}
		} else {
			// Publish load failure.
			GetBoundPackage().UpdateResidencyStateOnLoaderThread( ResidencyState::FAILED );

			result = Error::INVALID_PARAMETER;
		}

		return result;
	}

// ---------------------------------------------------

	ErrorCode PackageDeserializationContext::DeserializeContent() {
		// This should have been created before deserialization began. If this hasn't been created yet, someone's been ignoring documentation/error codes!
		ETRuntimeAssert( nullptr != _file );

	// ---

		ErrorCode	result( Error::NONE );
		auto&		package( GetBoundPackage() );

		if( auto exports = FlatBuffers::GetHeader( _file->GetAddressForFileByteOffset( 0u ) )->exports() ) {
			for( auto definition : *exports ) {
				for( auto& factory : GetContentLibrary().GetFactoriesForResourceType( definition->type()->c_str() ) ) {
					result = factory.AllocateResourceView( package.GetAllocator(), package.GetContentLibrary(), package, definition->name()->c_str() );

					if( !result ) {
						break;
					}
				}
			}
		}

		// Broadcast the new residency state (either published or failed) depending on whether or not the load was successful.
		package.UpdateResidencyStateOnLoaderThread( result ? ResidencyState::PUBLISHED : ResidencyState::FAILED );

		return result;
	}

// ---------------------------------------------------

	Result<ReadableMemoryMappedFile> PackageDeserializationContext::CreateBackingFile( Allocator& allocator, const UTF8Char* const suffix ) {
		const size_t						nameSizeInBytes( StringLength( GetBoundPackage().GetName() ) + StringLength( suffix ) );
		UTF8String<ExternalArenaAllocator>	fileName( { _alloca( nameSizeInBytes + 33u ), nameSizeInBytes + 33u, UTF8L( "ContentPackage::CreateBackingFile() Temporary Allocator" ) } );

		fileName.Reserve( nameSizeInBytes );
		fileName.Append( GetBoundPackage().GetName() ).Append( suffix ? suffix : UTF8L("") );

		return GetContentLibrary().GetContentProvider().CreateReadableMemoryMappedFile( allocator, ContentProvider::KnownContentLocation::PACKAGE_DIRECTORY, fileName.GetCharacterArray() );
	}

// ---------------------------------------------------

	ContentLibrary& PackageDeserializationContext::GetContentLibrary() {
		return _packageReference->GetContentLibrary();
	}

}	// namespace FileSystem
}	// namespace Eldritch2