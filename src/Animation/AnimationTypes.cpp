/*==================================================================*\
  AnimationTypes.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/AnimationTypes.hpp>
#include <Utility/MPL/VectorTypes.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Animation;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Animation {
	
	CompressedUnitQuaternion::operator Quaternion() const {
		/*Float4	temp( static_cast<float32>(component0), static_cast<float32>(component1), static_cast<float32>(component2), 0.0f );
		temp *= (1.0f / 1024.0f);

		switch( reconstructedComponentIndex ) {
			case 0:
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
		};*/

		return Quaternion();
	}

// ---------------------------------------------------

	BoneAnimationKey::BoneAnimationKey( BoneIndex localBoneIndex, RigidTransform knot0Transform, RigidTransform knot1Transform, float16 knot0Time, float16 knot1Time ) : localBoneIndex( localBoneIndex ) {
	//	Ensure we are always doing shortest-path interpolation.
		if( DotProduct( knot0Transform.orientation, knot1Transform.orientation ) < 0.0f ) {
			knot1Transform.orientation = -knot1Transform.orientation;
		}

		sourceKnot.localTranslationValues[0]		= knot0Transform.translation.coefficients[0];
		sourceKnot.localTranslationValues[1]		= knot0Transform.translation.coefficients[1];
		sourceKnot.localTranslationValues[2]		= knot0Transform.translation.coefficients[2];
		sourceKnot.time								= knot0Time;
		sourceKnot.localOrientationValues[0]		= knot0Transform.orientation.coefficients[0];
		sourceKnot.localOrientationValues[1]		= knot0Transform.orientation.coefficients[1];
		sourceKnot.localOrientationValues[2]		= knot0Transform.orientation.coefficients[2];
		sourceKnot.localOrientationValues[3]		= knot0Transform.orientation.coefficients[3];

		destinationKnot.localTranslationValues[0]	= knot1Transform.translation.coefficients[0];
		destinationKnot.localTranslationValues[1]	= knot1Transform.translation.coefficients[1];
		destinationKnot.localTranslationValues[2]	= knot1Transform.translation.coefficients[2];
		destinationKnot.time						= knot1Time;
		destinationKnot.localOrientationValues[0]	= knot1Transform.orientation.coefficients[0];
		destinationKnot.localOrientationValues[1]	= knot1Transform.orientation.coefficients[1];
		destinationKnot.localOrientationValues[2]	= knot1Transform.orientation.coefficients[2];
		destinationKnot.localOrientationValues[3]	= knot1Transform.orientation.coefficients[3];
	}

// ---------------------------------------------------

	BoneAnimationKey::BoneAnimationKey( BoneIndex localBoneIndex, Quaternion knot0Orientation, Quaternion knot1Orientation, float16 knot0Time, float16 knot1Time ) : localBoneIndex( localBoneIndex ) {
	//	Ensure we are always doing shortest-path interpolation.
		if( DotProduct( knot0Orientation, knot1Orientation ) < 0.0f ) {
			knot1Orientation = -knot1Orientation;
		}

		sourceKnot.localTranslationValues[0]		= sourceKnot.localTranslationValues[1] = sourceKnot.localTranslationValues[2] = 0.0f;
		sourceKnot.time								= knot0Time;
		sourceKnot.localOrientationValues[0]		= knot0Orientation.coefficients[0];
		sourceKnot.localOrientationValues[1]		= knot0Orientation.coefficients[1];
		sourceKnot.localOrientationValues[2]		= knot0Orientation.coefficients[2];
		sourceKnot.localOrientationValues[3]		= knot0Orientation.coefficients[3];

		destinationKnot.localTranslationValues[0]	= destinationKnot.localTranslationValues[1] = destinationKnot.localTranslationValues[2] = 0.0f;
		destinationKnot.time						= knot1Time;
		destinationKnot.localOrientationValues[0]	= knot1Orientation.coefficients[0];
		destinationKnot.localOrientationValues[1]	= knot1Orientation.coefficients[1];
		destinationKnot.localOrientationValues[2]	= knot1Orientation.coefficients[2];
		destinationKnot.localOrientationValues[3]	= knot1Orientation.coefficients[3];
	}

// ---------------------------------------------------

	BoneAnimationKey::BoneAnimationKey( BoneIndex localBoneIndex, Float4 knot0Translation, Float4 knot1Translation, float16 knot0Time, float16 knot1Time ) : localBoneIndex( localBoneIndex ) {
		sourceKnot.localTranslationValues[0]		= knot0Translation.coefficients[0];
		sourceKnot.localTranslationValues[1]		= knot0Translation.coefficients[1];
		sourceKnot.localTranslationValues[2]		= knot0Translation.coefficients[2];
		sourceKnot.time								= knot0Time;

		destinationKnot.localTranslationValues[0]	= knot1Translation.coefficients[0];
		destinationKnot.localTranslationValues[1]	= knot1Translation.coefficients[1];
		destinationKnot.localTranslationValues[2]	= knot1Translation.coefficients[2];
		destinationKnot.time						= knot1Time;
	}

}	// namespace Animation
}	// namespace Eldritch2

