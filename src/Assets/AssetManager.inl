/*==================================================================*\
  AssetManager.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Assets/AssetDatabase.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Assets {
	namespace Detail {

		template <typename PublicAsset, bool checkCast>
		struct CastHelper {
			ETInlineHint static const Asset* Cast(const Asset* asset) {
				return FindLastInstance(asset->GetPath(), PublicAsset::GetExtension()) ? asset : nullptr;
			}
		};

		template <typename PublicAsset>
		struct CastHelper<PublicAsset, /*bool checkCast =*/false> {
			ETInlineHint static const Asset* Cast(const Asset* asset) {
				return asset;
			}
		};

	} // namespace Detail

	ETInlineHint AssetReference::AssetReference(const Asset& asset) :
		_asset(eastl::addressof(asset)) {}

	// ---------------------------------------------------

	ETInlineHint AssetManager::AssetManager(const AssetDatabase& assets) :
		_assets(eastl::addressof(assets)) {}

	// ---------------------------------------------------

	ETInlineHint const Asset* AssetManager::Find(const Utf8Char* path) const {
		return _assets->Find(path);
	}

	// ---------------------------------------------------

	template <typename PublicAsset, bool checkCast>
	ETInlineHint const PublicAsset* Cast(const AssetReference& asset) {
		return static_cast<const PublicAsset*>(Detail::CastHelper<PublicAsset, checkCast>::Cast(asset._asset));
	}

	// ---------------------------------------------------

	ETInlineHint void Swap(AssetReference& lhs, AssetReference& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._asset, rhs._asset);
	}

}} // namespace Eldritch2::Assets
