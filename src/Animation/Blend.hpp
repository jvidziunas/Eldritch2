/*==================================================================*\
  Blend.hpp
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

namespace ispc {
struct GpuTransformWithVelocity;
struct GpuTransform;
} // namespace ispc

namespace Eldritch2 { namespace Animation {

	class ETPureAbstractHint Blend {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref Blend instance.
		Blend(const Blend&) = default;
		//!	Constructs this @ref Blend instance.
		Blend() = default;

		virtual ~Blend() = default;

		// ---------------------------------------------------

	public:
		virtual void EvaluateGlobalPose(Transformation localToWorld, BoneIndex maximumBoneToAnimate, ispc::GpuTransformWithVelocity transforms[]) const abstract;
		virtual void EvaluateGlobalPose(Transformation localToWorld, BoneIndex maximumBoneToAnimate, ispc::GpuTransform transforms[]) const abstract;

		virtual void EvaluateLocalPose(BoneIndex maximumBoneToAnimate, ispc::GpuTransformWithVelocity transforms[]) const abstract;
		virtual void EvaluateLocalPose(BoneIndex maximumBoneToAnimate, ispc::GpuTransform transforms[]) const abstract;
	};

}} // namespace Eldritch2::Animation
