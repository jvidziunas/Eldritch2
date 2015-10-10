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
#include <Utility/DisposingResultPair.hpp>
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
		ErrorCode						result( Errors::NONE );
		auto&							package( GetBoundPackage() );

		if( _reader( header ) ) {
			// Keep assembling import records from the data stream.
			for( MessagePackReader::InPlaceString dependencyName; 0u != header.arraySizeInElements; --header.arraySizeInElements ) {
				result = _reader( dependencyName ) ? package.AddDependency( dependencyName.first ) : Errors::INVALID_PARAMETER;

				if( !result ) {
					break;
				}
			}
		} else {
			result = Errors::INVALID_PARAMETER;
		}

		// Publish load failure in the event something went wrong.
		if( !result ) {
			package.UpdateResidencyStateOnLoaderThread( ResidencyState::FAILED );
		}
		
		return result;
	}

// ---------------------------------------------------

	ErrorCode PackageDeserializationContext::DeserializeContent() {
		struct SerializedExportRecord {
			ETInlineHint operator ResourceView::Initializer() const {
				return { name, typeName, serializedAsset };
			}

			ETInlineHint bool Serialize( MessagePackReader& reader ) {
				return reader( name, typeName, serializedAsset );
			}

		// - DATA MEMBERS ------------------------------------

			MessagePackReader::InPlaceString	name;
			MessagePackReader::TypeString		typeName;
			MessagePackReader::InPlaceBulkData	serializedAsset;
		};

		using ResidencyState = ContentPackage::ResidencyState;

	// ---

		MessagePackReader::ArrayHeader	header;
		ErrorCode						result( Errors::NONE );
		auto&							package( GetBoundPackage() );

		if( _reader( header ) ) {
			// Keep assembling import records from the data stream
			for( SerializedExportRecord exportRecord; 0u != header.arraySizeInElements; --header.arraySizeInElements ) {
				result = _reader( exportRecord ) ? package.AddContent( exportRecord ) : Errors::INVALID_PARAMETER;

				if( !result ) {
					break;
				}
			}
		} else {
			result = Errors::INVALID_PARAMETER;
		}

		// Broadcast the new residency state (either published or failed) depending on whether or not the load was successful.
		package.UpdateResidencyStateOnLoaderThread( result ? ResidencyState::PUBLISHED : ResidencyState::FAILED );

		return result;
	}

}	// namespace FileSystem
}	// namespace Eldritch2