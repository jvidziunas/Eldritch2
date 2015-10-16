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
#include <Utility/ReadableMemoryMappedFile.hpp>
#include <Utility/DisposingResult.hpp>
#include <Packages/ContentProvider.hpp>
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

	Range<const char*> GetFileContents( ReadableMemoryMappedFile* const file ) {
		return file ? file->TryGetStructureArrayAtOffset<const char>( 0u, file->GetAccessibleRegionSizeInBytes() ) : Range<const char*>::EmptySet();
	}

}	// anonymous namespace

namespace Eldritch2 {
namespace FileSystem {

	PackageDeserializationContext::PackageDeserializationContext( ObjectHandle<ContentPackage>&& package ) : _packageReference( move( package ) ),
																											 _allocator( UTF8L("Deserialization Context File Allocator") ),
																											 _file( GetBoundPackage().CreateBackingFile( _allocator, UTF8L(".eldritch2package") ).object ),
																											 _reader( GetFileContents( _file ) ) {}

// ---------------------------------------------------

	PackageDeserializationContext::~PackageDeserializationContext() {
		_allocator.Delete( _file );
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

		using ResidencyState = ContentPackage::ResidencyState;

	// ---

		FixedStackAllocator<128u>	dependencyAllocator( UTF8L("PackageDeserializationContext::DeserializeDependencies() Temporary Allocator") );
		MessagePackReader::Array	header;
		ErrorCode					result( Error::NONE );

		if( _reader( header ) ) {
			const auto	checkpoint( dependencyAllocator.CreateCheckpoint() );

			for( ; 0u != header.sizeInElements; --header.sizeInElements ) {
				if( !_reader( ImportRecord( dependencyAllocator, GetBoundPackage() ) ) ) {
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

		using ResidencyState = ContentPackage::ResidencyState;

	// ---

		MessagePackReader::Array	header;
		ErrorCode					result( Error::NONE );

		if( _reader( header ) ) {
			// Keep assembling import records from the data stream
			for( ExportRecord exportRecord; 0u != header.sizeInElements; --header.sizeInElements ) {
				result = _reader( exportRecord ) ? GetBoundPackage().AddContent( exportRecord ) : Error::INVALID_PARAMETER;

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