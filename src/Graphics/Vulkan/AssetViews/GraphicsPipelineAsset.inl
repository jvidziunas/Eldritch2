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

namespace Eldritch2 { namespace Graphics { namespace Vulkan { namespace AssetViews {

	ETInlineHint const GraphicsPipelineBuilder& GraphicsPipelineAsset::GetBuilder() const {
		return _pipelineBuilder;
	}

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews
