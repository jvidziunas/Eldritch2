/*==================================================================*\
  ImageTools.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {

	ETPureFunctionHint float32 GetModelScreenCoverageConstant(float32 distance, Angle fov, float32 inverseResolution);

	ETPureFunctionHint float32 EstimateTriangleScreenArea(float32 worldArea, float32 screenCoverageConstant);

	ETPureFunctionHint float32 CalculateMinTextureLod(float32 triangleScreenArea, float32 triangleUvArea);

	ETPureFunctionHint uint32 CalcTotalMipPyramidLevels(uint32 width, uint32 height, uint32 depth = 0u);

}} // namespace Eldritch2::Graphics

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/ImageTools.inl>
//------------------------------------------------------------------//
