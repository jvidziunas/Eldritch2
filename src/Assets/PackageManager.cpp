/*==================================================================*\
  PackageManager.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Assets/PackageDatabase.hpp>
#include <Assets/PackageManager.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Assets {

		AsynchronousImport::AsynchronousImport(PackageList packages) : _packages(eastl::move(packages)) {}

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

		PackageManager::PackageManager(
			PackageDatabase& packages
		) : _packages(eastl::addressof(packages)),
			_imports(MallocAllocator("Script Asset Manager Import Collection Allocator")) {
		}

	// ---------------------------------------------------

		void PackageManager::ProcessTick() {}

	// ---------------------------------------------------

		CountedPointer<const Package> PackageManager::Load(const Utf8Char* path) {
			return _packages->Load(path);
		}

	// ---------------------------------------------------

		bool PackageManager::Require(const Utf8Char* path) {
			if (CountedPointer<const Package> package{ Load(path) }) {
				_imports.Append(eastl::move(package));
				return true;
			}

			return false;
		}

	// ---------------------------------------------------

		ErrorCode PackageManager::BindResources() {
			return Require("boot") ? Error::None : Error::InvalidFileName;
		}

	// ---------------------------------------------------

		void PackageManager::FreeResources() {
			_imports.Clear();
		}

	}	// namespace Assets
}	// namespace Eldritch2