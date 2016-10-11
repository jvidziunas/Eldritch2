/*==================================================================*\
  SparseImage.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/SparseImage.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

#if 0
	VkExtent3D SparseImagePageManager::CalculateTileSize3D( VkPhysicalDevice device, VkDeviceSize granularityInBytes, VkFormat format ) {

	}
#endif

// ---------------------------------------------------

	VkExtent3D SparseImagePageManager::CalculateExtent2D( VkPhysicalDevice device, VkDeviceSize granularityInBytes, VkFormat format ) {
		VkSparseImageFormatProperties	properties;
		uint32_t						propertyCount( 1u );

		vkGetPhysicalDeviceSparseImageFormatProperties( device,
														format,
														VkImageType::VK_IMAGE_TYPE_2D,
														VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT,
														VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT,
														VkImageTiling::VK_IMAGE_TILING_OPTIMAL,
														&propertyCount,
														&properties );

		return properties.imageGranularity;
	}

// ---------------------------------------------------

#if 0
	VkExtent3D SparseImagePageManager::CalculateTileSize1D( VkPhysicalDevice device, VkDeviceSize granularityInBytes, VkFormat format ) {

	}
#endif

// ---------------------------------------------------

	SparseImage::SparseImage( Eldritch2::UniquePointer<SparseImagePageManager> pageManager, Eldritch2::Allocator& allocator ) : _pageManager( eastl::move( pageManager ) ), _translationTable( { allocator, "Sparse Image TLB Allocator" } ) {}

// ---------------------------------------------------

	SparseImage::SparseImage( const SparseImage& ) {
		ETRuntimeAssert( false );
	}

// ---------------------------------------------------

	SparseImage::SparseImage( SparseImage&& image ) : _pageManager( eastl::move( image._pageManager ) ), _translationTable( eastl::move( image._translationTable ) ) {}

// ---------------------------------------------------

	bool SparseImage::IsResident( SparseImageVirtualCoordinate tileCoordinates ) const {
		return _translationTable.Find( tileCoordinates ) == _translationTable.ConstEnd();
	}

// ---------------------------------------------------

	bool SparseImage::MakeResident( SparseImageVirtualCoordinate tileCoordinate ) {
		auto	candidate( _translationTable.Find( tileCoordinate ) );

	//	Check to see if we have not already had a request for this tile previously.
		if( candidate != _translationTable.End() ) {
		//	If we have, increment the reference count and return; this is the best case.
			++candidate->second.referenceCount;
			return true;
		}

	//	Try to reserve a spot in the backing memory pool. We will finally commit the tile once we have generated/loaded data to fill it.
		const auto	physicalCoordinate( _pageManager->Reserve( tileCoordinate ) );

	//	The page manager returns the invalid/sentinel value if it was unable to reserve a tile.
		if( physicalCoordinate == SparseImagePageManager::InvalidPhysicalCoordinate ) {
			return false;
		}

	//	Record the tile mapping.
		_translationTable.Emplace( tileCoordinate, physicalCoordinate );
		return true;
	}

// ---------------------------------------------------

	bool SparseImage::MakeResident( VkExtent3D offsetInTexels, VkExtent3D regionInTexels ) {
		return false;
	}

// ---------------------------------------------------

	void SparseImage::Evict( SparseImageVirtualCoordinate tileCoordinate ) {
		auto	candidate( _translationTable.Find( tileCoordinate ) );

	/*	Handle the case where the initial residency request fell through.
	 *	This makes the function idempotent */
		if( candidate == _translationTable.End() ) {
			return;
		}

	//	Retain the mapping if there are still external references to the tile.
		if( 0u != --candidate->second.referenceCount ) {
			return;
		}

	//	Release device resources, if necessary, and finally remove the mapping entry from the translation table.
		_pageManager->Release( candidate->second.coordinate );
		_translationTable.Erase( candidate );
	}

// ---------------------------------------------------

	void SparseImage::Evict( VkExtent3D offsetInTexels, VkExtent3D regionInTexels ) {}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

