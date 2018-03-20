/*==================================================================*\
  GpuResources.ShaderImage.cpp
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
#include <Graphics/Vulkan/IoBuilder.hpp>
#include <Graphics/Vulkan/GpuHeap.hpp>
#include <Graphics/ImageSource.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {
namespace {

	enum : VkMemoryPropertyFlags {
		InferMemoryPropertiesFromUsage = 0u
	};

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint VkExtent3D GetMipExtent( const ImageSource::Dimensions& baseDimensions, uint32 mip = 0u ) {
		return VkExtent3D{ Max( baseDimensions.widthInTexels >> mip, 1u ), Max( baseDimensions.heightInTexels >> mip, 1u ), Max( baseDimensions.depthInTexels >> mip, 1u ) };
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint VkImageCreateInfo GetCreateInfo( const ImageSource::Dimensions& source ) {
		return VkImageCreateInfo{
			VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			nullptr,
			0u,						// Standard flags-- VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT?
			VK_IMAGE_TYPE_2D,
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

	ShaderImage::ShaderImage() : _backing( nullptr ), _image( nullptr ) {}

// ---------------------------------------------------

	ShaderImage::ShaderImage(
		ShaderImage&& image
	) : _backing( eastl::exchange( image._backing, nullptr ) ),
		_image( eastl::exchange( image._image, nullptr ) ) {}

// ---------------------------------------------------

	ShaderImage::~ShaderImage() {
		ET_ASSERT( _backing == nullptr, "Leaking Vulkan memory allocation!" );
		ET_ASSERT( _image == nullptr,   "Leaking Vulkan image!" );
	}

// ---------------------------------------------------

	VkResult ShaderImage::Upload( IoBuilder& ioBuilder, MipIndex firstMip, MipIndex lastMip, ArraySlice firstSlice, ArraySlice lastSlice ) {
		const ImageSource::Dimensions	dimensions( _source->GetDimensions() );
		const uint32					bitsPerPixel( 32u );

		lastSlice	= Min<ArraySlice>( dimensions.sliceCount, lastSlice );
		lastMip		= Min<MipIndex>( dimensions.mips, lastMip );

		for (MipIndex mip( firstMip ); mip < lastMip; ++mip) {
			const VkExtent3D	extent( GetMipExtent( dimensions, mip ) );
			const uint32		scanlineStride( extent.width * bitsPerPixel / CHAR_BIT );

			for (ArraySlice slice( firstSlice ); slice < lastSlice; ++slice) {
				ImageSource::StreamRequest request;

				request.scanlineStrideInBytes = scanlineStride;
				request.sliceStrideInBytes	  = scanlineStride * extent.height;
				request.mip                   = mip;
				request.arraySlice            = slice;
				request.target                = nullptr;

				ioBuilder.TransferToGpu(
					_image,
					VkImageSubresourceLayers{VK_IMAGE_ASPECT_COLOR_BIT, mip, slice, 1u},
					VkOffset3D{0, 0, 0},
					extent
				);

				_source->StreamTexels( request );
			}
		}

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	VkResult ShaderImage::BindResources( GpuHeap& heap, const ImageSource& source ) {
		using ::Eldritch2::Swap;

		const VkImageCreateInfo			imageInfo( GetCreateInfo( source.GetDimensions() ) );
		const VmaAllocationCreateInfo	allocationInfo{
			0u,
			VMA_MEMORY_USAGE_GPU_ONLY,
			InferMemoryPropertiesFromUsage,
			InferMemoryPropertiesFromUsage,
			VK_NULL_HANDLE,	//	Use default pooling scheme.
			nullptr			//	No user data.
		};

		VmaAllocation	backing;
		VkImage			image;

		ET_FAIL_UNLESS( vmaCreateImage( heap, &imageInfo, &allocationInfo, &image, &backing, nullptr ) );
		ET_AT_SCOPE_EXIT( vmaDestroyImage( heap, image, backing ) );

		Swap( _backing, backing );
		Swap( _image,	image );

		_source = &source;

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	void ShaderImage::FreeResources( GpuHeap& heap ) {
		if (eastl::exchange( _source, nullptr )) {
			heap.AddGarbage( { eastl::exchange( _image, nullptr ), eastl::exchange( _backing, nullptr ) } );
		}
	}

// ---------------------------------------------------

	void Swap( ShaderImage& image0, ShaderImage& image1 ) {
		using ::Eldritch2::Swap;

		Swap( image0._backing,	image1._backing );
		Swap( image0._image,	image1._image );
		Swap( image0._source,	image1._source );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2