/*==================================================================*\
  AnimationTypes.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/AnimateSkeleton_generated.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Animation {

	template <uint32 knotCount>
	ETInlineHint void KnotCache::PushKnots(uint32 index, const float32 (&startTimes)[knotCount], const float32 (&endTimes)[knotCount], const uint16 (&startValues)[knotCount], const uint16 (&endValues)[knotCount], const uint16 (&startTangents)[knotCount], const uint16 (&endTangents)[knotCount]) {
		KnotCache::PushKnots(index, knotCount, startTimes, endTimes, startValues, endValues, startTangents, endTangents);
	}

	// ---------------------------------------------------

	ETInlineHint void KnotCache::PushKnots(uint32 index, uint32 knotCount, const float32* startTimes, const float32* endTimes, const uint16* startValues, const uint16* endValues, const uint16* startTangents, const uint16* endTangents) {
		float32* reciprocalDurations(ETStackAlloc(float32, knotCount));

		for (uint32 i(0u); i < knotCount; ++i) {
			reciprocalDurations[i] = Reciprocal(endTimes[i] - startTimes[i]);
		}

		CopyArrayNonTemporal(_startTimes + index, startTimes, knotCount);
		CopyArrayNonTemporal(_reciprocalDurations + index, reciprocalDurations, knotCount);
		CopyArrayNonTemporal(_startValues + index, startValues, knotCount);
		CopyArrayNonTemporal(_endValues + index, endValues, knotCount);
		CopyArrayNonTemporal(_startTangents + index, startTangents, knotCount);
		CopyArrayNonTemporal(_endTangents + index, endTangents, knotCount);
	}

	// ---------------------------------------------------

	ETInlineHint void KnotCache::PushKnot(uint32 index, float32 startTime, float32 endTime, uint16 startValue, uint16 endValue, uint16 startTangent, uint16 endTangent) {
		_startTimes[index]          = startTime;
		_reciprocalDurations[index] = Reciprocal(endTime - startTime);
		_startValues[index]         = startValue;
		_endValues[index]           = endValue;
		_startTangents[index]       = startTangent;
		_endTangents[index]         = endTangent;
	}

	// ---------------------------------------------------

	ETInlineHint const uint32* KnotCache::GetTargetChannels() const {
		return reinterpret_cast<const uint32*>(_memory.GetData());
	}

	// ---------------------------------------------------

	ETInlineHint const float32* KnotCache::GetStartTimes() const {
		return _startTimes;
	}

	// ---------------------------------------------------

	ETInlineHint const float32* KnotCache::GetReciprocalDurations() const {
		return _reciprocalDurations;
	}

	// ---------------------------------------------------

	ETInlineHint const uint16* KnotCache::GetStartValues() const {
		return _startValues;
	}

	// ---------------------------------------------------

	ETInlineHint const uint16* KnotCache::GetEndValues() const {
		return _endValues;
	}

	// ---------------------------------------------------

	ETInlineHint const uint16* KnotCache::GetStartTangents() const {
		return _startTangents;
	}

	// ---------------------------------------------------

	ETInlineHint const uint16* KnotCache::GetEndTangents() const {
		return _endTangents;
	}

	// ---------------------------------------------------

	ETInlineHint uint32 KnotCache::GetSize() const {
		return _count;
	}

	// ---------------------------------------------------

	ETInlineHint uint32 KnotCache::AllocateKnots(uint32 knotCount, const uint32* channelIndices) {
		const uint32 firstKnot(eastl::exchange(_count, _count + knotCount));
		if (_capacity < _count) {
			Resize(_capacity + 2u * knotCount);
		}

		CopyArrayNonTemporal(_channelIndices + firstKnot, channelIndices, knotCount);

		return firstKnot;
	}

	// ---------------------------------------------------

	template <uint32 knotCount>
	ETInlineHint uint32 KnotCache::AllocateKnots(const uint32 (&channelIndices)[knotCount]) {
		return this->AllocateKnots(knotCount, channelIndices);
	}

	// ---------------------------------------------------

	ETInlineHint void Clip::Attach(KnotCache& knots, uint32 knotCount) {
		_firstKnot = knots.AllocateKnots(knotCount, nullptr);
	}

	// ---------------------------------------------------

	ETInlineHint void EvaluatePose(Transformation poseToLocals[], Transformation poseToVelocity[], float32 scratch[], const Blend& blend, const KnotCache& knots, BoneIndex maximumBone, float32 time) {
		enum : size_t { FloatsPerTransform = sizeof(Transformation) / sizeof(float32) };

		const size_t   floatsPerPose(FloatsPerTransform * maximumBone);
		float32* const palette(scratch + floatsPerPose);
		float32* const velocityPalette(scratch + 2u * floatsPerPose);

		ispc::BulkHermiteWithDerivative(palette, velocityPalette, knots.GetTargetChannels(), knots.GetStartTimes(), knots.GetReciprocalDurations(), knots.GetStartValues(), knots.GetEndValues(), knots.GetStartTangents(), knots.GetEndTangents(), maximumBone, time);

		static_assert(sizeof(*poseToLocals) == 2u * sizeof(float32[4]), "Size mismatch between ISPC float structure and Transform type");
		blend.Evaluate(scratch, palette, maximumBone);
		ispc::TransposeTransforms(reinterpret_cast<float32(*)[4]>(poseToLocals), scratch, maximumBone);
		blend.Evaluate(scratch, velocityPalette, maximumBone);
		ispc::TransposeTransforms(reinterpret_cast<float32(*)[4]>(poseToVelocity), scratch, maximumBone);
	}

	// ---------------------------------------------------

	ETInlineHint void EvaluatePose(Transformation poseToLocals[], float32 scratch[], const Blend& blend, const KnotCache& knots, BoneIndex maximumBone, float32 time) {
		enum : size_t { FloatsPerTransform = sizeof(Transformation) / sizeof(float32) };

		const size_t   floatsPerPose(FloatsPerTransform * maximumBone);
		float32* const palette(scratch + floatsPerPose);

		ispc::BulkHermite(palette, knots.GetTargetChannels(), knots.GetStartTimes(), knots.GetReciprocalDurations(), knots.GetStartValues(), knots.GetEndValues(), knots.GetStartTangents(), knots.GetEndTangents(), maximumBone, time);

		static_assert(sizeof(*poseToLocals) == 2u * sizeof(float32[4]), "Size mismatch between ISPC float structure and Transform type");
		blend.Evaluate(scratch, palette, maximumBone);
		ispc::TransposeTransforms(reinterpret_cast<float32(*)[4]>(poseToLocals), scratch, 0u);
	}

}} // namespace Eldritch2::Animation
