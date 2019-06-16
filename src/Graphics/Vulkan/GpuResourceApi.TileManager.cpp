/*==================================================================*\
  GpuResourceApi.TileManager.cpp
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
#include <Graphics/Vulkan/GpuResourceApi.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	TileManager::TileManager() ETNoexceptHint : Cache<ImageTile, VkDeviceSize>(AllocatorType("Tile Manager Resident Tile Set Allocator")), _tileExtent{ 0u, 0u, 0u } {}

	// ---------------------------------------------------

	TileManager::TileManager(TileManager&& tileManager) ETNoexceptHint : TileManager() {
		Swap(*this, tileManager);
	}

	// ---------------------------------------------------

	VkResult TileManager::BindResources(VkExtent3D imageExtent, VkExtent3D tileExtent) {
		using ::Eldritch2::Swap;

		ETAssert(imageExtent.width < ImageTile::MaxDimension, "tiled image width {} must be < {}", imageExtent.width, uint32(ImageTile::MaxDimension));
		ETAssert(imageExtent.height < ImageTile::MaxDimension, "tiled image height {} must be < {} (was {})", imageExtent.height, uint32(ImageTile::MaxDimension));
		ETAssert(imageExtent.depth < ImageTile::MaxDimension, "tiled image depth {} must be < {} (was {})", imageExtent.depth, uint32(ImageTile::MaxDimension));

		Swap(_tileExtent, tileExtent);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void TileManager::FreeResources() ETNoexceptHint {
		_tileExtent = { 0u, 0u, 0u };
		Clear();
	}

	// ---------------------------------------------------

	void Swap(TileManager& lhs, TileManager& rhs) ETNoexceptHint {
		using ::Eldritch2::Swap;

		Swap(lhs._tileExtent, rhs._tileExtent);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
