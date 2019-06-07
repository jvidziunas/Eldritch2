/*==================================================================*\
  KeyframeClip.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/AnimationApi.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Animation { namespace AssetViews {
	class KeyframeClipAsset;
}}} // namespace Eldritch2::Animation::AssetViews

namespace Eldritch2 { namespace Animation {

	class KeyframeClip : public Animation::Clip {
		// - TYPE PUBLISHING ---------------------------------

	public:
		struct Interval {
			ETConstexpr bool Covers(ClipTime time) const ETNoexceptHint;

			ClipTime begin;
			ClipTime end;
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref KeyframeClip instance.
		ETConstexpr KeyframeClip(MicrosecondTime startTime, const AssetViews::KeyframeClipAsset& asset) ETNoexceptHint;
		//! Constructs this @ref KeyframeClip instance.
		KeyframeClip(const KeyframeClip&) ETNoexceptHint = default;

		~KeyframeClip() = default;

		// ---------------------------------------------------

	public:
		ETConstexpr ClipTime AsLocalTime(MicrosecondTime worldTime) const ETNoexceptHint;

		void Evaluate(MicrosecondTime worldTime, BoneIndex maximumBone, SoaTransformation pose[]) ETNoexceptHint override;

		// ---------------------------------------------------

	public:
		ETConstexpr void SetPlaybackRate(float32 rate) ETNoexceptHint;

		ETConstexpr void SetLoopMode(LoopMode mode) ETNoexceptHint;

		// ---------------------------------------------------

	private:
		void UpdateCache(ClipTime time) ETNoexceptHint;

		// - DATA MEMBERS ------------------------------------

	private:
		float32                              _playbackRate;
		LoopMode                             _loop;
		const AssetViews::KeyframeClipAsset* _asset;
		Interval                             _validPeriod;
	};

}} // namespace Eldritch2::Animation

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Animation/KeyframeClip.inl>
//------------------------------------------------------------------//
