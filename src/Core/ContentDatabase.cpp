/*==================================================================*\
  ContentDatabase.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/ContentDatabase.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Core {

	ContentDatabase::ContentDatabase() ETNoexceptHint : _allocator("Asset Root Allocator"),
														_factoryByExtension(MallocAllocator("Asset Database Factory Map Allocator")),
														_assets(AssetSet::AllocatorType("Asset Database Asset Set Allocator")),
														_packages(PackageSet::AllocatorType("Package Database Package Set Allocator")),
														_gcCursor(_packages.Begin()),
														_requests(MallocAllocator("Package Request Queue Allocator")) {}

	// ---------------------------------------------------

	void ContentDatabase::DestroyGarbage(PackageSet::SizeType limit) {
		Package::AssetList tempAssets(Package::AssetList::AllocatorType("Package Asset List"));

		ET_READ_LOCK_SCOPE(_packagesMutex);
		GcCursor cursor(_gcCursor), end(_packages.End());
		for (limit = Minimum(limit, _packages.GetSize()); limit; --limit) {
			if (cursor->ShouldCollect()) {
				//	Use of const_cast is gross, but this is one of the suggested solutions in http://www.open-std.org/jtc1/sc22/wg21/docs/lwg-defects.html#103
				const_cast<Package&>(*cursor).BindAssets(FileTime::Unavailable, tempAssets);
				DestroyAssets(tempAssets);
			}

			if (++cursor == end) {
				//	Wrap the cursor if we've reached the end of the set.
				cursor = _packages.Begin();
			}
		}

		_gcCursor = cursor;
		// End of lock scope.
	}

	// ---------------------------------------------------

	Package::AssetHandle ContentDatabase::Insert(Logging::Log& log, StringSpan path, const AssetBuilder& builder) {
		/* Find a registered handler for the file type, keyed by file extension.
		 * Note that this substring includes the leading extension separator character, if present. */
		const auto candidate(_factoryByExtension.Find(path.Slice(path.FindLast(ET_EXTENSION_DELIMITER))));
		if (ET_UNLIKELY(candidate == _factoryByExtension.End())) {
			return nullptr;
		}

		UniquePointer<Asset> asset(candidate->second(_allocator, path));
		if (Failed(asset->BindResources(log, builder))) {
			return nullptr;
		}

		ET_LOCK_SCOPE(_assetsMutex);
		const auto result(_assets.Emplace(Move(asset)));
		return result.second ? result.first->Get() : nullptr;
		// End of lock scope.
	} 

	// ---------------------------------------------------

	Result ContentDatabase::BindResources(AssetApiBuilder api, PackageSet packages) {
		using ::Eldritch2::Swap;

		PackageSet::Iterator gcCursor(packages.Begin());
		{
			ET_LOCK_SCOPE(_requestsMutex);
			CancelAllRequests();
			Swap(_factoryByExtension, api.GetFactories());

			ET_LOCK_SCOPE(_packagesMutex);
			Swap(_packages, packages);
			Swap(_gcCursor, gcCursor);
		} // End of lock scope.

		// Destroy the previous loadable set.
		packages.ClearAndDispose([this](Package& package) {
			ETAssert(package.IsReferenced() == false, "Package {} has dangling references!", package.GetPath());
			Package::AssetList tempAssets(Package::AssetList::AllocatorType(""));

			package.BindAssets(FileTime::Unavailable, tempAssets);
			DestroyAssets(tempAssets);
		});

		return Result::Success;
	}

	// ---------------------------------------------------

	void ContentDatabase::FreeResources() ETNoexceptHint {
		BindResources(AssetApiBuilder(), PackageSet(_packages.GetAllocator()));
	}

	// ---------------------------------------------------

	void ContentDatabase::DestroyAssets(Package::AssetList& assets) {
		assets.ClearAndDispose([&](const Asset* asset) {
			ET_LOCK_SCOPE(_assetsMutex);
			_assets.Erase(asset, _assets.GetHash(), _assets.GetEqualityPredicate());
		});
	}

}} // namespace Eldritch2::Core
