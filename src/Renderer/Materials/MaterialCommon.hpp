/*==================================================================*\
  MaterialCommon.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/CharTypes.hpp>
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {

	namespace Renderer	= ::Eldritch2::Renderer;

// ---------------------------------------------------

	namespace MaterialStageFlag {

		ENUM_CLASS( ::Eldritch2::uint32 ) {
			RASTER_DISPATCH		= 1u,
			COMPUTE_DISPATCH	= 0u,
			INDIRECT_DISPATCH	= 2u,

			DEFAULT_FLAGS		= RASTER_DISPATCH
		};

	}	// namespace MaterialStageFlag

	typedef ::Eldritch2::uint32	MaterialStageFlags;

// ---------------------------------------------------

	// Material property names
	extern const ::Eldritch2::UTF8Char* const	rasterizerStatePropertyString;
	extern const ::Eldritch2::UTF8Char* const	depthStencilStatePropertyString;
	extern const ::Eldritch2::UTF8Char* const	blendStatePropertyString;
	extern const ::Eldritch2::UTF8Char* const	renderTargetsPropertyString;
	extern const ::Eldritch2::UTF8Char* const	depthStencilTargetPropertyString;
	extern const ::Eldritch2::UTF8Char* const	vertexShaderResourcesPropertyString;
	extern const ::Eldritch2::UTF8Char* const	hullShaderResourcesPropertyString;
	extern const ::Eldritch2::UTF8Char* const	domainShaderResourcesPropertyString;
	extern const ::Eldritch2::UTF8Char* const	geometryShaderResourcesPropertyString;
	extern const ::Eldritch2::UTF8Char* const	pixelShaderResourcesPropertyString;
	extern const ::Eldritch2::UTF8Char* const	computeShaderResourcesPropertyString;
	extern const ::Eldritch2::UTF8Char* const	computeShaderUnorderedAccessViewsPropertyString;
	extern const ::Eldritch2::UTF8Char* const	vertexShaderPropertyString;
	extern const ::Eldritch2::UTF8Char* const	hullShaderPropertyString;
	extern const ::Eldritch2::UTF8Char* const	domainShaderPropertyString;
	extern const ::Eldritch2::UTF8Char* const	geometryShaderPropertyString;
	extern const ::Eldritch2::UTF8Char* const	pixelShaderPropertyString;
	extern const ::Eldritch2::UTF8Char* const	computeShaderPropertyString;
	extern const ::Eldritch2::UTF8Char* const	alphaToCoverageEnabledPropertyString;
	extern const ::Eldritch2::UTF8Char* const	blendStatesPropertyString;
	extern const ::Eldritch2::UTF8Char* const	customBlendColorPropertyString;
	extern const ::Eldritch2::UTF8Char* const	sourceBlendFactorPropertyString;
	extern const ::Eldritch2::UTF8Char* const	renderTargetBlendFactorPropertyString;
	extern const ::Eldritch2::UTF8Char* const	sourceAlphaBlendFactorPropertyString;
	extern const ::Eldritch2::UTF8Char* const	renderTargetAlphaBlendFactorPropertyString;
	extern const ::Eldritch2::UTF8Char* const	combineOperationPropertyString;
	extern const ::Eldritch2::UTF8Char* const	alphaCombineOperationPropertyString;

	// Cull mode material strings
	extern const ::Eldritch2::UTF8Char* const	frontFaceCullModeString;
	extern const ::Eldritch2::UTF8Char* const	backFaceCullModeString;
	extern const ::Eldritch2::UTF8Char* const	noCullModeString;

	// Depth comparison material strings
	extern const ::Eldritch2::UTF8Char* const	lessDepthComparisonModeString;
	extern const ::Eldritch2::UTF8Char* const	equalDepthComparisonModeString;
	extern const ::Eldritch2::UTF8Char* const	lessEqualDepthComparisonModeString;
	extern const ::Eldritch2::UTF8Char* const	greaterDepthComparisonModeString;
	extern const ::Eldritch2::UTF8Char* const	greaterEqualDepthComparisonModeString;
	extern const ::Eldritch2::UTF8Char* const	notEqualDepthComparisonModeString;
	extern const ::Eldritch2::UTF8Char* const	alwaysPassDepthComparisonModeString;

	// Blend factor strings
	extern const ::Eldritch2::UTF8Char* const	zeroBlendFactorString;
	extern const ::Eldritch2::UTF8Char* const	oneBlendFactorString;
	extern const ::Eldritch2::UTF8Char* const	sourceColorBlendFactorString;
	extern const ::Eldritch2::UTF8Char* const	oneMinusSourceColorBlendFactorString;
	extern const ::Eldritch2::UTF8Char* const	sourceAlphaBlendFactorString;
	extern const ::Eldritch2::UTF8Char* const	saturatedSourceAlphaBlendFactorString;
	extern const ::Eldritch2::UTF8Char* const	oneMinusSourceAlphaBlendFactorString;
	extern const ::Eldritch2::UTF8Char* const	renderTargetColorBlendFactorString;
	extern const ::Eldritch2::UTF8Char* const	oneMinusRenderTargetColorBlendFactorString;
	extern const ::Eldritch2::UTF8Char* const	renderTargetAlphaBlendFactorString;
	extern const ::Eldritch2::UTF8Char* const	oneMinusRenderTargetAlphaBlendFactorString;
	extern const ::Eldritch2::UTF8Char* const	customColorBlendFactorString;
	extern const ::Eldritch2::UTF8Char* const	oneMinusCustomColorBlendFactorString;
	extern const ::Eldritch2::UTF8Char* const	dualSourceColorBlendFactorString;
	extern const ::Eldritch2::UTF8Char* const	oneMinusDualSourceColorBlendFactorString;
	extern const ::Eldritch2::UTF8Char* const	dualSourceAlphaBlendFactorString;
	extern const ::Eldritch2::UTF8Char* const	oneMinusDualSourceAlphaBlendFactorString;

	// Blend mode strings
	extern const ::Eldritch2::UTF8Char* const	addBlendModeString;
	extern const ::Eldritch2::UTF8Char* const	subtractBlendModeString;
	extern const ::Eldritch2::UTF8Char* const	reverseSubtractBlendModeString;
	extern const ::Eldritch2::UTF8Char* const	minBlendModeString;
	extern const ::Eldritch2::UTF8Char* const	maxBlendModeString;

}	// namespace Renderer
}	// namespace Eldritch2