/*==================================================================*\
  ContentLocator.inl
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

		template <typename PublicAsset, bool isChecked>
		struct CastHelper {
			static ETInlineHint ETForceInlineHint ETPureFunctionHint const Asset* Cast(const Asset* asset) ETNoexceptHint {
				return asset->GetPath().EndsWith(PublicAsset::GetExtension()) ? asset : nullptr;
			}
		};

		template <typename PublicAsset>
		struct CastHelper<PublicAsset, /*bool isChecked =*/false> {
			static ETInlineHint ETForceInlineHint ETPureFunctionHint const Asset* Cast(const Asset* asset) ETNoexceptHint {
				return asset;
			}
		};

	} // namespace Detail

	ETInlineHint ETForceInlineHint AssetReference::AssetReference(const Asset& asset) ETNoexceptHint : _asset(ETAddressOf(asset)) {}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint ContentLocator::ContentLocator() ETNoexceptHint : _assets(nullptr), _packages(nullptr) {}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint const Asset* ContentLocator::FindAsset(StringView path) const ETNoexceptHint {
		return _assets->Find(path);
	}

	// ---------------------------------------------------

	template <typename PublicAsset, bool isChecked>
	ETInlineHint ETForceInlineHint const PublicAsset* Cast(const AssetReference& asset) ETNoexceptHint {
		return static_cast<const PublicAsset*>(Detail::CastHelper<PublicAsset, isChecked>::Cast(asset._asset));
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void Swap(AssetReference& lhs, AssetReference& rhs) ETNoexceptHint {
		using ::Eldritch2::Swap;

		Swap(lhs._asset, rhs._asset);
	}

}} // namespace Eldritch2::Assets
