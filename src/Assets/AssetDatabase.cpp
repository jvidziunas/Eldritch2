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

	AssetDatabase::AssetDatabase() :
		_allocator("Asset Root Allocator"),
		_assets(MallocAllocator("Asset Database Asset Bucket Allocator")),
		_factoryByExtension(MallocAllocator("Asset Database Factory Bucket Allocator")) {
	}

	// ---------------------------------------------------

	AssetDatabase::~AssetDatabase() {
		ET_ASSERT(_assets.IsEmpty(), "Leaking package references!");
	}

	// ---------------------------------------------------

	UniquePointer<Asset> AssetDatabase::Insert(const Package& owner, StringView path) {
		// Find a registered handler for the file type, keyed by file extension.
		// Note that this does include the leading extension separator character.
		const auto candidate(_factoryByExtension.Find(path.Substring(path.FindLast(L'.'))));
		if (ET_UNLIKELY(candidate == _factoryByExtension.End())) {
			return nullptr;
		}

		UniquePointer<Asset> asset(candidate->second(_allocator, owner, path));
		if (asset) {
			Lock _(_assetsMutex);
			_assets.Insert(asset.Get());
		} // End of lock scope.

		return eastl::move(asset);
	}

	// ---------------------------------------------------

	ErrorCode AssetDatabase::BindResources(FactoryMap factories) {
		Swap(_factoryByExtension, factories);
		return Error::None;
	}

	// ---------------------------------------------------

	void AssetDatabase::FreeResources() {
		_factoryByExtension.Clear();
		_assets.Clear();
	}

}} // namespace Eldritch2::Assets
