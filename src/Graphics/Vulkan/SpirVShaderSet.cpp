/*==================================================================*\
  SpirVShaderSet.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2019 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/SpirVShaderSet.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	SpirVShaderSet::SpirVShaderSet(SpirVShaderSet&& set) ETNoexceptHint : SpirVShaderSet() {
		Swap(*this, set);
	}

	// ---------------------------------------------------

	SpirVShaderSet::SpirVShaderSet() ETNoexceptHint : _combinedBytecode("SPIR-V Shader Combined Bytecode Allocator"), _subshaderByPass("SPIR-V Sub-shader by Pass Allocator") {}

	// ---------------------------------------------------

	Result SpirVShaderSet::BindResources(BytecodeList bytecode, SubshaderMap shaders) ETNoexceptHint {
#if ET_ASSERTIONS_ENABLED

#endif

		return Result::Success;
	}

	// ---------------------------------------------------

	void SpirVShaderSet::FreeResources() ETNoexceptHint {
		_subshaderByPass.Clear();
		_combinedBytecode.Clear(ReleaseMemorySemantics());
	}

}}} // namespace Eldritch2::Graphics::Vulkan
