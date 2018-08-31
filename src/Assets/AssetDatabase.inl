/*==================================================================*\
  AssetDatabase.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Assets/Asset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Assets {

	ETInlineHint ETPureFunctionHint ETForceInlineHint bool AssetDatabase::AssetEqual::operator()(const Asset* asset, StringView path) const ETNoexceptHint {
		return asset->GetPath() == path;
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint ETForceInlineHint bool AssetDatabase::AssetEqual::operator()(const Asset* lhs, const Asset* rhs) const ETNoexceptHint {
		return lhs->GetPath() == rhs->GetPath();
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint ETForceInlineHint size_t AssetDatabase::AssetHash::operator()(StringView path, size_t seed) const ETNoexceptHint {
		return GetHashCode(path, seed);
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint ETForceInlineHint size_t AssetDatabase::AssetHash::operator()(const Asset* asset, size_t seed) const ETNoexceptHint {
		return GetHashCode(asset->GetPath(), seed);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint AssetApiBuilder::AssetApiBuilder() :
		_factoriesByExtension(MallocAllocator("Asset Database Factory Bucket Allocator")) {}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint AssetApiBuilder& AssetApiBuilder::DefineType(StringView extension, AssetFactory factory) {
		_factoriesByExtension.Emplace(eastl::move(extension), eastl::move(factory));
		return *this;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint AssetApiBuilder::FactoryMap& AssetApiBuilder::GetFactories() ETNoexceptHint {
		return _factoriesByExtension;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint const Asset* AssetDatabase::Find(StringView path) const ETNoexceptHint {
		ReadLock   _(_assetsMutex);
		const auto candidate(_assets.Find(path, _assets.GetHash(), _assets.GetEqualityPredicate()));
		return candidate != _assets.End() ? *candidate : nullptr;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint AssetDatabase::ResidentSet::SizeType AssetDatabase::Erase(Asset& asset) {
		Lock _(_assetsMutex);
		return _assets.Erase(ETAddressOf(asset));
	}

	// ---------------------------------------------------

	template <typename ConcreteAsset>
	ETInlineHint ETPureFunctionHint ETForceInlineHint AssetApiBuilder::AssetFactory AssetApiBuilder::DefaultFactory() ETNoexceptHint {
		static_assert(eastl::is_base_of<Asset, ConcreteAsset>::value, "Type must derive from Assets::Asset!");

		return [](Allocator& allocator, const Package& /*package*/, StringView path) ETNoexceptHint {
			return MakeUnique<ConcreteAsset>(allocator, path);
		};
	}

}} // namespace Eldritch2::Assets
