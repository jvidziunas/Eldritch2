/*==================================================================*\
  GpuResources.TileManager.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GpuResources.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	TileManager::TileManager(
		VkFormat   format,
		VkExtent3D tileExtent,
		VkExtent3D imageExtent) :
		_residentTiles(MallocAllocator("Tile Manager GPU Resident Tile Set Allocator")),
		_logTileExtent(tileExtent),
		_imageExtent(imageExtent) {
	}

	// ---------------------------------------------------

	TileManager::TileManager(TileManager&& tileManager) :
		TileManager() {
		Swap(*this, tileManager);
	}

	// ---------------------------------------------------

	TileManager::TileManager() :
		TileManager(VK_FORMAT_UNDEFINED, VkExtent3D{ 1u, 1u, 1u }, VkExtent3D{ 1u, 1u, 1u }) {}

	// ---------------------------------------------------

	void Swap(TileManager& lhs, TileManager& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._logTileExtent, rhs._logTileExtent);
		Swap(lhs._imageExtent, rhs._imageExtent);
		Swap(lhs._residentTiles, rhs._residentTiles);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
