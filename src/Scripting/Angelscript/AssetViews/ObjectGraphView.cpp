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
#include <Scripting/Angelscript/AssetViews/ObjectGraphView.hpp>
#include <Utility/Containers/Range.hpp>
#include <Assets/AssetLibrary.hpp>
#include <Utility/ErrorCode.hpp>
#include <Utility/Result.hpp>
//------------------------------------------------------------------//
#include <FlatBuffers/AngelscriptObjectGraph_generated.h>
//------------------------------------------------------------------//
#include <angelscript.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Assets;

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {
namespace AssetViews {

	ObjectGraphView::ObjectGraphView( const Utf8Char* const name ) : AssetView( name ) {}

// ---------------------------------------------------

	ErrorCode ObjectGraphView::InstantiateInto( WorldService& /*service*/ ) const {
		return Error::None;
	}

// ---------------------------------------------------

	Result<Eldritch2::UniquePointer<AssetView>> ObjectGraphView::CreateView( Allocator& allocator, const AssetLibrary& /*library*/, const Utf8Char* const name, Range<const char*> /*assetBytes*/ ) {
		auto result( MakeUnique<ObjectGraphView>( allocator, name ) );
		if( !result ) {
			return Error::OutOfMemory;
		}

		return eastl::move( result );
	}

// ---------------------------------------------------

	ETPureFunctionHint Utf8Literal ObjectGraphView::GetExtension() {
		return Utf8Literal( FlatBuffers::AngelscriptObjectGraphExtension() );
	}

}	// namespace AssetViews
}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2