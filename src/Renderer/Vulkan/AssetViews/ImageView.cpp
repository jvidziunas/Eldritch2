/*==================================================================*\
  ImageView.cpp
  ------------------------------------------------------------------
  Purpose:
  
  
  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/AssetViews/ImageView.hpp>
#include <Renderer/Vulkan/ObjectBuilders.hpp>
#include <Utility/Containers/Range.hpp>
#include <Renderer/Vulkan/Device.hpp>
#include <Assets/AssetLibrary.hpp>
#include <Utility/ErrorCode.hpp>
#include <Utility/Result.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Logging;
using namespace ::Eldritch2::Assets;

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {
namespace AssetViews {

	ImageView::ImageView( const Utf8Char* const name, DeviceMemoryPool::Allocation allocation, UniquePointer<VkImageView> view ) : AssetView( name ), _allocation( eastl::move( allocation ) ), _view( eastl::move( view ) ) {}

// ---------------------------------------------------

	Result<Eldritch2::UniquePointer<AssetView>> ImageView::CreateView( Allocator& allocator, Device& device, DeviceMemoryPool& pool, const AssetLibrary& library, const Utf8Char* const name, Range<const char*> rawBytes ) {
		UniquePointer<VkImageView>	view;
		ImageBuilder				imageBuilder;

	//	Configure all core image parameters.
		imageBuilder.SetTiling( VkImageTiling::VK_IMAGE_TILING_OPTIMAL );

		VulkanResult<VkImage>	createImageResult( imageBuilder.Create( device, device.GetAllocator() ) );
		if( !createImageResult ) {
			library.GetLog()( MessageSeverity::Error, "Error creating VkImage for asset '{}'." ET_UTF8_NEWLINE_LITERAL, name );
			return Error::Unspecified;
		}

	//	Determine memory requirements.
		VkMemoryRequirements	memoryRequirements;
		vkGetImageMemoryRequirements( device, createImageResult->Get(), &memoryRequirements );

	//	Allocate memory.
		Result<DeviceMemoryPool::Allocation>	allocateMemoryResult( pool.TryAllocateRegion( memoryRequirements.size ) );
		if( !allocateMemoryResult ) {
			library.GetLog()( MessageSeverity::Error, "Error allocating Vulkan device memory for asset '{}'." ET_UTF8_NEWLINE_LITERAL, name );
			return allocateMemoryResult.GetErrorCode();
		}

	//	Bind memory to image.
		vkBindImageMemory( device, createImageResult->Get(), pool.GetDeviceMemory(), allocateMemoryResult->GetOffsetIntoPoolInBytes() );

	//	Create a shader view.
		ImageViewBuilder			viewBuilder;
		VulkanResult<VkImageView>	createViewResult( viewBuilder.Create( device, createImageResult->Get(), device.GetAllocator() ) );
		if( !createViewResult ) {
			library.GetLog()( MessageSeverity::Error, "Error creating shader VkImageView for asset '{}'." ET_UTF8_NEWLINE_LITERAL, name );
			return Error::Unspecified;
		}

	//	Commit the view to the outer function.
		view = eastl::move( *createViewResult );

	//	Combine all the pieces into the final ImageView.
		auto	result( MakeUnique<ImageView>( allocator, name, eastl::move( *allocateMemoryResult ), eastl::move( view ) ) );
		if( !result ) {
			library.GetLog()( MessageSeverity::Error, "Error creating asset view object for asset '{}'." ET_UTF8_NEWLINE_LITERAL, name );
			return Error::OutOfMemory;
		}

		return eastl::move( result );
	}

// ---------------------------------------------------

	ETPureFunctionHint Utf8Literal ImageView::GetExtension() {
		return ".E2Image";
	}

}	// namespace AssetViews
}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2
