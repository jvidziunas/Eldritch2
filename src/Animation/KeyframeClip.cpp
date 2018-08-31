/*==================================================================*\
  KeyframeClip.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/AssetViews/KeyframeClipAsset.hpp>
#include <Animation/KeyframeClip.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Animation {

	using namespace ::Eldritch2::Animation::AssetViews;

	void KeyframeClip::FetchKnots(KnotCache& knots, BoneIndex maximumBone, uint64 time) {
		const float32 localTime(AsLocalTime(time));

		for (BoneIndex bone(0u); bone < maximumBone; ++bone) {
			//	knots.PushChannel();
		}
	}

}} // namespace Eldritch2::Animation
