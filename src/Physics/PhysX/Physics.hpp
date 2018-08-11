/*==================================================================*\
  Physics.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/PhysXPointer.hpp>
#include <Animation/AnimationTypes.hpp>
//------------------------------------------------------------------//
#include <PxArticulation.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Animation {
	class Armature;
}

namespace Physics { namespace PhysX { namespace AssetViews {
	class PhysicsAsset;
}}} // namespace Physics::PhysX::AssetViews
} // namespace Eldritch2

namespace Eldritch2 { namespace Physics { namespace PhysX {

	class Physics {
		// - TYPE PUBLISHING ---------------------------------

	public:
		class AnimationClip : public Animation::Clip {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref AnimationClip instance.
			AnimationClip(PhysxPointer<physx::PxArticulation> articulation, const Animation::Armature& target);
			//!	Constructs this @ref AnimationClip instance.
			AnimationClip(const AnimationClip&) = default;

			~AnimationClip() = default;

			// ---------------------------------------------------

		public:
			void FetchKnots(Animation::KnotCache& knots, uint64 time, Animation::BoneIndex maximumBone) override;

			// - DATA MEMBERS ------------------------------------

		public:
			PhysxPointer<physx::PxArticulation> _articulation;
			const Animation::Armature*          _target;
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref Physics instance.
		Physics(AnimationClip& clip);
		//! Disable copy construction.
		Physics(const Physics&) = delete;

		~Physics() = default;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		Physics& operator=(const Physics&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		AnimationClip* _clip;
	};

}}} // namespace Eldritch2::Physics::PhysX
