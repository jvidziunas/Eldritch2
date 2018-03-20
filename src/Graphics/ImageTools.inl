/*==================================================================*\
  ImageTools.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2012 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {

	ETInlineHint ETPureFunctionHint float32 GetModelScreenCoverageConstant( float32 distance, Angle fov, float32 inverseResolution ) {
		const float32	temp( inverseResolution * distance * ApproxTangent( fov ) );

		return temp * temp;
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint float32 EstimateTriangleScreenArea( float32 worldArea, float32 coverageConstant ) {
		return worldArea * coverageConstant;
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint float32 CalculateMinTextureLod( float32 triangleScreenArea, float32 triangleUvArea ) {
		return 0.5f * Log2( triangleScreenArea / triangleUvArea );
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint uint32 CalcTotalMipPyramidLevels( uint32 width, uint32 height, uint32 depth ) {
		return 1u + Log2( Max( Max( width, height ), depth ) );
	}

}	// namespace Graphics
}	// namespace Eldritch2