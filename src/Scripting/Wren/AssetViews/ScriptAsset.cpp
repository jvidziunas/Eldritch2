/*==================================================================*\
  ScriptAsset.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/AssetViews/ScriptAsset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace Wren {
namespace AssetViews {

	using namespace ::Eldritch2::Assets;

	ScriptAsset::ScriptAsset( const Utf8Char* const path ) : Asset( path ), _source( MallocAllocator( "Wren Script Asset Source Allocator" ) ) {}

// ---------------------------------------------------

	ETPureFunctionHint Utf8Literal ScriptAsset::GetExtension() {
		return ".wren";
	}

// ---------------------------------------------------

	ErrorCode ScriptAsset::BindResources( const Builder& asset ) {
		if (asset.GetSize() == 0u) {
			return Error::InvalidParameter;
		}

		String<> source( asset.Begin(), asset.End(), _source.GetAllocator() );

		Swap( _source, source );

		return Error::None;
	}

// ---------------------------------------------------

	void ScriptAsset::FreeResources() {
		_source.Clear();
	}

}	// namespace AssetViews
}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2