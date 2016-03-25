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
#include <Scripting/Angelscript/ObjectGraphResourceView.hpp>
#include <Utility/Containers/Range.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//
#include <angelscript.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	ObjectGraphResourceView::ObjectGraphResourceView( const UTF8Char* const name ) : ResourceView( name ) {}

// ---------------------------------------------------

	ErrorCode ObjectGraphResourceView::DeserializeIntoWorldView( WorldView& /*worldView*/ ) const {
		return Error::None;
	}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const ObjectGraphResourceView::GetSerializedDataTag() {
		return UTF8L("AngelscriptObjectGraph");
	}

// ---------------------------------------------------

	ErrorCode ObjectGraphResourceView::AttachToPackage( const Range<const char*> /*bytes*/, ContentPackage& /*package*/, ContentLibrary& library ) {
		PublishToLibraryAs<ObjectGraphResourceView>( library );

		return Error::None;
	}

// ---------------------------------------------------

	void ObjectGraphResourceView::DetachFromPackage( ContentPackage& /*package*/, ContentLibrary& library ) const {
		RemoveFromLibraryAs<ObjectGraphResourceView>( library );
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2