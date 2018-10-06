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

	ETInlineHint ETForceInlineHint MeshSurface MeshAsset::GetSurface(uint32 surface) const ETNoexceptHint {
		ET_ASSERT(surface < _surfaces.GetSize(), "Surface {} is out-of-bounds (limit {}) for mesh {}", surface, _surfaces.GetSize(), GetPath());
		return _surfaces[surface];
	}

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews
