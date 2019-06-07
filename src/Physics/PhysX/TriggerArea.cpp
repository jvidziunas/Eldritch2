/*==================================================================*\
  TriggerArea.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/TriggerArea.hpp>
//------------------------------------------------------------------//
/*	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does.
 *	We can't fix this, but we can at least disable the warning. */
ET_PUSH_MSVC_WARNING_STATE(disable : 6326)
#include <PxRigidStatic.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {

	using namespace ::physx;

	TriggerArea::TriggerArea(PhysxPointer<PxRigidStatic> actor, PhysxPointer<PxShape> shape) ETNoexceptHint : _actor(Move(actor)), _collisionShape(Move(shape)) {}

	// ---------------------------------------------------

	void TriggerArea::Enable() {
		_actor->attachShape(*_collisionShape);
	}

	// ---------------------------------------------------

	void TriggerArea::Disable() {
		_actor->detachShape(*_collisionShape);
	}

}}} // namespace Eldritch2::Physics::PhysX
