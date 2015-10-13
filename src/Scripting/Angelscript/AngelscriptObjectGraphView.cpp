/*==================================================================*\
  AngelscriptEngine.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Angelscript/AngelscriptObjectGraphView.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//
#include <angelscript.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Scripting {

	AngelscriptObjectGraphView::AngelscriptObjectGraphView( const Initializer& initializer, Allocator& allocator ) : ResourceView( initializer, allocator ) {}

// ---------------------------------------------------

	ErrorCode AngelscriptObjectGraphView::DeserializeIntoWorldView( AngelscriptWorldView& /*worldView*/ ) const {
		return Error::NONE;
	}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const AngelscriptObjectGraphView::GetSerializedDataTag() {
		return UTF8L("AngelscriptObjectGraph");
	}

}	// namespace Scripting
}	// namespace Eldritch2