/*==================================================================*\
  PackageLocator.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Assets/PackageDatabase.hpp>
#include <Assets/PackageLocator.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Assets {

	AsynchronousImport::AsynchronousImport(PackageList packages) :
		_packages(eastl::move(packages)) {}

	// ---------------------------------------------------

	bool AsynchronousImport::IsComplete() const {
		for (const PackageList::ValueType& package : _packages) {
			if (!package->IsLoaded()) {
				return false;
			}
		}

		return true;
	}

	// ---------------------------------------------------

	PackageLocator::PackageLocator(
		PackageDatabase& packages) :
		_packages(eastl::addressof(packages)),
		_imports(MallocAllocator("Package Locator Imports List Allocator")) {
	}

	// ---------------------------------------------------

	void PackageLocator::PollLoadStatus() {}

	// ---------------------------------------------------

	ErrorCode PackageLocator::RequireAsynchronous(StringView<Utf8Char> path) {
		return _packages->Load(path, [this](CountedPointer<const Package> package, ErrorCode result) {
			if (Failed(result)) {
				return;
			}

			_imports.Append(eastl::move(package));
		});
	}

	// ---------------------------------------------------

	ErrorCode PackageLocator::Require(StringView<Utf8Char> path) {
		return _packages->Load(path, [this](CountedPointer<const Package> package, ErrorCode result) {
			if (Failed(result)) {
				return;
			}

			_imports.Append(eastl::move(package));
		});
	}

	// ---------------------------------------------------

	ErrorCode PackageLocator::BindResources() {
		static const StringView<Utf8Char> loadSet[] = { "boot" };

		for (StringView<Utf8Char> package : loadSet) {
			ET_FAIL_UNLESS(Require(package));
		}

		return Error::None;
	}

	// ---------------------------------------------------

	void PackageLocator::FreeResources() {
		_imports.Clear();
	}

}} // namespace Eldritch2::Assets
