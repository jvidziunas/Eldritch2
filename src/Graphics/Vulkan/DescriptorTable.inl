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

	ETConstexpr ETInlineHint ETForceInlineHint auto DescriptorTable::GetDescriptorSetLayouts() const ETNoexceptHint -> const VkDescriptorSetLayout (&)[1] {
		return _setLayouts;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint auto DescriptorTable::GetDescriptorSets() const ETNoexceptHint -> const VkDescriptorSet (&)[1] {
		return _sets;
	}

	// ---------------------------------------------------

	template <typename InputIterator>
	ETInlineHint ETForceInlineHint void DescriptorTable::PushShaderResources(uint32_t& outSlot, InputIterator begin, InputIterator end) {
		outSlot = _resources.GetSize();
		while (begin != end) {
			_resources.EmplaceBack(*begin++, VK_NULL_HANDLE);
		}
	}

}}} // namespace Eldritch2::Graphics::Vulkan
