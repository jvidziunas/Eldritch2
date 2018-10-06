/*==================================================================*\
  AnimationApi.inl
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

	ETInlineHint ETForceInlineHint KnotCache::ConstReference KnotCache::operator[](SizeType offset) const ETNoexceptHint {
		return _knots[offset];
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint KnotCache::Reference KnotCache::operator[](SizeType offset) ETNoexceptHint {
		return _knots[offset];
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint KnotCache::ConstPointer KnotCache::Get(SizeType /*offset*/) const ETNoexceptHint {
		return _knots.GetData();
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint KnotCache::Pointer KnotCache::Get(SizeType /*offset*/) ETNoexceptHint {
		return _knots.GetData();
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint KnotCache::SizeType KnotCache::GetSize() const ETNoexceptHint {
		return _knots.GetSize();
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint KnotCache::SizeType KnotCache::Append(SizeType count, const ChannelIndex* channels) {
		const SizeType first(_knots.GetSize());
		_knots.Reserve(first + count);

		for (SizeType knot(0u); knot < count; ++knot) {
			_knots.Append(/*start =*/0.0f, /*reciprocalDuration =*/0.0f, /*startValue =*/0u, /*endValue =*/0u, /*startTangent =*/0u, /*endTangent =*/0u, channels[knot]);
		}

		return first;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void KnotCache::Reserve(SizeType capacityHint) {
		_knots.Reserve(capacityHint);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void KnotCache::Clear() ETNoexceptHint {
		_knots.Clear();
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void EvaluatePose(BoneIndex maximumBone, float32 time, Transformation poseToLocals[], Transformation poseToVelocity[], float32 scratch[], const KnotCache& cache, const Blend& blend) {
		enum : size_t { FloatsPerTransform = sizeof(Transformation) / sizeof(float32) };
		static_assert(sizeof(*poseToLocals) == 2u * sizeof(float32[4]), "Size mismatch between ISPC float structure and Transform type");

		const size_t   floatsPerPose(FloatsPerTransform * maximumBone);
		float32* const velocityPalette(scratch + floatsPerPose);
		float32* const palette(velocityPalette + floatsPerPose);
		const auto     knots(cache.Get(0));

		ispc::BulkHermiteWithDerivative(maximumBone, time, palette, velocityPalette, eastl::get<KnotCache::TargetChannels>(knots), eastl::get<KnotCache::StartTimes>(knots), eastl::get<KnotCache::ReciprocalDurations>(knots), eastl::get<KnotCache::StartValues>(knots), eastl::get<KnotCache::EndValues>(knots), eastl::get<KnotCache::StartTangents>(knots), eastl::get<KnotCache::EndTangents>(knots));

		blend.Evaluate(maximumBone, scratch, palette);
		ispc::TransposeTransforms(maximumBone, reinterpret_cast<float32(*)[4]>(poseToLocals), scratch);
		blend.Evaluate(maximumBone, scratch, velocityPalette);
		ispc::TransposeTransforms(maximumBone, reinterpret_cast<float32(*)[4]>(poseToVelocity), scratch);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void EvaluatePose(BoneIndex maximumBone, float32 time, Transformation poseToLocals[], float32 scratch[], const KnotCache& cache, const Blend& blend) {
		enum : size_t { FloatsPerTransform = sizeof(Transformation) / sizeof(float32) };
		static_assert(sizeof(*poseToLocals) == 2u * sizeof(float32[4]), "Size mismatch between ISPC float structure and Transform type");

		float32* const palette(scratch + FloatsPerTransform * maximumBone);
		const auto     knots(cache.Get(0));

		ispc::BulkHermite(maximumBone, time, palette, eastl::get<KnotCache::TargetChannels>(knots), eastl::get<KnotCache::StartTimes>(knots), eastl::get<KnotCache::ReciprocalDurations>(knots), eastl::get<KnotCache::StartValues>(knots), eastl::get<KnotCache::EndValues>(knots), eastl::get<KnotCache::StartTangents>(knots), eastl::get<KnotCache::EndTangents>(knots));

		blend.Evaluate(maximumBone, scratch, palette);
		ispc::TransposeTransforms(maximumBone, reinterpret_cast<float32(*)[4]>(poseToLocals), scratch);
	}

}} // namespace Eldritch2::Animation
