/*==================================================================*\
  GpuResourceApi.TileManager.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GpuResourceApi.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	TileManager::TileManager() :
		_residentTiles(MallocAllocator("Tile Manager GPU Resident Tile Set Allocator")),
		_tileExtent { 0u, 0u, 0u } {
	}

	// ---------------------------------------------------

	TileManager::TileManager(TileManager&& tileManager) :
		TileManager() {
		Swap(*this, tileManager);
	}

	// ---------------------------------------------------

	VkResult TileManager::BindResources(Gpu& /*gpu*/, VkExtent3D /*imageExtent*/, VkExtent3D tileExtent) {
		using ::Eldritch2::Swap;

		HashSet<Tile> residentTiles(_residentTiles.GetAllocator());

		Swap(_residentTiles, residentTiles);
		Swap(_tileExtent, tileExtent);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void TileManager::FreeResources(Gpu& gpu) {
		_tileExtent = { 0u, 0u, 0u };
		_residentTiles.Clear();
	}

	// ---------------------------------------------------

	void Swap(TileManager& lhs, TileManager& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._residentTiles, rhs._residentTiles);
		Swap(lhs._tileExtent, rhs._tileExtent);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
