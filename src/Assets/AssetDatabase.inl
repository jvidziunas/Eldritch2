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

	ETInlineHint ETPureFunctionHint bool AssetDatabase::AssetEqual::operator()(const Asset* asset, StringView<Utf8Char> path) const {
		return asset->GetPath() == path;
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint bool AssetDatabase::AssetEqual::operator()(const Asset* lhs, const Asset* rhs) const {
		return lhs->GetPath() == rhs->GetPath();
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint size_t AssetDatabase::AssetHash::operator()(StringView<Utf8Char> path, size_t seed) const {
		return GetHashCode(path, seed);
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint size_t AssetDatabase::AssetHash::operator()(const Asset* asset, size_t seed) const {
		return GetHashCode(asset->GetPath(), seed);
	}

	// ---------------------------------------------------

	ETInlineHint AssetApiBuilder::AssetApiBuilder() :
		_factoriesByExtension(MallocAllocator("Asset Database Factory Bucket Allocator")) {}

	// ---------------------------------------------------

	ETInlineHint AssetApiBuilder& AssetApiBuilder::DefineType(StringView<Utf8Char> extension, AssetFactory factory) {
		_factoriesByExtension.Emplace(eastl::move(extension), eastl::move(factory));
		return *this;
	}

	// ---------------------------------------------------

	template <typename Type>
	ETInlineHint ETPureFunctionHint AssetApiBuilder::AssetFactory AssetApiBuilder::DefaultFactory() {
		static_assert(eastl::is_base_of<Asset, Type>::value, "Type must derive from Assets::Asset!");

		return [](Allocator& allocator, const Package& /*package*/, StringView<Utf8Char> path) {
			return MakeUnique<Type>(allocator, path);
		};
	}

	// ---------------------------------------------------

	ETInlineHint AssetApiBuilder::FactoryMap& AssetApiBuilder::GetFactories() {
		return _factoriesByExtension;
	}

	// ---------------------------------------------------

	ETInlineHint const Asset* AssetDatabase::Find(StringView<Utf8Char> path) const {
		ReadLock _(_assetsMutex);

		const auto candidate(_assets.Find(path, _assets.GetHashPredicate(), _assets.GetEqualityPredicate()));
		return candidate != _assets.End() ? *candidate : nullptr;
	}

}} // namespace Eldritch2::Assets
