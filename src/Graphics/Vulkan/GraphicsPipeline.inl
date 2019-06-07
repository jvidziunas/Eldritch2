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

	ETConstexpr ETInlineHint ETForceInlineHint const PipelineBatcher& GraphicsPipeline::GetBatches() const ETNoexceptHint {
		return _batches;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint const DescriptorTable& GraphicsPipeline::GetShaderResources() const ETNoexceptHint {
		return _shaderResources;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
