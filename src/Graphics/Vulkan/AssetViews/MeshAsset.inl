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

	ETInlineHint ETForceInlineHint MeshAsset::SurfaceDescription MeshAsset::GetSurface(uint32 surface) const ETNoexceptHint {
		return _surfaces[surface];
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint uint32 MeshAsset::GetSurfaceCount() const ETNoexceptHint {
		return uint32(_surfaces.GetSize());
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint uint32 MeshAsset::GetVerticesSize() const ETNoexceptHint {
		return sizeof(Vector);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint uint32 MeshAsset::GetIndicesSize() const ETNoexceptHint {
		return sizeof(uint16);
	}

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews
