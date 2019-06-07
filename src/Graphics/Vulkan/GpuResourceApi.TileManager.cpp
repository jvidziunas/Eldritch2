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

	TileManager::TileManager() ETNoexceptHint : _residentTiles(ResidentSet::AllocatorType("Tile Manager Resident Tile Set Allocator")), _tileExtent{ 0u, 0u, 0u } {}

	// ---------------------------------------------------

	TileManager::TileManager(TileManager&& tileManager) ETNoexceptHint : TileManager() {
		Swap(*this, tileManager);
	}

	// ---------------------------------------------------

	VkResult TileManager::BindResources(Gpu& /*gpu*/, VkExtent3D imageExtent, VkExtent3D tileExtent) {
		ETAssert(imageExtent.width < MaxImageDimension, "tiled image width {} must be < {}", imageExtent.width, uint32(MaxImageDimension));
		ETAssert(imageExtent.height < MaxImageDimension, "tiled image height {} must be < {} (was {})", imageExtent.height, uint32(MaxImageDimension));
		ETAssert(imageExtent.depth < MaxImageDimension, "tiled image depth {} must be < {} (was {})", imageExtent.depth, uint32(MaxImageDimension));

		using ::Eldritch2::Swap;

		ResidentSet residentTiles(_residentTiles.GetAllocator());

		Swap(_residentTiles, residentTiles);
		Swap(_tileExtent, tileExtent);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void TileManager::FreeResources(Gpu& gpu) ETNoexceptHint {
		_tileExtent = { 0u, 0u, 0u };
		_residentTiles.Clear();
	}

	// ---------------------------------------------------

	void Swap(TileManager& lhs, TileManager& rhs) ETNoexceptHint {
		using ::Eldritch2::Swap;

		Swap(lhs._residentTiles, rhs._residentTiles);
		Swap(lhs._tileExtent, rhs._tileExtent);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
