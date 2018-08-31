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

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	ETInlineHint ETForceInlineHint VkDescriptorSet DescriptorTable::GetDescriptorSet() const {
		return _descriptors;
	}

	// ---------------------------------------------------

	template <uint32_t count>
	ETInlineHint ETForceInlineHint void DescriptorTable::PushDescriptors(Gpu& gpu, uint32_t slot, const VkDescriptorImageInfo (&images)[count]) {
		return this->PushDescriptors(gpu, slot, count, images);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
