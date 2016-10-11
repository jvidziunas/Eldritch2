/*==================================================================*\
  SwapChain.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/ObjectBuilders.hpp>
#include <Renderer/Vulkan/SwapChain.hpp>
#include <Renderer/Vulkan/Device.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	SwapChain::SwapChain( UniquePointer<VkSurfaceKHR> surface ) : _surface( eastl::move( surface ) ), _swapChain( nullptr ), _currentImageIndex( 0u ) {}

// ---------------------------------------------------

	VkResult SwapChain::OnFrameBegin( VkDevice deviceContext, VkSemaphore semaphore, uint64 acquisitionTimeoutPeriodInNanoseconds, const VkAllocationCallbacks* allocationCallbacks ) {
		VkResult	result( VkResult::VK_SUCCESS );

		if( !_swapChain || VkResult::VK_ERROR_OUT_OF_DATE_KHR == AcquireNextImage( deviceContext, semaphore, acquisitionTimeoutPeriodInNanoseconds ) ) {
			result = Rebuild( deviceContext, allocationCallbacks );
		}

		return result;
	}

// ---------------------------------------------------

	VkResult SwapChain::AcquireNextImage( VkDevice deviceContext, VkSemaphore semaphore, uint64 timeoutPeriodNanoseconds ) {
		return vkAcquireNextImageKHR( deviceContext, _swapChain.Get(), timeoutPeriodNanoseconds, semaphore, VK_NULL_HANDLE, &_currentImageIndex );
	}

// ---------------------------------------------------

	VkResult SwapChain::Rebuild( VkDevice deviceContext, const VkAllocationCallbacks* allocationCallbacks ) {
		static_assert( _countof( _images ) == _countof( _views ), "Mismatch between image count and view count for Eldritch2::Renderer::Vulkan::Detail::DeviceSwapChainBase" );

	// ---

		ImageViewBuilder	viewBuilder;
		SwapChainBuilder	swapChainBuilder;
		uint32				imageCount( _countof(_images) );

	//	Subclasses can set the builder parameters however they like here. Commonly this will involve pulling parameters from an operating system window object, etc.
		ConfigureForRebuild( swapChainBuilder, viewBuilder );

	//	Overwrite/fill out parameters.
		swapChainBuilder
			.SetImageCount( imageCount )
			.SetUsage( VkImageUsageFlagBits::VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT )
			.SetOldSwapchain( _swapChain.Get() );

	//	Create the core swap chain object using the parameters we have just set.
		VulkanResult<VkSwapchainKHR> createSwapchainResult( swapChainBuilder.Create( deviceContext, _surface.Get(), allocationCallbacks ) );
		if( !createSwapchainResult ) {
			return createSwapchainResult;
		}

	//	Pull all the image objects (which we do not own) from the swap chain.
		VkResult getImagesResult( vkGetSwapchainImagesKHR( deviceContext, createSwapchainResult->Get(), &imageCount, _images ) );
		if( getImagesResult < VkResult::VK_SUCCESS ) {
			return getImagesResult;
		}

	//	Create image views (which we thus own) for all the system-created images within the swap chain.
		Transform( _images, _views, [&] ( VkImage sourceImage ) -> UniquePointer<VkImageView> {
			if( VK_NULL_HANDLE == sourceImage ) {
				return { nullptr };
			}

			VulkanResult<VkImageView> result( viewBuilder.Create( deviceContext, sourceImage, allocationCallbacks ) );
			if( !result ) {
				return { nullptr };
			}

			return eastl::move( *result );
		} );

	//	Commit the new swap chain and proceed!
		_swapChain = eastl::move( *createSwapchainResult );

		return VkResult::VK_SUCCESS;
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2