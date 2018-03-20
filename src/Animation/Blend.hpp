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

namespace Eldritch2 {
	class	Transformation;
}

namespace ispc {
	struct	GpuTransformWithVelocity;
	struct	GpuTransform;
}

namespace Eldritch2 {
namespace Animation {

	class ETPureAbstractHint Blend {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Blend instance.
		Blend( const Blend& ) = default;
	//!	Constructs this @ref Blend instance.
		Blend() = default;

		~Blend() = default;

	// ---------------------------------------------------

	public:
		virtual void	EvaluateGlobalPose( Transformation localToWorld, BoneIndex maximumBoneToAnimate, ispc::GpuTransformWithVelocity transforms[] ) const abstract;
		virtual void	EvaluateGlobalPose( Transformation localToWorld, BoneIndex maximumBoneToAnimate, ispc::GpuTransform transforms[] ) const abstract;

		virtual void	EvaluateLocalPose( BoneIndex maximumBoneToAnimate, ispc::GpuTransformWithVelocity transforms[] ) const abstract;
		virtual void	EvaluateLocalPose( BoneIndex maximumBoneToAnimate, ispc::GpuTransform transforms[] ) const abstract;
	};

}	// namespace Animation
}	// namespace Eldritch2