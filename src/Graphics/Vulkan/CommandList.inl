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

	ETConstexpr ETInlineHint ETForceInlineHint auto CommandList::Get() const ETNoexceptHint -> const VkCommandBuffer (&)[1] {
		return _commands;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void Swap(CommandList& lhs, CommandList& rhs) ETNoexceptHint {
		using ::Eldritch2::Swap;

		Swap(lhs._pool, rhs._pool);
		Swap(lhs._commands, rhs._commands);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
