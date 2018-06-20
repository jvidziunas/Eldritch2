/*==================================================================*\
  ImageAsset.inl
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

	ETInlineHint const CrunchImageSource& ImageAsset::GetImageSource() const {
		return _source;
	}

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews
