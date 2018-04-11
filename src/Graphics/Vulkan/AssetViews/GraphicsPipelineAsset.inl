/*==================================================================*\
  GraphicsPipelineAsset.inl
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
namespace AssetViews {

	ETInlineHint const Utf8Char* GraphicsPipelineAsset::StageAsset::GetShaderUsageName() const {
		return _shaderUsageName;
	}

// ---------------------------------------------------

	ETInlineHint const ArrayList<GraphicsPipelineAsset::AttachmentAsset>& GraphicsPipelineAsset::GetAttachments() const {
		return _attachments;
	}

// ---------------------------------------------------

	ETInlineHint const ArrayList<GraphicsPipelineAsset::StageAsset>& GraphicsPipelineAsset::GetStages() const {
		return _stages;
	}

}	// namespace AssetViews
}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2