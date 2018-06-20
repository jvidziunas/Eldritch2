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
#include <Animation/Clip.hpp>
//------------------------------------------------------------------//
#include <PxArticulation.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Animation {
	class Armature;
}

namespace Physics { namespace PhysX { namespace AssetViews {
	class ArmatureAsset;
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
			AnimationClip(const physx::PxArticulation& articulation);
			//!	Constructs this @ref AnimationClip instance.
			AnimationClip(const AnimationClip&) = default;

			~AnimationClip() = default;

			// ---------------------------------------------------

		public:
			void PrefetchTransforms(uint64 timeBegin, uint64 timeEnd) override;

			void EvaluateWorldPose(Transformation localToWorld, ispc::GpuTransformWithVelocity transforms[]) const override;
			void EvaluateWorldPose(Transformation localToWorld, ispc::GpuTransform transforms[]) const override;

			// - DATA MEMBERS ------------------------------------

		public:
			const physx::PxArticulation* _articulation;
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref Physics instance.
		Physics(PhysxPointer<physx::PxArticulation> articulation, Animation::Armature& target);
		//! Disable copy construction.
		Physics(const Physics&) = delete;

		~Physics() = default;

		// ---------------------------------------------------

	public:
		void Attach(const AssetViews::PhysicsAsset& asset);

		void Detach(const AssetViews::PhysicsAsset& asset);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		Physics& operator=(const Physics&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		const PhysxPointer<physx::PxArticulation> _articulation;
		Animation::Armature*                      _target;
	};

}}} // namespace Eldritch2::Physics::PhysX
