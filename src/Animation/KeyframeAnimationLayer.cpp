/*==================================================================*\
  KeyframeAnimationLayer.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/KeyframeAnimationResourceView.hpp>
#include <Animation/KeyframeAnimationLayer.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Animation;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Animation {

	KeyframeAnimationLayer::KeyframeAnimationLayer( KeyframeAnimationResourceView& asset ) : _asset( &asset ) {}

// ---------------------------------------------------

	void KeyframeAnimationLayer::EvaluateCacheForTime( KeyCache& /*cache*/, BoneIndex /*maximumBoneToConsider*/, uint64 /*time*/ ) {}

}	// namespace Animation
}	// namespace Eldritch2