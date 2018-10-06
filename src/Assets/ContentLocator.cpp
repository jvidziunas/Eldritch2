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
		_pendingLoadCount(0u),
		_loadedPackages(MallocAllocator("Asynchronous Import Loaded Package List Allocator")) {}

	// ---------------------------------------------------

	AsynchronousImport::~AsynchronousImport() {
#if ETIsDebugBuild()
		const auto pendingLoads(_pendingLoadCount.load(std::memory_order_consume));
		ET_ASSERT(pendingLoads == 0, "Asynchronous import {} has {} pending load requests!", fmt::ptr(this), pendingLoads);
#endif
	}

	// ---------------------------------------------------

	bool AsynchronousImport::IsComplete(MemoryOrder order) const ETNoexceptHint {
		return _pendingLoadCount.load(order) == 0u;
	}

	// ---------------------------------------------------

	ErrorCode AsynchronousImport::InitiateLoad(PackageDatabase& packages, StringView path) {
		_pendingLoadCount.fetch_add(1u, std::memory_order_acquire);
		const ErrorCode result(packages.Load(path, *this));
		if (Failed(result)) {
			_pendingLoadCount.fetch_sub(1u, std::memory_order_release);
		}

		return result;
	}

	// ---------------------------------------------------

	void AsynchronousImport::CompleteLoad(CountedPointer<const Package> package, ErrorCode finalResult) ETNoexceptHint {
		if (Succeeded(finalResult)) {
			_loadedPackages.Append(eastl::move(package));
		}

		_pendingLoadCount.fetch_sub(1u, std::memory_order_release);
	}

	// ---------------------------------------------------

	ErrorCode ContentLocator::BindResources(const AssetDatabase& assets, PackageDatabase& packages) {
		_assets   = ETAddressOf(assets);
		_packages = ETAddressOf(packages);

		return Error::None;
	}

	// ---------------------------------------------------

	void ContentLocator::FreeResources() {
		_packages = nullptr;
		_assets   = nullptr;
	}

}} // namespace Eldritch2::Assets
