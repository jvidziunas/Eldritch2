/*==================================================================*\
  SwapChain.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	ETInlineHint Eldritch2::uint32 SwapChain::GetCurrentImageIndex() const {
		return _currentImageIndex;
	}

// ---------------------------------------------------

	ETInlineHint SwapChain::operator VkSwapchainKHR() const {
		return _swapChain.Get();
	}

// ---------------------------------------------------

	ETInlineHint SwapChain::operator VkImageView() const {
		return (GetCurrentImageIndex() < MaximumQueuedImageCount) ? _views[GetCurrentImageIndex()].Get() : VK_NULL_HANDLE;
	}

// ---------------------------------------------------

	ETInlineHint SwapChain::operator VkImage() const {
		return (GetCurrentImageIndex() < MaximumQueuedImageCount) ? _images[GetCurrentImageIndex()] : VK_NULL_HANDLE;
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2