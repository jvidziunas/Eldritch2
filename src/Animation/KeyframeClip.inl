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
	namespace Detail {

		ETInlineHint void AbstractKeyframeClip::SetStartTimestamp(uint64 worldTime) {
			_startTimestamp = worldTime;
		}

	} // namespace Detail

	template <class Keyframe>
	ETInlineHint KeyframeClip<Keyframe>::KeyframeClip(Allocator& /*allocator*/, const AssetViews::KeyframeClipAsset& asset) :
		Detail::AbstractKeyframeClip(asset) {}

	// ---------------------------------------------------

	template <class Keyframe>
	ETInlineHint void KeyframeClip<Keyframe>::PrefetchTransforms(uint64 timeBegin, uint64 timeEnd) {
		//	Transform global time into local time.
		const auto localBegin(AsInt((timeBegin - _startTimestamp) * _inverseLength));
		const auto localEnd(AsInt((timeEnd - _startTimestamp) * _inverseLength));
	}

}} // namespace Eldritch2::Animation
