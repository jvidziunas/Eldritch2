/*==================================================================*\
  CharacterPhysics.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/PhysxPointer.hpp>
//------------------------------------------------------------------//
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
ET_PUSH_MSVC_WARNING_STATE(disable : 6326)
#include <characterkinematic/PxController.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Animation {
	class Armature;
}} // namespace Eldritch2::Animation

namespace Eldritch2 { namespace Physics { namespace PhysX {

	class CharacterPhysics {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref CharacterPhysics instance.
		CharacterPhysics(PhysxPointer<physx::PxController> controller, Animation::Armature& target);
		//! Constructs this @ref CharacterPhysics instance.
		CharacterPhysics(const CharacterPhysics&) = delete;

		~CharacterPhysics() = default;

		// ---------------------------------------------------

	public:
		void Move(Vector displacement);

		// - DATA MEMBERS ------------------------------------

	private:
		const PhysxPointer<physx::PxController> _controller;
		Animation::Armature*                    _target;
		physx::PxControllerFilters              _filters;
		physx::PxControllerCollisionFlags       _collisionFlags;
	};

}}} // namespace Eldritch2::Physics::PhysX
