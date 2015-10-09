/*==================================================================*\
  SSE4SkeletalAnimator.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/Animators/SSE4SkeletalAnimator.hpp>
#include <Animation/AnimationTypes.hpp>
#include <Utility/Containers/Range.hpp>
#include <intrin.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2;
using namespace ::Eldritch2::Animation;

namespace {

	ETInlineHint ETNoAliasHint __m128 LinearInterpolate( __m128 quaternion0, const __m128 quaternion1, const float32 alpha ) {
		// Determine whether or not one of the quaternions must be negated in order for shortest-path interpolation.
		// Mask bits will be set to all ones if the dot product is less than 0.
		const register __m128	mask( ::_mm_cmplt_ps( ::_mm_dp_ps( quaternion0, quaternion1, 0xFF ), ::_mm_set1_ps( 0.0f ) ) );

		// Negate the initial quaternion if necessary. This is done in a branchless way with masking.
		quaternion0 = ::_mm_or_ps( ::_mm_and_ps( mask, ::_mm_mul_ps( quaternion0, ::_mm_set1_ps( -1.0f ) ) ), ::_mm_andnot_ps( mask, quaternion0 ) );

		// Perform a conventional linear interpolation on the coefficients of the quaternion...
		register const __m128	interpolatedQuaternion( ::_mm_add_ps( quaternion0, ::_mm_mul_ps( ::_mm_sub_ps( quaternion1, quaternion0 ), ::_mm_set_ps1( alpha ) ) ) );
		// ... then normalize the result to remove any undesired scaling.
		return ::_mm_mul_ps( ::_mm_rsqrt_ps( ::_mm_dp_ps( interpolatedQuaternion, interpolatedQuaternion, 0xFF ) ), interpolatedQuaternion );
	}

// ---------------------------------------------------

	ETInlineHint ETNoAliasHint __m128 DecompressQuaternion( const CompressedUnitQuaternion quaternion ) {
		ENUM_CLASS( unsigned int ) {
			X_COMPONENT = 0,
			Y_COMPONENT,
			Z_COMPONENT,
			W_COMPONENT
		};

	// ---

		// Load all the components into an XMM register. Compressed quaternions are 10-bit unsigned normalized integers with two bits indicating the reconstructed component.
		// Constant term is 1/1024 and will convert from a whole number to a fraction.
		register __m128	components( ::_mm_mul_ps( ::_mm_cvtepi32_ps( ::_mm_set_epi32( 0, static_cast<int>(quaternion.component2), static_cast<int>(quaternion.component1), static_cast<int>(quaternion.component0) ) ), ::_mm_set_ps1( 0.0009765625f ) ) );
		// Reconstruct the fourth component based on the remaining magnitude of the quaternion (since length normalized, formula is simply 1 - <length(components)>), then place it into the low-order word.
		components = ::_mm_blend_ps( components, ::_mm_sub_ss( ::_mm_set_ss( 1.0f ), ::_mm_sqrt_ss( ::_mm_dp_ps( components, components, 0xFF ) ) ), 1 );

		// Permute the register according to the encoding scheme.
		switch( quaternion.reconstructedComponentIndex ) {
			// Element order: <Reconstructed>, component0, component1, component2
		case X_COMPONENT: { return ::_mm_shuffle_ps( components, components, _MM_SHUFFLE( 3, 2, 1, 0 ) ); }
			// Element order: component0, <Reconstructed>, component1, component2
		case Y_COMPONENT: { return ::_mm_shuffle_ps( components, components, _MM_SHUFFLE( 3, 2, 0, 1 ) ); }
			// Element order: component0, component1, <Reconstructed>, component2
		case Z_COMPONENT: { return ::_mm_shuffle_ps( components, components, _MM_SHUFFLE( 3, 0, 2, 1 ) ); }
			// Element order: component0, component1, component2, <Reconstructed>
		case W_COMPONENT: { return ::_mm_shuffle_ps( components, components, _MM_SHUFFLE( 0, 3, 2, 1 ) ); }
		default: ETNoDefaultCaseHint;
		};
	}

}	// anonymous namespace

namespace Eldritch2 {
namespace Animation {

	void SSE4SkeletalAnimator::AnimateKeys( const uint32 localAnimationTime, const Range<const CachedQuaternionAnimationKey*> keys ) {
		for( const CachedQuaternionAnimationKey& key : keys ) {
			LinearInterpolate( DecompressQuaternion( key.startKey ), DecompressQuaternion( key.endKey ), static_cast<float32>(localAnimationTime - key.startTime) * key.reciprocalLength );
		}
	}

}	// namespace Animation
}	// namespace Eldritch2