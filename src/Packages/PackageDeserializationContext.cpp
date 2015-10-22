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
#include <Utility/MessagePackReader.hpp>
#include <Utility/DisposingResult.hpp>
#include <Packages/ContentLibrary.hpp>
#include <Packages/ContentPackage.hpp>
#include <Utility/Result.hpp>
#include <Utility/Assert.hpp>
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

	PackageDeserializationContext::PackageDeserializationContext( ObjectHandle<ContentPackage>&& package ) : _packageReference( move( package ) ),
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

		const auto	createFileResult( GetBoundPackage().CreateBackingFile( _allocator, UTF8L(".eldritch2package") ) );

		if( createFileResult ) {
			_file = createFileResult.object;
		} else {
			GetBoundPackage().UpdateResidencyStateOnLoaderThread( ResidencyState::FAILED );
		}

		return createFileResult.resultCode;
	}

// ---------------------------------------------------

	ErrorCode PackageDeserializationContext::DeserializeDependencies() {
		struct ImportRecord {
			ETInlineHint ImportRecord( Allocator& allocator, ContentPackage& package ) : _allocator( allocator ), _package( package ) {}
			
			ETInlineHint bool Serialize( MessagePackReader& reader ) {
				MessagePackReader::String dependencyName;

				if( reader( dependencyName ) ) {
					UTF8String<>	terminatedName( dependencyName.first, dependencyName.onePastLast, { _allocator, UTF8L("ImportRecord::Serialize() Temporary Name Allocator") } );

					return _package.AddDependency( terminatedName.GetCharacterArray() );
				}

				return false;
			}

		// - DATA MEMBERS ------------------------------------

			Allocator&		_allocator;
			ContentPackage&	_package;
		};

	// ---

		// This should have been created before deserialization began. If this hasn't been created yet, someone's been ignoring documentation/error codes!
		ETRuntimeAssert( nullptr != _file );

		MessagePackReader			reader( _file->TryGetStructureArrayAtOffset<char>( 0u, _file->GetAccessibleRegionSizeInBytes() ) );
		FixedStackAllocator<128u>	dependencyAllocator( UTF8L("PackageDeserializationContext::DeserializeDependencies() Temporary Allocator") );
		MessagePackReader::Array	header;
		ErrorCode					result( Error::NONE );

		if( reader( header ) ) {
			const auto	checkpoint( dependencyAllocator.CreateCheckpoint() );

			for( ; 0u != header.sizeInElements; --header.sizeInElements ) {
				if( !reader( ImportRecord( dependencyAllocator, GetBoundPackage() ) ) ) {
					result = Error::INVALID_PARAMETER;
					break;
				}

				dependencyAllocator.RestoreCheckpoint( checkpoint );
			}
		} else {
			result = Error::INVALID_PARAMETER;
		}

		// Publish load failure in the event something went wrong.
		if( !result ) {
			GetBoundPackage().UpdateResidencyStateOnLoaderThread( ResidencyState::FAILED );
		}
		
		return result;
	}

// ---------------------------------------------------

	ErrorCode PackageDeserializationContext::DeserializeContent() {
		struct ExportRecord {
			ETInlineHint operator ResourceView::Initializer() const {
				return { name, typeName, serializedAsset };
			}

			ETInlineHint bool Serialize( MessagePackReader& reader ) {
				return reader( name, typeName, serializedAsset );
			}

		// - DATA MEMBERS ------------------------------------

			MessagePackReader::String		name;
			MessagePackReader::String		typeName;
			MessagePackReader::BinaryData	serializedAsset;
		};

	// ---

		// This should have been created before deserialization began. If this hasn't been created yet, someone's been ignoring documentation/error codes!
		ETRuntimeAssert( nullptr != _file );

		MessagePackReader			reader( _file->TryGetStructureArrayAtOffset<char>( 0u, _file->GetAccessibleRegionSizeInBytes() ) );
		MessagePackReader::Array	header;
		ErrorCode					result( Error::NONE );

		if( reader( header ) ) {
			// Keep assembling import records from the data stream
			for( ExportRecord exportRecord; 0u != header.sizeInElements; --header.sizeInElements ) {
				result = reader( exportRecord ) ? GetBoundPackage().AddContent( exportRecord ) : Error::INVALID_PARAMETER;

				if( !result ) {
					break;
				}
			}
		} else {
			result = Error::INVALID_PARAMETER;
		}

		// Broadcast the new residency state (either published or failed) depending on whether or not the load was successful.
		GetBoundPackage().UpdateResidencyStateOnLoaderThread( result ? ResidencyState::PUBLISHED : ResidencyState::FAILED );

		return result;
	}

}	// namespace FileSystem
}	// namespace Eldritch2