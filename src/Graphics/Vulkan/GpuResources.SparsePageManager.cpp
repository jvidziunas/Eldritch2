/*==================================================================*\
  GpuResources.SparsePageManager.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GpuResources.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/GpuHeap.hpp>
#include <Graphics/ImageSource.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {
namespace {

	ETInlineHint VkImageType GetImageType( const ImageSource::Dimensions& dimensions ) {
		if (dimensions.depthInTexels > 1u) {
			return VK_IMAGE_TYPE_3D;
		}

		if (dimensions.heightInTexels > 1u) {
			return VK_IMAGE_TYPE_2D;
		}

		return VK_IMAGE_TYPE_1D;
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint VkExtent3D CalculateExtentInTiles( VkExtent3D extentInTexels, VkExtent3D logTileExtentInTexels ) {
		return { extentInTexels.width >> logTileExtentInTexels.width, extentInTexels.height >> logTileExtentInTexels.height, extentInTexels.depth >> logTileExtentInTexels.depth };
	}

// ---------------------------------------------------

	static ETInlineHint ETPureFunctionHint VkImageCreateInfo GetCreateInfo( const ImageSource::Dimensions& source ) {
		return VkImageCreateInfo{
			VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			nullptr,
			VK_IMAGE_CREATE_SPARSE_BINDING_BIT | VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT,
			GetImageType( source ),
			VK_FORMAT_UNDEFINED,
			VkExtent3D{ source.widthInTexels, source.heightInTexels, source.depthInTexels },
			source.mips,
			1u,						// Array layers
			VK_SAMPLE_COUNT_1_BIT,	// Standard resources are always 1 sample/texel.
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
			VK_SHARING_MODE_EXCLUSIVE,
			0u, nullptr,			// Exclusive sharing.
			VK_IMAGE_LAYOUT_UNDEFINED
		};
	}

}	// anonymous namespace

	SparsePageManager::SparsePageManager(
	) : _freeTiles( { "Vulkan Sparse Image Page Manager Free Tile Collection Allocator" } ),
		_logTileExtent( { 1u, 1u, 1u } ),
		_imageExtent( { 0u, 0u, 0u } ),
		_backing( nullptr ),
		_image( nullptr ) {}

// ---------------------------------------------------

	SparsePageManager::SparsePageManager(
		SparsePageManager&& pageManager
	) :	_freeTiles( { "Vulkan Sparse Image Page Manager Free Tile Collection Allocator" } ),
		_logTileExtent( eastl::exchange( pageManager._logTileExtent, VkExtent3D{ 1u, 1u, 1u } ) ),
		_imageExtent( eastl::exchange( pageManager._imageExtent, VkExtent3D{ 0u, 0u, 0u } ) ),
		_backing( eastl::exchange( pageManager._backing, nullptr ) ),
		_image( eastl::exchange( pageManager._image, nullptr ) ) {}

// ---------------------------------------------------

	SparsePageManager::~SparsePageManager() {
		ET_ASSERT( _backing == nullptr,	"Leaking Vulkan allocation!" );
		ET_ASSERT( _image == nullptr,	"Leaking Vulkan image!" );
	}

// ---------------------------------------------------

	VkResult SparsePageManager::BindResources( GpuHeap& heap, VkExtent3D tileExtent, const ImageSource& source ) {
		const VkImageCreateInfo	imageInfo( GetCreateInfo( source.GetDimensions() ) );

		if (imageInfo.extent.width >= MaxImageDimension || imageInfo.extent.height >= MaxImageDimension || imageInfo.extent.depth >= MaxImageDimension) {
			return VK_ERROR_FORMAT_NOT_SUPPORTED;
		}
		
		VmaAllocation backing;
		{	const VkMemoryRequirements	requirements{
				0u,
				0u,
				0u
			};

			const VmaAllocationCreateInfo allocationInfo{
				0u,
				VMA_MEMORY_USAGE_GPU_ONLY,
				0u, 0u, VK_NULL_HANDLE, nullptr
			};
			ET_FAIL_UNLESS( vmaAllocateMemory( heap, &requirements, &allocationInfo, &backing, nullptr ) );
		}
		ET_AT_SCOPE_EXIT( vmaFreeMemory( heap, backing ) );

		_logTileExtent	= VkExtent3D{ Log2( tileExtent.width ), Log2( tileExtent.height ), Log2( tileExtent.depth ) };
		_imageExtent	= imageInfo.extent;
		_source			= &source;

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	void SparsePageManager::FreeResources( GpuHeap& heap ) {
		if (eastl::exchange( _source, nullptr )) {
			heap.AddGarbage( { eastl::exchange( _image, nullptr ), eastl::exchange( _backing, nullptr ) } );
		}	
	}

// ---------------------------------------------------

	void Swap( SparsePageManager& manager0, SparsePageManager& manager1 ) {
		using ::Eldritch2::Swap;

		Swap( manager0._freeTiles,		manager1._freeTiles );
		Swap( manager0._logTileExtent,	manager1._logTileExtent );
		Swap( manager0._imageExtent,	manager1._imageExtent );
		Swap( manager0._backing,		manager1._backing );
		Swap( manager0._image,			manager1._image );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2