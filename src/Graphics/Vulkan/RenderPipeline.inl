/*==================================================================*\
  RenderPipeline.inl
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
namespace Graphics {
namespace Vulkan {

	ETInlineHint VkRenderPass RenderPipeline::GetRenderPass() {
		return _renderPass;
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2