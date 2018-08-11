/*==================================================================*\
  KeyframeClip.inl
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

	ETInlineHint KeyframeClip::KeyframeClip(Allocator& /*allocator*/, const AssetViews::KeyframeClipAsset& asset) :
		_asset(eastl::addressof(asset)) {}

	// ---------------------------------------------------

	ETInlineHint float32 KeyframeClip::AsLocalTime(uint64 globalTime) const {
		return AsFloat(Min(globalTime, _startTimestamp) - _startTimestamp) * _inverseLength;
	}

	// ---------------------------------------------------

	ETInlineHint void KeyframeClip::SetStartTimestamp(uint64 worldTime) {
		_startTimestamp = worldTime;
	}

	// ---------------------------------------------------

	ETInlineHint void KeyframeClip::SetPlaybackRate(float32 rate) {
		_inverseLength = rate != 0.0f ? Reciprocal(AsFloat(_duration) * rate) : 0.0f;
	}

}} // namespace Eldritch2::Animation
