/*==================================================================*\
  FrustumUtils.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Math/FrustumUtils.hpp>
#include <Utility/Algorithms.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Utility {

#if 0
	ETNoThrowHint ETNoAliasHint FrustumPlaneCollection ExtractFrustumPlanes( const Float4x4& cameraMatrix ) {
		FrustumPlaneCollection	result;

		result[FrustumPlanes::LEFT]		= cameraMatrix.col( 4 ) + cameraMatrix.col( 1 );
		result[FrustumPlanes::RIGHT]	= cameraMatrix.col( 4 ) - cameraMatrix.col( 1 );
		result[FrustumPlanes::TOP]		= cameraMatrix.col( 4 ) - cameraMatrix.col( 2 );
		result[FrustumPlanes::BOTTOM]	= cameraMatrix.col( 4 ) + cameraMatrix.col( 2 );
		result[FrustumPlanes::NEAR]		= cameraMatrix.col( 3 );
		result[FrustumPlanes::FAR]		= cameraMatrix.col( 4 ) - cameraMatrix.col( 3 );

		return result;
	}

// ---------------------------------------------------

	ETNoThrowHint ETNoAliasHint FrustumPlaneCollection ExtractNormalizedFrustumPlanes( const Float4x4& cameraMatrix ) {
		struct NormalizePredicate {
			ETNoAliasHint ETNoThrowHint static void Apply( Float4& plane ) {
				plane /= plane.block<3, 1>( 0, 0 ).norm();
			}
		};

	// ---

		FrustumPlaneCollection	frustumPlanes( ExtractFrustumPlanes( cameraMatrix ) );

		ForEach( frustumPlanes.begin(), frustumPlanes.end(), &NormalizePredicate::Apply );

		return frustumPlanes;
	}

// ---------------------------------------------------

	ETNoThrowHint ETNoAliasHint StructureOfArraysFrustumPlaneCollection ExtractStructureOfArraysFrustumPlanes( const Float4x4& cameraMatrix ) {
		StructureOfArraysFrustumPlaneCollection	resultPlanes;
		const FrustumPlaneCollection			frustumPlanes( ExtractFrustumPlanes( cameraMatrix ) );

		// Rearrange the frustum planes so that dot products can be batched more readily
		resultPlanes[0] = Float4( frustumPlanes[0].x(), frustumPlanes[1].x(), frustumPlanes[2].x(), frustumPlanes[3].x() );
		resultPlanes[1] = Float4( frustumPlanes[0].y(), frustumPlanes[1].y(), frustumPlanes[2].y(), frustumPlanes[3].y() );
		resultPlanes[2] = Float4( frustumPlanes[0].z(), frustumPlanes[1].z(), frustumPlanes[2].z(), frustumPlanes[3].z() );
		resultPlanes[3] = Float4( frustumPlanes[0].w(), frustumPlanes[1].w(), frustumPlanes[2].w(), frustumPlanes[3].w() );
		resultPlanes[4] = Float4( frustumPlanes[4].x(), frustumPlanes[5].x(), frustumPlanes[4].x(), frustumPlanes[5].x() );
		resultPlanes[5] = Float4( frustumPlanes[4].y(), frustumPlanes[5].y(), frustumPlanes[4].y(), frustumPlanes[5].y() );
		resultPlanes[6] = Float4( frustumPlanes[4].z(), frustumPlanes[5].z(), frustumPlanes[4].z(), frustumPlanes[5].z() );
		resultPlanes[7] = Float4( frustumPlanes[4].w(), frustumPlanes[5].w(), frustumPlanes[4].w(), frustumPlanes[5].w() );

		return resultPlanes;
	}

// ---------------------------------------------------

	ETNoThrowHint ETNoAliasHint StructureOfArraysFrustumPlaneCollection ExtractStructureOfArraysNormalizedFrustumPlanes( const Float4x4& cameraMatrix ) {
		StructureOfArraysFrustumPlaneCollection	resultPlanes;
		const FrustumPlaneCollection			frustumPlanes( ExtractNormalizedFrustumPlanes( cameraMatrix ) );

		// Rearrange the frustum planes so that dot products can be batched more readily
		resultPlanes[0] = Float4( frustumPlanes[0].x(), frustumPlanes[1].x(), frustumPlanes[2].x(), frustumPlanes[3].x() );
		resultPlanes[1] = Float4( frustumPlanes[0].y(), frustumPlanes[1].y(), frustumPlanes[2].y(), frustumPlanes[3].y() );
		resultPlanes[2] = Float4( frustumPlanes[0].z(), frustumPlanes[1].z(), frustumPlanes[2].z(), frustumPlanes[3].z() );
		resultPlanes[3] = Float4( frustumPlanes[0].w(), frustumPlanes[1].w(), frustumPlanes[2].w(), frustumPlanes[3].w() );
		resultPlanes[4] = Float4( frustumPlanes[4].x(), frustumPlanes[5].x(), frustumPlanes[4].x(), frustumPlanes[5].x() );
		resultPlanes[5] = Float4( frustumPlanes[4].y(), frustumPlanes[5].y(), frustumPlanes[4].y(), frustumPlanes[5].y() );
		resultPlanes[6] = Float4( frustumPlanes[4].z(), frustumPlanes[5].z(), frustumPlanes[4].z(), frustumPlanes[5].z() );
		resultPlanes[7] = Float4( frustumPlanes[4].w(), frustumPlanes[5].w(), frustumPlanes[4].w(), frustumPlanes[5].w() );

		return resultPlanes;
	}
#endif

}	// namespace Utility
}	// namespace Eldritch2