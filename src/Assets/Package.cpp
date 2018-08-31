/*==================================================================*\
  Package.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Assets/Package.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Assets {

	Package::Package(StringView path) :
		_referenceCount(0u),
		_isLoaded(false),
		_assets(MallocAllocator("Package Asset List Allocator")) {
		path.Copy(_path, ETCountOf(_path));
	}

	// ---------------------------------------------------

	Package::~Package() {
		ET_ASSERT(!IsLoaded(), "Package contains assets! Call FreeAssets() before the package is destroyed!");
	}

	// ---------------------------------------------------

	void Package::BindAssets(AssetList assets) {
		ET_ASSERT(!IsLoaded(), "Package contains assets! Call FreeAssets() before binding a new asset list!");

		assets.ShrinkToFit();
		Swap(_assets, assets);
		_isLoaded.store(true, std::memory_order_release);
	}

	// ---------------------------------------------------

	Package::AssetList Package::FreeAssets() {
		_isLoaded.store(false, std::memory_order_acquire);
		return eastl::exchange(_assets, AssetList(_assets.GetAllocator()));
	}

}} // namespace Eldritch2::Assets
