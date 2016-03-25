/*==================================================================*\
  BytecodePackageResourceView.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Angelscript/BytecodePackageResourceView.hpp>
#include <Utility/Memory/StandardLibrary.hpp>
#include <Utility/Math/StandardLibrary.hpp>
#include <Packages/ContentPackage.hpp>
#include <Utility/ErrorCode.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#include <Scripting/AngelScript/BytecodePackage_generated.h>
#include <angelscript.h>
//------------------------------------------------------------------//
#include <utility>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	BytecodePackageResourceView::BytecodePackageResourceView( const UTF8Char* const name, ModuleHandle&& module, Allocator& allocator ) : ResourceView( name ), BytecodeMetadata( allocator ), _module( ::std::move( module ) ) {}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const BytecodePackageResourceView::GetSerializedDataTag() {
		return FlatBuffers::ModuleMetadataExtension();
	}

// ---------------------------------------------------

	ErrorCode BytecodePackageResourceView::AttachToPackage( const Range<const char*> bytes, ContentPackage& /*package*/, ContentLibrary& library ) {
		struct Reader : public ::asIBinaryStream {
			ETInlineHint Reader( const Range<const char*>& sourceData ) : remainingData( sourceData ) {}

			void Read( void* destination, ::asUINT sizeInBytes ) override {
				const size_t	readAmount( Min<size_t>( sizeInBytes, remainingData.Size() ) );
				
				CopyMemory( destination, remainingData.first, readAmount );

				remainingData.first += readAmount;
			}

			void Write( const void* /*source*/, ::asUINT /*sizeInBytes*/ ) override {
				ETRuntimeAssert( false );
			}

			Range<const char*>	remainingData;
		};

	// ---

		Reader	reader( bytes );

		if( ::asSUCCESS != _module->LoadByteCode( &reader ) ) {
			return Error::InvalidParameter;
		}

		if( !BindToModule( *_module, reader.remainingData ) ) {
			return Error::InvalidParameter;
		}

		PublishToLibraryAs<BytecodePackageResourceView>( library );

		return Error::None;
	}

// ---------------------------------------------------

	void BytecodePackageResourceView::DetachFromPackage( ContentPackage& /*package*/, ContentLibrary& library ) const {
		RemoveFromLibraryAs<BytecodePackageResourceView>( library );
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2