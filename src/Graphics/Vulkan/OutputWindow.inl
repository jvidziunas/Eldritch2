/*==================================================================*\
  OutputWindow.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	ETInlineHint size_t OutputWindow::AddReference( std::memory_order order ) const {
		return _referenceCount.fetch_add( 1u, order );
	}

// ---------------------------------------------------

	ETInlineHint size_t OutputWindow::ReleaseReference( std::memory_order order ) const {
		return _referenceCount.fetch_sub( 1u, order );
	}

// ---------------------------------------------------

	ETInlineHint bool OutputWindow::IsReferenced( std::memory_order order ) const {
		return _referenceCount.load( order ) != 0u;
	}

// ---------------------------------------------------

	ETInlineHint VkSwapchainKHR OutputWindow::GetSwapchain() const {
		return _swapchain;
	}

// ---------------------------------------------------

	ETInlineHint uint32_t OutputWindow::GetPresentableImageIndex() const {
		return _presentableIndex;
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2