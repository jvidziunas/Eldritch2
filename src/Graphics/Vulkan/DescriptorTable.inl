/*==================================================================*\
  DescriptorTable.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	template <uint32_t imageCount>
	ETInlineHint void DescriptorTable::PushDescriptors( Gpu& gpu, uint32_t slot, const VkDescriptorImageInfo (&images)[imageCount] ) {
		return this->PushDescriptors( gpu, slot, imageCount, images );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2