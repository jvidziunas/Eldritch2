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
#include <Animation/AnimationApi.hpp>
//------------------------------------------------------------------//
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
ET_PUSH_MSVC_WARNING_STATE(disable : 6326)
#include <characterkinematic/PxController.h>
#include <PxAggregate.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 {

namespace Physics { namespace PhysX { namespace AssetViews {
	class PhysicsAsset;
}}} // namespace Physics::PhysX::AssetViews

namespace Animation {
	class Armature;
} // namespace Animation

} // namespace Eldritch2

namespace Eldritch2 { namespace Physics { namespace PhysX {

	class PhysicsClip : public Animation::Clip {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref PhysicsClip instance.
		PhysicsClip(const physx::PxAggregate& aggregate);
		//!	Constructs this @ref PhysicsClip instance.
		PhysicsClip(const PhysicsClip&) = default;

		~PhysicsClip() = default;

		// ---------------------------------------------------

	public:
		void Evaluate(MicrosecondTime worldTime, Animation::BoneIndex maximumBone, Animation::SoaTransformation pose[]) ETNoexceptHint;

		// - DATA MEMBERS ------------------------------------

	public:
		const physx::PxAggregate* _aggregate;
	};

	// ---

	class Physics {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref Physics instance.
		Physics(const AssetViews::PhysicsAsset& asset, PhysxPointer<physx::PxController> controller, PhysicsClip& clip) ETNoexceptHint;
		//! Disable copy construction.
		Physics(const Physics&) = delete;

		~Physics() = default;

		// ---------------------------------------------------

	public:
		void SetMovement(Vector displacement) ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		Physics& operator=(const Physics&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		Vector                            _moveDisplacement;
		const AssetViews::PhysicsAsset*   _asset;
		PhysxPointer<physx::PxController> _controller;
		PhysicsClip*                      _clip;
		physx::PxControllerFilters        _filters;
		physx::PxControllerCollisionFlags _collisionFlags;
	};

}}} // namespace Eldritch2::Physics::PhysX
