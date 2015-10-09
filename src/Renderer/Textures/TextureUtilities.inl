/*==================================================================*\
  TextureUtilities.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2012 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Math/MathUtils.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {

	ETInlineHint ETNoAliasHint ::Eldritch2::float32 GetModelScreenCoverageConstant( ::Eldritch2::float32 distance, ::Eldritch2::float32 FOV, ::Eldritch2::float32 inverseResolution ) {
		::Eldritch2::float32	temp( distance * ::Eldritch2::FastTangent( FOV ) );

		temp = inverseResolution * ::Eldritch2::Reciprocal( temp );
		return( temp * temp );
	}

// ---------------------------------------------------

	ETInlineHint ETNoAliasHint ::Eldritch2::float32 EstimateTriangleScreenArea( ::Eldritch2::float32 worldArea, ::Eldritch2::float32 coverageConstant ) {
		return worldArea * coverageConstant;
	}

// ---------------------------------------------------

	ETInlineHint ETNoAliasHint ::Eldritch2::float32 CalculateMinTextureLOD( ::Eldritch2::float32 triangleScreenArea, ::Eldritch2::float32 triangleUVArea ) {
		return( 0.5f * ::Eldritch2::Log2( triangleScreenArea / triangleUVArea ) );
	}

// ---------------------------------------------------

	ETInlineHint ETNoAliasHint ::Eldritch2::uint32 CalcTotalMIPPyramidLevels( ::Eldritch2::uint32 width, ::Eldritch2::uint32 height, ::Eldritch2::uint32 depth ) {
		return( 1u + ::Eldritch2::Log2( ::Eldritch2::Max( ::Eldritch2::Max( width, height ), depth ) ) );
	}

}	// namespace Renderer
}	// namespace Eldritch2