/*==================================================================*\
  MeshAsset.inl
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

	ETInlineHint const ArrayList<MeshAsset::Attribute>& MeshAsset::GetAttributes() const {
		return _attributes;
	}

	// ---------------------------------------------------

	ETInlineHint const ArrayList<MeshAsset::Surface>& MeshAsset::GetSurfaces() const {
		return _surfaces;
	}

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews
