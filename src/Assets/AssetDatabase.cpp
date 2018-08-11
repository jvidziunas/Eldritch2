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

		ETPureFunctionHint StringView<Utf8Char> FindExtension(StringView<Utf8Char> path) {
			return { FindLastInstance(path, '.'), path.End() };
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
	}

	// ---------------------------------------------------

	UniquePointer<Asset> AssetDatabase::Insert(const Package& package, StringView<Utf8Char> path) {
		const FactoryMap::ConstIterator candidate(_factoryByExtension.Find(FindExtension(path)));
		if (ET_UNLIKELY(candidate == _factoryByExtension.End())) {
			return nullptr;
		}

		UniquePointer<Asset> asset(candidate->second(_allocator, package, path));
		if (asset) {
			Lock _(_assetsMutex);
			_assets.Insert(asset.Get());
		} // End of lock scope.

		return eastl::move(asset);
	}

	// ---------------------------------------------------

	void AssetDatabase::Erase(Asset& asset) {
		Lock _(_assetsMutex);

		_assets.Erase(eastl::addressof(asset));
	}

	// ---------------------------------------------------

	ErrorCode AssetDatabase::BindResources(FactoryMap factories) {
		Swap(_factoryByExtension, factories);

		return Error::None;
	}

	// ---------------------------------------------------

	void AssetDatabase::FreeResources() {
		_factoryByExtension.Clear();
	}

}} // namespace Eldritch2::Assets
