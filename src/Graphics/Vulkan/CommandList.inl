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

	ETInlineHint ETForceInlineHint VkCommandBuffer CommandList::Get() ETNoexceptHint {
		return _commands;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint CommandList::operator VkCommandBuffer() ETNoexceptHint {
		return _commands;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void Swap(CommandList& lhs, CommandList& rhs) ETNoexceptHint {
		using ::Eldritch2::Swap;

		Swap(lhs._pool, rhs._pool);
		Swap(lhs._commands, rhs._commands);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
