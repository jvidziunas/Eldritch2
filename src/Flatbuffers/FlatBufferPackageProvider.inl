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

namespace Eldritch2 { namespace FlatBuffers {

	ETConstexpr ETInlineHint ETForceInlineHint Assets::PackageDatabase& FlatBufferPackageProvider::GetPackageDatabase() ETNoexceptHint {
		return _packageDatabase;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint Assets::AssetDatabase& FlatBufferPackageProvider::GetAssetDatabase() ETNoexceptHint {
		return _assetDatabase;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool FlatBufferPackageProvider::ShouldRun(MemoryOrder order) const ETNoexceptHint {
		return _runBehavior.load(order) == Continue;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void FlatBufferPackageProvider::DestroyGarbage(size_t destructionLimit) {
		_packageDatabase.DestroyGarbage(_assetDatabase, destructionLimit);
	}

}} // namespace Eldritch2::FlatBuffers
