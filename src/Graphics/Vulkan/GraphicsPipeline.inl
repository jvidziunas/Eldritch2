/*==================================================================*\
  GraphicsPipeline.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	ETConstexpr ETInlineHint ETForceInlineHint const BatchCoordinator& GraphicsPipeline::GetBatches() const ETNoexceptHint {
		return _batches;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint BatchCoordinator& GraphicsPipeline::GetBatches() ETNoexceptHint {
		return _batches;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint const DescriptorTable& GraphicsPipeline::GetShaderResources() const ETNoexceptHint {
		return _shaderResources;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint DescriptorTable& GraphicsPipeline::GetShaderResources() ETNoexceptHint {
		return _shaderResources;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
