/*==================================================================*\
  ContentLocator.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Assets/PackageDatabase.hpp>
#include <Assets/ContentLocator.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Assets {

	AsynchronousImport::AsynchronousImport() :
		_pendingLoads(0u),
		_loadedPackages(MallocAllocator("Asynchronous Import Loaded Package List Allocator")) {}

	// ---------------------------------------------------

	bool AsynchronousImport::IsComplete(MemoryOrder order) const {
		return _pendingLoads.load(order) == 0u;
	}

	// ---------------------------------------------------

	void AsynchronousImport::CompleteLoad(CountedPointer<const Package> package, ErrorCode finalResult) ETNoexceptHint {
		if (Succeeded(finalResult)) {
			_loadedPackages.Append(eastl::move(package));
		}

		_pendingLoads.fetch_sub(1u, std::memory_order_release);
	}

	// ---------------------------------------------------

	ContentLocator::ContentLocator(const AssetDatabase& assets, PackageDatabase& packages) ETNoexceptHint : _assets(ETAddressOf(assets)),
																											_packages(ETAddressOf(packages)),
																											_loadedPackages(MallocAllocator("Content Locator Imports List Allocator")) {
	}

	// ---------------------------------------------------

	void ContentLocator::RequirementsComplete(MemoryOrder order) const {
		return _pendingLoads.load(order) == 0u;
	}

	// ---------------------------------------------------

	ErrorCode ContentLocator::RequirePackageAsynchronous(StringView path, PackageLoadClient& client) {
		return _packages->Load(path, client);
	}

	// ---------------------------------------------------

	ErrorCode ContentLocator::BindResources() {
		static ETConstexpr StringView loadSet[] = { "boot" };

		for (StringView package : loadSet) {
			_pendingLoads.fetch_add(1u, std::memory_order_acquire);
			ET_ABORT_UNLESS(RequirePackageAsynchronous(package, *this));
		}

		return Error::None;
	}

	// ---------------------------------------------------

	void ContentLocator::CompleteLoad(CountedPointer<const Package> package, ErrorCode finalResult) ETNoexceptHint {
		if (Succeeded(finalResult)) {
			_loadedPackages.Append(eastl::move(package));
		}

		_pendingLoads.fetch_sub(1u, std::memory_order_release);
	}

	// ---------------------------------------------------

	void ContentLocator::FreeResources() {
		_loadedPackages.Clear();
	}

}} // namespace Eldritch2::Assets
