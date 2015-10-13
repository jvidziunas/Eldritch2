/*==================================================================*\
  TextureUtilities.hpp
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

	ETInlineHint ETNoAliasHint ::Eldritch2::float32	GetModelScreenCoverageConstant( ::Eldritch2::float32 distance, ::Eldritch2::float32 FOV, ::Eldritch2::float32 inverseResolution );

// ---------------------------------------------------

	ETInlineHint ETNoAliasHint ::Eldritch2::float32	EstimateTriangleScreenArea( ::Eldritch2::float32 worldArea, ::Eldritch2::float32 coverageConstant );

// ---------------------------------------------------

	ETInlineHint ETNoAliasHint ::Eldritch2::float32	CalculateMinTextureLOD( ::Eldritch2::float32 triangleScreenArea, ::Eldritch2::float32 triangleUVArea );

// ---------------------------------------------------

	ETInlineHint ETNoAliasHint ::Eldritch2::uint32	CalcTotalMIPPyramidLevels( ::Eldritch2::uint32 width, ::Eldritch2::uint32 height, ::Eldritch2::uint32 depth = 0u );

}	// namespace Renderer
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Renderer/Textures/TextureUtilities.inl>
//------------------------------------------------------------------//