/*==================================================================*\
  ImageUtilities.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/FloatTypes.hpp>
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {

	ETPureFunctionHint Eldritch2::float32	GetModelScreenCoverageConstant( Eldritch2::float32 distance, Eldritch2::float32 FOV, Eldritch2::float32 inverseResolution );

	ETPureFunctionHint Eldritch2::float32	EstimateTriangleScreenArea( Eldritch2::float32 worldArea, Eldritch2::float32 coverageConstant );

	ETPureFunctionHint Eldritch2::float32	CalculateMinTextureLod( Eldritch2::float32 triangleScreenArea, Eldritch2::float32 triangleUVArea );

	ETPureFunctionHint Eldritch2::uint32		CalcTotalMipPyramidLevels( Eldritch2::uint32 width, Eldritch2::uint32 height, Eldritch2::uint32 depth = 0u );

}	// namespace Renderer
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Renderer/ImageUtilities.inl>
//------------------------------------------------------------------//