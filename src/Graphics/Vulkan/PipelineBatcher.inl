/*==================================================================*\
  PipelineBatcher.inl
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
	ETInlineHint ETForceInlineHint VkResult PipelineBatcher::Compile(Gpu& gpu, const SpirVShaderSet& shaders, const Constants& constants) {
		ETStaticAssert(IsTriviallyCopyConstructible<Constants>(), "Shader constant block must satisfy IsTriviallyCopyConstructible concept!");
		return Compile(gpu, shaders, sizeof(constants) reinterpret_cast<const byte*>(ETAddressOf(constants)));
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint PipelineBatcher::CommandPool& PipelineBatcher::FindPool(VkPipeline pipeline) ETNoexceptHint {
		using ::Eldritch2::GetHashCode;
		return _commandPools[GetHashCode(pipeline) % _commandPools.GetSize()];
	}

}}} // namespace Eldritch2::Graphics::Vulkan
