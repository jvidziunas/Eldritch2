/*==================================================================*\
  MeshView.inl
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

	ETInlineHint const AssetViews::SpirVShaderPipelineView& MeshView::Submesh::GetShaderPipeline() const {
		return *_pipeline;
	}

}	// namespace AssetViews
}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

