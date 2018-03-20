/*==================================================================*\
  SpirVShaderPackageAsset.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {
namespace AssetViews {

	ETInlineHint typename SpirVShaderPackageAsset::UsageIterator<SpirVShaderPackageAsset::Usage> SpirVShaderPackageAsset::Begin() const {
		return _usages.Begin();
	}

// ---------------------------------------------------

	ETInlineHint typename SpirVShaderPackageAsset::UsageIterator<SpirVShaderPackageAsset::Usage> SpirVShaderPackageAsset::End() const {
		return _usages.End();
	}

// ---------------------------------------------------

	ETInlineHint typename SpirVShaderPackageAsset::UsageIterator<SpirVShaderPackageAsset::Usage> SpirVShaderPackageAsset::Find( const Utf8Char* name ) const {
		return _usages.Find( name, _usages.GetHashPredicate(), _usages.GetEqualityPredicate() );
	}

}	// namespace AssetViews
}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2