/*==================================================================*\
  AngelscriptBytecodePackageView.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Angelscript/AngelscriptBytecodePackageView.hpp>
#include <Utility/Memory/StandardLibrary.hpp>
#include <Utility/Math/StandardLibrary.hpp>
#include <Utility/MessagePackReader.hpp>
#include <Utility/ErrorCode.hpp>
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

	void AngelscriptBytecodePackageView::ModuleDeleter::operator()( ::asIScriptModule* const module ) {
		module->Discard();
	}

// ---------------------------------------------------

	AngelscriptBytecodePackageView::AngelscriptBytecodePackageView( const Initializer& initializer, Allocator& allocator ) : ResourceView( initializer, allocator ), _metadata( allocator ) {}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const AngelscriptBytecodePackageView::GetSerializedDataTag() {
		return UTF8L("AngelscriptBytecodePackage");
	}

// ---------------------------------------------------

	ErrorCode AngelscriptBytecodePackageView::InstantiateFromByteArray( const Range<const char*>& sourceBytes, ::asIScriptEngine& engine ) {
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

		decltype(_module)	newModule( engine.GetModule( GetName().GetCharacterArray(), ::asGM_CREATE_IF_NOT_EXISTS ) );
		Reader				reader( sourceBytes );

		if( newModule && newModule->LoadByteCode( &reader ) && MessagePackReader( reader.remainingData )( _metadata ) && _metadata.BindToModule( *newModule ) ) {
			_module = move( newModule );
			return Error::NONE;
		}

		return Error::INVALID_PARAMETER;
	}

}	// namespace Scripting
}	// namespace Eldritch2