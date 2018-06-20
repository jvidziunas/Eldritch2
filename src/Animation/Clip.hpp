/*==================================================================*\
  Clip.hpp
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

	class ETPureAbstractHint Clip {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref Clip instance.
		Clip(const Clip&) = delete;
		//! Constructs this @ref Clip instance.
		Clip() = default;

		virtual ~Clip() = default;

		// ---------------------------------------------------

	public:
		virtual void PrefetchTransforms(uint64 timeBegin, uint64 timeEnd) abstract;

		virtual void EvaluateWorldPose(Transformation localToWorld, ispc::GpuTransformWithVelocity transforms[]) const abstract;
		virtual void EvaluateWorldPose(Transformation localToWorld, ispc::GpuTransform transforms[]) const abstract;
	};

}} // namespace Eldritch2::Animation
