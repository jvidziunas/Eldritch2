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
#include <Animation/AssetViews/KeyframeAnimationView.hpp>
#include <Animation/KeyframeAnimationLayer.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Animation {

	KeyframeAnimationLayer::KeyframeAnimationLayer( AssetViews::KeyframeAnimationView& asset ) : _asset( &asset ) {}

// ---------------------------------------------------

	void KeyframeAnimationLayer::EvaluateCacheForAlphaFactor( KeyCache& /*cache*/, BoneIndex /*maximumBoneToConsider*/, uint64 /*time*/ ) {}

}	// namespace Animation
}	// namespace Eldritch2