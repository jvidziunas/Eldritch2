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
		using ResidencyState = ContentPackage::ResidencyState;

	// ---

		MessagePackReader::ArrayHeader	header;
		ErrorCode						result( Error::NONE );
		auto&							package( GetBoundPackage() );

		if( _reader( header ) ) {
			// Keep assembling import records from the data stream.
			for( MessagePackReader::String dependencyName; 0u != header.arraySizeInElements; --header.arraySizeInElements ) {
				const UTF8String<FixedStackAllocator<64u>>	terminatedName( dependencyName.first, dependencyName.Size(), UTF8L("Dependency Name Temporary Allocator") );

				result = _reader( dependencyName ) ? package.AddDependency( terminatedName.GetCharacterArray() ) : Error::INVALID_PARAMETER;

				if( !result ) {
					break;
				}
			}
		} else {
			result = Error::INVALID_PARAMETER;
		}

		// Publish load failure in the event something went wrong.
		if( !result ) {
			package.UpdateResidencyStateOnLoaderThread( ResidencyState::FAILED );
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

		using ResidencyState = ContentPackage::ResidencyState;

	// ---

		MessagePackReader::ArrayHeader	header;
		ErrorCode						result( Error::NONE );
		auto&							package( GetBoundPackage() );

		if( _reader( header ) ) {
			// Keep assembling import records from the data stream
			for( ExportRecord exportRecord; 0u != header.arraySizeInElements; --header.arraySizeInElements ) {
				result = _reader( exportRecord ) ? package.AddContent( exportRecord ) : Error::INVALID_PARAMETER;

				if( !result ) {
					break;
				}
			}
		} else {
			result = Error::INVALID_PARAMETER;
		}

		// Broadcast the new residency state (either published or failed) depending on whether or not the load was successful.
		package.UpdateResidencyStateOnLoaderThread( result ? ResidencyState::PUBLISHED : ResidencyState::FAILED );

		return result;
	}

}	// namespace FileSystem
}	// namespace Eldritch2