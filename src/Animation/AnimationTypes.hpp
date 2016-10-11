/*==================================================================*\
  AnimationTypes.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/FloatTypes.hpp>
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	RigidTransform;
	class	Quaternion;
	class	Float4;
}

namespace Eldritch2 {
namespace Animation {

	using BoneIndex = Eldritch2::int16;
	using KeyTime	= Eldritch2::float16;

// ---

	struct CompressedUnitQuaternion {
		operator Eldritch2::Quaternion() const;

	// - DATA MEMBERS ------------------------------------

		Eldritch2::uint32	reconstructedComponentIndex : 2;
		Eldritch2::uint32	component0					: 10;
		Eldritch2::uint32	component1					: 10;
		Eldritch2::uint32	component2					: 10;
	};

// ---

	struct BoneAnimationKey {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref BoneAnimationKey instance.
		BoneAnimationKey( Animation::BoneIndex localBoneIndex, Eldritch2::RigidTransform knot0Transform, Eldritch2::RigidTransform knot1Transform, Eldritch2::float16 knot0Time, Eldritch2::float16 knot1Time );
	//!	Constructs this @ref BoneAnimationKey instance.
		BoneAnimationKey( Animation::BoneIndex localBoneIndex, Eldritch2::Quaternion knot0Orientation, Eldritch2::Quaternion knot1Orientation, Eldritch2::float16 knot0Time, Eldritch2::float16 knot1Time );
	//!	Constructs this @ref BoneAnimationKey instance.
		BoneAnimationKey( Animation::BoneIndex localBoneIndex, Eldritch2::Float4 knot0Translation, Eldritch2::Float4 knot1Translation, Eldritch2::float16 knot0Time, Eldritch2::float16 knot1Time );
	//!	Constructs this @ref BoneAnimationKey instance.
		BoneAnimationKey( const BoneAnimationKey& ) = default;

		~BoneAnimationKey() = default;

	// - DATA MEMBERS ------------------------------------

		Animation::BoneIndex	localBoneIndex;
		struct {
			Eldritch2::float16	localOrientationValues[4];
			Eldritch2::float16	localTranslationValues[3];
			Eldritch2::float16	time;
		}						sourceKnot,
								destinationKnot;
	};

}	// namespace Animation
}	// namespace Eldritch2