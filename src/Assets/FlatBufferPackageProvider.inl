/*==================================================================*\
  FlatBufferPackageProvider.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Assets {

	ETInlineHint PackageDatabase& FlatBufferPackageProvider::GetPackageDatabase() {
		return _packageDatabase;
	}

	// ---------------------------------------------------

	ETInlineHint AssetDatabase& FlatBufferPackageProvider::GetAssetDatabase() {
		return _assetDatabase;
	}

}} // namespace Eldritch2::Assets
