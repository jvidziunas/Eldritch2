/*==================================================================*\
  LocomotionAnimationLayer.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/LocomotionAnimationLayer.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Animation;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Animation {

	BipedalLocomotionAnimationLayer::BipedalLocomotionAnimationLayer( Armature& /*armature*/ ) {}

// ---------------------------------------------------

	void BipedalLocomotionAnimationLayer::EvaluatePoseForTime( const uint16 /*maximumBoneToConsider*/, const uint64 /*time*/ ) {}

// ---------------------------------------------------

	Float4 BipedalLocomotionAnimationLayer::EvaluateRootDisplacementDeltaForTime( const uint64 /*lastEvalutedTime*/, const uint64 /*currentTime*/ ) {
		return Float4( 0.0f, 0.0f, 0.0f, 0.0f );
	}

}	// namespace Animation
}	// namespace Eldritch2