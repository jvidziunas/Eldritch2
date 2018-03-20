/*==================================================================*\
  ScriptAsset.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace Wren {
namespace AssetViews {

	ETInlineHint const Utf8Char* ScriptAsset::Begin() const {
		return _source.Begin();
	}

// ---------------------------------------------------

	ETInlineHint const Utf8Char* ScriptAsset::End() const {
		return _source.End();
	}

// ---------------------------------------------------

	ETInlineHint size_t ScriptAsset::GetLength() const {
		return _source.GetLength();
	}

}	// namespace AssetViews
}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2