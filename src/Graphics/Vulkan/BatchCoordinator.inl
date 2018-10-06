/*==================================================================*\
  BatchCoordinator.inl
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

	template <typename Constants>
	ETInlineHint ETForceInlineHint VkResult BatchCoordinator::BindShaderSet(Gpu& gpu, const SpirVShaderSet& shaders, const Constants& constants) {
		static_assert(eastl::is_trivially_copyable_v<Constants>, "Shader constant structure must be trivially copyable!");

		return BindShaderSet(gpu, shaders, ETAddressOf(constants), sizeof(constants));
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint BatchCoordinator::CommandPool& BatchCoordinator::FindPool(VkPipeline pipeline) ETNoexceptHint {
		using ::Eldritch2::GetHashCode;
		return _commandPools[GetHashCode(pipeline) % _commandPools.GetSize()];
	}

}}} // namespace Eldritch2::Graphics::Vulkan
