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
#include <Animation/AnimationTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Animation { namespace AssetViews {
	class KeyframeClipAsset;
}}} // namespace Eldritch2::Animation::AssetViews

namespace Eldritch2 { namespace Animation {

	class KeyframeClip : public Animation::Clip {
		// - TYPE PUBLISHING ---------------------------------

	public:
		enum class LoopMode : uint8 {
			Hold,
			Repeat,
			Mirror,
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref KeyframeClip instance.
		KeyframeClip(Allocator& allocator, const AssetViews::KeyframeClipAsset& asset);
		//! Constructs this @ref KeyframeClip instance.
		KeyframeClip(const KeyframeClip&) = default;

		~KeyframeClip() = default;

		// ---------------------------------------------------

	public:
		void SetStartTimestamp(uint64 worldTime);

		void SetPlaybackRate(float32 rate);

		// ---------------------------------------------------

	public:
		float32 AsLocalTime(uint64 globalTime) const;

		// ---------------------------------------------------

	public:
		void FetchKnots(KnotCache& knots, BoneIndex maximumBone, uint64 time) override;

		// - DATA MEMBERS ------------------------------------

	private:
		const AssetViews::KeyframeClipAsset* _asset;
		uint64                               _startTimestamp;
		uint32                               _duration;
		LoopMode                             _loop[2];
		float32                              _inverseLength;
	};

}} // namespace Eldritch2::Animation

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Animation/KeyframeClip.inl>
//------------------------------------------------------------------//
