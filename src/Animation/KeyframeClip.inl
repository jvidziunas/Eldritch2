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

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Animation {

	ETConstexpr ETForceInlineHint bool KeyframeClip::Interval::Covers(ClipTime time) const ETNoexceptHint {
		return bool(time > begin) & bool(time < end);
	}

	// ---------------------------------------------------

	ETConstexpr KeyframeClip::KeyframeClip(MicrosecondTime startTime, const AssetViews::KeyframeClipAsset& asset) ETNoexceptHint : Clip(startTime),
																																   _asset(ETAddressOf(asset)),
																																   _validPeriod{ ClipTime::End, ClipTime::Start },
																																   _playbackRate(0.0f),
																																   _loop(LoopMode::Repeat) {}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint ClipTime KeyframeClip::AsLocalTime(MicrosecondTime worldTime) const ETNoexceptHint {
		switch (_loop) {
		case LoopMode::Repeat: return ClipTime(uint32(float32(uint64(worldTime) - uint64(GetStartTime())) * _playbackRate));
		case LoopMode::Hold: return ClipTime(Minimum(uint32(float32(uint64(worldTime) - uint64(GetStartTime())) * _playbackRate), uint32(ClipTime::End)));
		};
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint void KeyframeClip::SetPlaybackRate(float32 rate) ETNoexceptHint {
		_playbackRate = rate;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint void KeyframeClip::SetLoopMode(LoopMode mode) ETNoexceptHint {
		_loop = mode;
	}

}} // namespace Eldritch2::Animation
