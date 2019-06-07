/*==================================================================*\
  ResourceBus.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	ETInlineHint ETForceInlineHint const ShaderImage* ResourceBus::Find(const ImageSource& image) const ETNoexceptHint {
		const auto candidate(_imagesBySource.Find(ETAddressOf(image)));
		return ET_LIKELY(candidate != _imagesBySource.End()) ? ETAddressOf(candidate->second) : nullptr;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint const Mesh* ResourceBus::Find(const MeshSource& mesh) const ETNoexceptHint {
		const auto candidate(_meshesBySource.Find(ETAddressOf(mesh)));
		return ET_LIKELY(candidate != _meshesBySource.End()) ? ETAddressOf(candidate->second) : nullptr;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
