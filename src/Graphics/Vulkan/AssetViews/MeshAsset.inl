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

	ETInlineHint ETForceInlineHint Meshlet MeshAsset::GetMeshlet(uint32 meshlet) const ETNoexceptHint {
		ETAssert(meshlet < _meshlets.GetSize(), "Meshlet {} is out-of-bounds (valid range [0-{})) for mesh {}", meshlet, _meshlets.GetSize(), GetPath());
		return _meshlets[meshlet];
	}

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews
