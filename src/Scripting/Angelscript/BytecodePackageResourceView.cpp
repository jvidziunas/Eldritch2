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
#include <Utility/ErrorCode.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#include <angelscript.h>
//------------------------------------------------------------------//
#include <utility>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;
using namespace ::std;

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	void BytecodePackageResourceView::ModuleDeleter::operator()( ::asIScriptModule* const module ) {
		module->Discard();
	}

// ---------------------------------------------------

	BytecodePackageResourceView::BytecodePackageResourceView( ::asIScriptEngine& engine, const UTF8Char* const name, Allocator& allocator ) : ResourceView( name, allocator ),
																																			  BytecodeMetadata( allocator ),
																																			  _module( engine.GetModule( GetName().GetCharacterArray(), ::asGM_CREATE_IF_NOT_EXISTS ) ) {}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const BytecodePackageResourceView::GetSerializedDataTag() {
		return UTF8L("AngelscriptBytecodePackage");
	}

// ---------------------------------------------------

	ErrorCode BytecodePackageResourceView::UpdateFromByteStream( const Range<const char*> bytes ) {
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

		return (::asSUCCESS == _module->LoadByteCode( &reader )) ? BindToModule( *_module, reader.remainingData ) : Error::INVALID_PARAMETER;
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2