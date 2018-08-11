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

	ErrorCode PackageDatabase::Load(StringView<Utf8Char> path, Function<void(CountedPointer<const Package>, ErrorCode)> callback) {
		CountedPointer<Package> package;
		{
			ReadLock                    _(_packagesMutex);
			const LoadableSet::Iterator candidate(_packages.Find(path, _packages.GetHashPredicate(), _packages.GetEqualityPredicate()));
			if (candidate == _packages.End()) {
				return Error::InvalidFileName;
			}

			/*	Use of const_cast is gross, but this is one of the suggested solutions in C++ Defect Report 103. We solemnly swear not to modify
			 *	the package path. See http://www.open-std.org/jtc1/sc22/wg21/docs/lwg-defects.html#103 */
			package = eastl::addressof(const_cast<Package&>(*candidate));
		} // End of lock scope.

		PushRequest({ eastl::move(callback), eastl::move(package) });

		return Error::None;
	}

	// ---------------------------------------------------

	ErrorCode PackageDatabase::BindResources(LoadableSet packages) {
		using ::Eldritch2::Swap;

		LoadableSet::Iterator gcCursor(packages.Begin());

		{
			Lock _(_requestsMutex);

			// Complete any outstanding load requests with a failure result.
			ClearRequests();

			{
				Lock _(_packagesMutex);

				Swap(_packages, packages);
				Swap(_gcCursor, gcCursor);
			} // End of lock scope.
		}     // End of lock scope.

		// Destroy the previous loadable set.
		packages.Clear();
	}

	// ---------------------------------------------------

	void PackageDatabase::FreeResources() {
		LoadableSet           emptySet(_packages.GetAllocator());
		LoadableSet::Iterator gcCursor(emptySet.Begin());
		Lock                  _(_requestsMutex);

		// Complete any outstanding requests with a failure result.
		ClearRequests();

		{
			Lock _(_packagesMutex);

			Swap(_packages, emptySet);
			Swap(_gcCursor, gcCursor);
		} // End of lock scope.
	}

	// ---------------------------------------------------

	void PackageDatabase::ClearRequests() {
		LoadRequest current;

		while (!_requests.IsEmpty()) {
			current = eastl::move(_requests.Top());
			_requests.Pop();

			current.callback(eastl::move(current.package), ErrorCode(Error::Unspecified));
		}
	}

}} // namespace Eldritch2::Assets
