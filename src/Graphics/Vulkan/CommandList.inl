/*==================================================================*\
  CommandList.inl
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

	ETInlineHint VkCommandBuffer CommandList::Get() {
		return _commandBuffer;
	}

	// ---------------------------------------------------

	ETInlineHint CommandList::operator VkCommandBuffer() {
		return _commandBuffer;
	}

	// ---------------------------------------------------

	ETInlineHint void Swap(CommandList& lhs, CommandList& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._pool, rhs._pool);
		Swap(lhs._commandBuffer, rhs._commandBuffer);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
