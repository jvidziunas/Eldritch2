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
		// This should have been created before deserialization began. If this hasn't been created yet, someone's been ignoring documentation/error codes!
		ETRuntimeAssert( nullptr != _file );

		ErrorCode	result( Error::NONE );
		

		// Publish load failure in the event something went wrong.
		if( !result ) {
			GetBoundPackage().UpdateResidencyStateOnLoaderThread( ResidencyState::FAILED );
		}
		
		return result;
	}

// ---------------------------------------------------

	ErrorCode PackageDeserializationContext::DeserializeContent() {
		// This should have been created before deserialization began. If this hasn't been created yet, someone's been ignoring documentation/error codes!
		ETRuntimeAssert( nullptr != _file );

		ErrorCode	result( Error::NONE );

		// Broadcast the new residency state (either published or failed) depending on whether or not the load was successful.
		GetBoundPackage().UpdateResidencyStateOnLoaderThread( result ? ResidencyState::PUBLISHED : ResidencyState::FAILED );

		return result;
	}

}	// namespace FileSystem
}	// namespace Eldritch2