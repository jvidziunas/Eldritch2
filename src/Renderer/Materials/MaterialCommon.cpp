/*==================================================================*\
  MaterialCommon.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Materials/MaterialCommon.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Renderer {

	const UTF8Char* const	rasterizerStatePropertyString					= UTF8L( "RasterizerState" );
	const UTF8Char* const	depthStencilStatePropertyString					= UTF8L( "DepthStencilState" );
	const UTF8Char* const	blendStatePropertyString						= UTF8L( "BlendState" );
	const UTF8Char* const	renderTargetsPropertyString						= UTF8L( "RenderTargets" );
	const UTF8Char* const	depthStencilTargetPropertyString				= UTF8L( "DepthStencilTarget" );
	const UTF8Char* const	vertexShaderResourcesPropertyString				= UTF8L( "VertexShaderResources" );
	const UTF8Char* const	hullShaderResourcesPropertyString				= UTF8L( "HullShaderResources" );
	const UTF8Char* const	domainShaderResourcesPropertyString				= UTF8L( "DomainShaderResources" );
	const UTF8Char* const	geometryShaderResourcesPropertyString			= UTF8L( "GeometryShaderResources" );
	const UTF8Char* const	pixelShaderResourcesPropertyString				= UTF8L( "PixelShaderResources" );
	const UTF8Char* const	computeShaderResourcesPropertyString			= UTF8L( "ComputeShaderResources" );
	const UTF8Char* const	computeShaderUnorderedAccessViewsPropertyString	= UTF8L( "ComputeShaderUnorderedAccessViews" );
	const UTF8Char* const	vertexShaderPropertyString						= UTF8L( "VertexShader" );
	const UTF8Char* const	hullShaderPropertyString						= UTF8L( "HullShader" );
	const UTF8Char* const	domainShaderPropertyString						= UTF8L( "DomainShader" );
	const UTF8Char* const	geometryShaderPropertyString					= UTF8L( "GeometryShader" );
	const UTF8Char* const	pixelShaderPropertyString						= UTF8L( "PixelShader" );
	const UTF8Char* const	computeShaderPropertyString						= UTF8L( "ComputeShader" );
	const UTF8Char* const	alphaToCoverageEnabledPropertyString			= UTF8L( "AlphaToCoverageEnabled" );
	const UTF8Char* const	blendStatesPropertyString						= UTF8L( "BlendStates" );
	const UTF8Char* const	customBlendColorPropertyString					= UTF8L( "CustomBlendColor" );
	const UTF8Char* const	sourceBlendFactorPropertyString					= UTF8L( "SourceBlendFactor" );
	const UTF8Char* const	renderTargetBlendFactorPropertyString			= UTF8L( "RenderTargetBlendFactor" );
	const UTF8Char* const	sourceAlphaBlendFactorPropertyString			= UTF8L( "SourceAlphaBlendFactor" );
	const UTF8Char* const	renderTargetAlphaBlendFactorPropertyString		= UTF8L( "RenderTargetAlphaBlendFactor" );
	const UTF8Char* const	combineOperationPropertyString					= UTF8L( "CombineOperation" );
	const UTF8Char* const	alphaCombineOperationPropertyString				= UTF8L( "AlphaCombineOperation" );

	const UTF8Char* const	frontFaceCullModeString	= UTF8L( "FrontFace" );
	const UTF8Char* const	backFaceCullModeString	= UTF8L( "BackFace" );
	const UTF8Char* const	noCullModeString		= UTF8L( "None" );

	const UTF8Char* const	lessDepthComparisonModeString			= UTF8L( "Less" );
	const UTF8Char* const	equalDepthComparisonModeString			= UTF8L( "Equal" );
	const UTF8Char* const	lessEqualDepthComparisonModeString		= UTF8L( "LessEqual" );
	const UTF8Char* const	greaterDepthComparisonModeString		= UTF8L( "Greater" );
	const UTF8Char* const	greaterEqualDepthComparisonModeString	= UTF8L( "GreaterEqual" );
	const UTF8Char* const	notEqualDepthComparisonModeString		= UTF8L( "NotEqual" );
	const UTF8Char* const	alwaysPassDepthComparisonModeString		= UTF8L( "AlwaysPass" );

	const UTF8Char* const	zeroBlendFactorString						= UTF8L( "Zero" );
	const UTF8Char* const	oneBlendFactorString						= UTF8L( "One" );
	const UTF8Char* const	sourceColorBlendFactorString				= UTF8L( "SourceColor" );
	const UTF8Char* const	oneMinusSourceColorBlendFactorString		= UTF8L( "OneMinusSourceColor" );
	const UTF8Char* const	sourceAlphaBlendFactorString				= UTF8L( "SourceAlpha" );
	const UTF8Char* const	saturatedSourceAlphaBlendFactorString		= UTF8L( "SaturatedSourceAlpha" );
	const UTF8Char* const	oneMinusSourceAlphaBlendFactorString		= UTF8L( "OneMinusSourceAlpha" );
	const UTF8Char* const	renderTargetColorBlendFactorString			= UTF8L( "RenderTargetColor" );
	const UTF8Char* const	oneMinusRenderTargetColorBlendFactorString	= UTF8L( "OneMinusRenderTargetColor" );
	const UTF8Char* const	renderTargetAlphaBlendFactorString			= UTF8L( "RenderTargetAlpha" );
	const UTF8Char* const	oneMinusRenderTargetAlphaBlendFactorString	= UTF8L( "OneMinusRenderTargetAlpha" );
	const UTF8Char* const	customColorBlendFactorString				= UTF8L( "CustomColor" );
	const UTF8Char* const	oneMinusCustomColorBlendFactorString		= UTF8L( "OneMinusCustomColor" );
	const UTF8Char* const	dualSourceColorBlendFactorString			= UTF8L( "DualSourceColor" );
	const UTF8Char* const	oneMinusDualSourceColorBlendFactorString	= UTF8L( "OneMinusDualSourceColor" );
	const UTF8Char* const	dualSourceAlphaBlendFactorString			= UTF8L( "DualSourceAlpha" );
	const UTF8Char* const	oneMinusDualSourceAlphaBlendFactorString	= UTF8L( "OneMinusDualSourceAlpha" );

	const UTF8Char* const	addBlendModeString				= UTF8L( "Add" );
	const UTF8Char* const	subtractBlendModeString			= UTF8L( "Subtract" );
	const UTF8Char* const	reverseSubtractBlendModeString	= UTF8L( "ReverseSubtract" );
	const UTF8Char* const	minBlendModeString				= UTF8L( "Min" );
	const UTF8Char* const	maxBlendModeString				= UTF8L( "Max" );

}	// namespace Renderer
}	// namespace Eldritch2