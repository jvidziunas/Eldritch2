/*==================================================================*\
  SpirVShaderSet.inl
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

	ETCpp14Constexpr size_t GetHashCode(const SpirVShader& shader, size_t seed = 0u) ETNoexceptHint {
		return HashMemory(shader.name, StringLength(shader.name) * sizeof(*shader.name), seed);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator==(const SpirVShader& lhs, const SpirVShader& rhs) ETNoexceptHint {
		return lhs == StringView(rhs.name);
	}

	// ---------------------------------------------------

	ETPureFunctionHint ETForceInlineHint ETPureFunctionHint bool operator==(const SpirVShader& lhs, const StringView& rhs) ETNoexceptHint {
		return StringView(lhs.name) == rhs;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
