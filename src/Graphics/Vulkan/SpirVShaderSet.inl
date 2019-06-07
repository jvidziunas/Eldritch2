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

	ETInlineHint ETForceInlineHint SpirVShaderSet::BytecodeList::ConstSliceType SpirVShaderSet::GetBytecode() const ETNoexceptHint {
		return _combinedBytecode;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void Swap(SpirVShaderSet& lhs, SpirVShaderSet& rhs) ETNoexceptHint {
		using ::Eldritch2::Swap;

		Swap(lhs._combinedBytecode, rhs._combinedBytecode);
		Swap(lhs._subshaderByPass, rhs._subshaderByPass);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
