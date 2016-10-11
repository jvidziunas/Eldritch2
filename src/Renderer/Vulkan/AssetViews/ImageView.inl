/*==================================================================*\
  ImageView.inl
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
namespace AssetViews {

	ETInlineHint VkImageView ImageView::GetShaderView() const {
		return _view.Get();
	}

// ---------------------------------------------------

	ETInlineHint VkImageView ImageView::GetFramebufferAttachmentView() const {
		return GetShaderView();
	}

}	// namespace AssetViews
}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2