/*==================================================================*\
  LocalSpaceAnimationLayer.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/LocalSpaceAnimationLayer.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Animation;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Animation {

	LocalSpaceAnimator::LocalSpaceAnimator( const uint64 animationPeriodOffset, const uint32 animationPeriod ) : _animationPeriodOffset( animationPeriodOffset ), _animationPeriod( animationPeriod ) {}

// ---------------------------------------------------

	void LocalSpaceAnimator::EvaluateForTime( const uint16 /*maximumBoneToConsider*/, const uint64 time ) {
		const uint32	animationTime( (time - _animationPeriodOffset) % _animationPeriod );
	}

// ---------------------------------------------------

	LocalSpaceAdditiveKeyframeAnimationLayer::LocalSpaceAdditiveKeyframeAnimationLayer( const uint64 animationPeriodOffset, const uint32 animationPeriod, Armature::ReplacementKeyframeAnimationLayer& influencedLayer ) : LocalSpaceAnimator( animationPeriodOffset, animationPeriod ) {
		influencedLayer.AttachAnimationLayer( *this );
	}

// ---------------------------------------------------

	void LocalSpaceAdditiveKeyframeAnimationLayer::EvaluatePoseForTime( const uint16 maximumBoneToConsider, const uint64 time ) {
		LocalSpaceAnimator::EvaluateForTime( maximumBoneToConsider, time );
	}

// ---------------------------------------------------

	LocalSpaceReplacementKeyframeAnimationLayer::LocalSpaceReplacementKeyframeAnimationLayer( const uint64 animationPeriodOffset, const uint32 animationPeriod, Armature& influencedArmature ) : LocalSpaceAnimator( animationPeriodOffset, animationPeriod ) {
		influencedArmature.AttachAnimationLayer( *this );
	}

// ---------------------------------------------------

	void LocalSpaceReplacementKeyframeAnimationLayer::EvaluatePoseForTime( const uint16 maximumBoneToConsider, const uint64 time ) {
		LocalSpaceAnimator::EvaluateForTime( maximumBoneToConsider, time );
	}

}	// namespace Animation
}	// namespace Eldritch2