/*==================================================================*\
  AssetDatabase.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Assets/AssetDatabase.hpp>
#include <Assets/Asset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Assets {
	namespace {

		ETPureFunctionHint Utf8Literal FindExtension(const Utf8Char* path) {
			return Utf8Literal(FindLastInstance(path, '.'));
		}

	} // anonymous namespace

	AssetDatabase::AssetDatabase() :
		_allocator("Asset Database Root Allocator"),
		_assets(MallocAllocator("Asset Database Asset Bucket Allocator")),
		_factoryByExtension(MallocAllocator("Asset Database Factory Bucket Allocator")) {
	}

	// ---------------------------------------------------

	AssetDatabase::~AssetDatabase() {
		ET_ASSERT(_assets.IsEmpty(), "Leaking package references!");

		FreeResources();
	}

	// ---------------------------------------------------

	UniquePointer<Asset> AssetDatabase::CreateAsset(const Utf8Char* path) {
		const ExtensionMap<AssetFactory>::ConstIterator candidate(_factoryByExtension.Find(FindExtension(path)));

		if (ET_UNLIKELY(candidate == _factoryByExtension.End())) {
			return nullptr;
		}

		UniquePointer<Asset> asset(candidate->second(_allocator, path));

		Insert(*asset);

		return eastl::move(asset);
	}

	// ---------------------------------------------------

	bool AssetDatabase::Insert(Asset& asset) {
		Lock _(_assetsMutex);

		return _assets.Insert(eastl::addressof(asset)).second;
	}

	// ---------------------------------------------------

	void AssetDatabase::Erase(Asset& asset) {
		Lock _(_assetsMutex);

		_assets.Erase(eastl::addressof(asset));
	}

	// ---------------------------------------------------

	ErrorCode AssetDatabase::BindResources(ExtensionMap<AssetFactory> factories) {
		Swap(_factoryByExtension, factories);

		return Error::None;
	}

	// ---------------------------------------------------

	void AssetDatabase::FreeResources() {
		_factoryByExtension.Clear();
	}

}} // namespace Eldritch2::Assets
