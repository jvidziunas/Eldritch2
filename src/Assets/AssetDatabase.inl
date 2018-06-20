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

	ETInlineHint ETPureFunctionHint bool AssetDatabase::AssetEqual::operator()(const Asset* asset, const Utf8Char* path) const {
		return StringsEqual(asset->GetPath(), path);
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint bool AssetDatabase::AssetEqual::operator()(const Asset* lhs, const Asset* rhs) const {
		return lhs == rhs || StringsEqual(lhs->GetPath(), rhs->GetPath());
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint size_t AssetDatabase::AssetHash::operator()(const Utf8Char* path, size_t seed) const {
		return HashMemory(path, StringLength(path), seed);
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint size_t AssetDatabase::AssetHash::operator()(const Asset* asset, size_t seed) const {
		return HashMemory(asset->GetPath(), StringLength(asset->GetPath()), seed);
	}

	// ---------------------------------------------------

	ETInlineHint AssetApiBuilder::AssetApiBuilder() :
		_factoriesByExtension(MallocAllocator("Asset Database Factory Bucket Allocator")) {}

	// ---------------------------------------------------

	ETInlineHint AssetApiBuilder& AssetApiBuilder::DefineType(Utf8Literal extension, AssetFactory factory) {
		_factoriesByExtension.Emplace(eastl::move(extension), eastl::move(factory));

		return *this;
	}

	// ---------------------------------------------------

	template <typename Type>
	ETInlineHint ETPureFunctionHint AssetApiBuilder::AssetFactory AssetApiBuilder::DefaultFactory() {
		static_assert(eastl::is_base_of<Asset, Type>::value, "Type must derive from Assets::Asset!");

		return [](Allocator& allocator, const Utf8Char* path) {
			return MakeUnique<Type>(allocator, path);
		};
	}

	// ---------------------------------------------------

	ETInlineHint AssetApiBuilder::ExtensionMap<AssetApiBuilder::AssetFactory>& AssetApiBuilder::GetFactories() {
		return _factoriesByExtension;
	}

	// ---------------------------------------------------

	ETInlineHint const Asset* AssetDatabase::Find(const Utf8Char* const path) const {
		ReadLock _(_assetsMutex);

		const auto candidate(_assets.Find(path, _assets.GetHashPredicate(), _assets.GetEqualityPredicate()));

		return candidate != _assets.End() ? *candidate : nullptr;
	}

}} // namespace Eldritch2::Assets
