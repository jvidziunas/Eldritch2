/*==================================================================*\
  GraphicsPipelineAsset.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  �2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan { namespace AssetViews {

	ETInlineHint GraphicsPipelineAsset::GraphicsPipelineAsset(StringView path) :
		Assets::Asset(path) {
	}

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews