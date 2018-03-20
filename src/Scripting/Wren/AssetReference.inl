/*==================================================================*\
  AssetReference.inl
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

	template <typename Asset>
	ETInlineHint AssetReference<Asset>::AssetReference( const Asset& asset ) : _asset( &asset ) {}

// ---------------------------------------------------

	template <typename Asset>
	ETInlineHint AssetReference<Asset>::operator const Asset&() const {
		return *_asset;
	}

// ---------------------------------------------------

	template <typename Asset>
	ETInlineHint const Asset& AssetReference<Asset>::Get() const {
		return *_asset;
	}

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2