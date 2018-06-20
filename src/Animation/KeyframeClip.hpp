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
#include <Animation/Clip.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Animation { namespace AssetViews {
	class KeyframeClipAsset;
}}} // namespace Eldritch2::Animation::AssetViews

namespace Eldritch2 { namespace Animation {
	namespace Detail {

		class ETPureAbstractHint AbstractKeyframeClip : public Animation::Clip {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref AbstractKeyframeClip instance.
			AbstractKeyframeClip(const AssetViews::KeyframeClipAsset& asset);
			//! Constructs this @ref AbstractKeyframeClip instance.
			AbstractKeyframeClip(const AbstractKeyframeClip&) = default;

			~AbstractKeyframeClip() = default;

			// ---------------------------------------------------

		public:
			void SetStartTimestamp(uint64 worldTime);

			// - DATA MEMBERS ------------------------------------

		private:
			const AssetViews::KeyframeClipAsset* _asset;
			uint64                               _startTimestamp;
			float32                              _playbackRate;
			float32                              _inverseLength;
		};

	} // namespace Detail

	template <class Keyframe>
	class KeyframeClip : public Detail::AbstractKeyframeClip {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using KeyframeType = Keyframe;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref KeyframeClip instance.
		KeyframeClip(Allocator& allocator, const AssetViews::KeyframeClipAsset& asset);
		//! Constructs this @ref KeyframeClip instance.
		KeyframeClip(const KeyframeClip&) = default;

		~KeyframeClip() = default;

		// ---------------------------------------------------

	public:
		void PrefetchTransforms(uint64 timeBegin, uint64 timeEnd) override;

		void EvaluateWorldPose(Transformation localToWorld, ispc::GpuTransformWithVelocity transforms[]) const override;
		void EvaluateWorldPose(Transformation localToWorld, ispc::GpuTransform transforms[]) const override;

		// - DATA MEMBERS ------------------------------------

	private:
		uint16 _beginTime;
		uint16 _endTime;
	};

}} // namespace Eldritch2::Animation

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Animation/KeyframeClip.inl>
//------------------------------------------------------------------//
