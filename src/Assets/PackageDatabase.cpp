/*==================================================================*\
  PackageDatabase.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Assets/PackageDatabase.hpp>
#include <Assets/AssetDatabase.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Assets {

	PackageDatabase::PackageDatabase() :
		_packages(MallocAllocator("Asset Database Package Bucket Allocator")),
		_gcCursor(_packages.Begin()) {
	}

	// ---------------------------------------------------

	void PackageDatabase::DestroyGarbage(AssetDatabase& assets, size_t destructionLimit) {
		ReadLock _(_packagesMutex);
		GcCursor cursor(_gcCursor), end(_packages.End());

		for (; destructionLimit != 0u && cursor != end; ++cursor) {
			if (cursor->IsReferenced()) {
				continue;
			}

			/*	Use of const_cast is gross, but this is one of the suggested solutions in C++ Defect Report 103. We solemnly swear not to modify
			 *	the package path. See http://www.open-std.org/jtc1/sc22/wg21/docs/lwg-defects.html#103 */
			for (UniquePointer<Asset>& asset : const_cast<Package&>(*cursor).FreeAssets()) {
				assets.Erase(*asset);
			}

			--destructionLimit;
		}

		//	Wrap the cursor if we've reached the end of the set.
		_gcCursor = (cursor != end) ? cursor : _packages.Begin();
	}

	// ---------------------------------------------------

	ErrorCode PackageDatabase::Load(StringView path, PackageLoadClient& client) {
		CountedPointer<Package> package;
		{
			ReadLock   _(_packagesMutex);
			const auto candidate(_packages.Find(path, _packages.GetHash(), _packages.GetEqualityPredicate()));
			if (ET_UNLIKELY(candidate == _packages.End())) {
				return Error::InvalidFileName;
			}

			/*	Use of const_cast is gross, but this is one of the suggested solutions in C++ Defect Report 103. We solemnly swear not to modify
			 *	the package path. See http://www.open-std.org/jtc1/sc22/wg21/docs/lwg-defects.html#103 */
			package = ETAddressOf(const_cast<Package&>(*candidate));
		} // End of lock scope.

		if (package->IsLoaded()) {
			client.CompleteLoad(eastl::move(package), Error::None);
		} else {
			PushRequest({ ETAddressOf(client), eastl::move(package) });
		}

		return Error::None;
	}

	// ---------------------------------------------------

	ErrorCode PackageDatabase::BindResources(LoadableSet packages) {
		using ::Eldritch2::Swap;

		LoadableSet::Iterator gcCursor(packages.Begin());
		{
			// Complete any outstanding load requests with a failure result.
			Lock _(_requestsMutex);
			ClearRequests();
			{
				Lock _1(_packagesMutex);
				Swap(_packages, packages);
				Swap(_gcCursor, gcCursor);
			} // End of lock scope.
		}     // End of lock scope.

		// Destroy the previous loadable set.
		packages.Clear();

		return Error::None;
	}

	// ---------------------------------------------------

	void PackageDatabase::FreeResources() {
		LoadableSet emptySet(_packages.GetAllocator());
		auto        gcCursor(emptySet.Begin());
		Lock        _(_requestsMutex);

		// Complete any outstanding requests with a failure result.
		ClearRequests();
		{
			Lock _1(_packagesMutex);

			Swap(_packages, emptySet);
			Swap(_gcCursor, gcCursor);
		} // End of lock scope.
	}

}} // namespace Eldritch2::Assets
