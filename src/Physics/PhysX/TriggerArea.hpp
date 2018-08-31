/*==================================================================*\
  TriggerArea.hpp
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
/*	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does.
 *		We can't fix this, but we can at least disable the warning. */
ET_PUSH_MSVC_WARNING_STATE(disable : 6326)
#include <PxRigidStatic.h>
#include <PxShape.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {

	class TriggerArea {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref TriggerArea instance.
		TriggerArea(PhysxPointer<physx::PxRigidStatic> actor, PhysxPointer<physx::PxShape> shape);
		//!	Disable copy construction.
		TriggerArea(const TriggerArea&) = delete;

		~TriggerArea() = default;

		// ---------------------------------------------------

	public:
		void Enable();

		void Disable();

		// ---------------------------------------------------

		//!	Disable copy assignment.
		TriggerArea& operator=(const TriggerArea&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		const PhysxPointer<physx::PxRigidStatic> _actor;
		PhysxPointer<physx::PxShape>             _collisionShape;
	};

}}} // namespace Eldritch2::Physics::PhysX
