/*==================================================================*\
  TriggerArea.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/TriggerArea.hpp>
//------------------------------------------------------------------//
ET_PUSH_COMPILER_WARNING_STATE()
/*	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does.
 *		We can't fix this, but we can at least disable the warning. */
	ET_SET_MSVC_WARNING_STATE( disable : 6326 )
#	include <PxRigidStatic.h>
ET_POP_COMPILER_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	using namespace ::physx;

	TriggerArea::TriggerArea(
		PhysxPointer<PxRigidStatic> actor,
		PhysxPointer<PxShape> shape
	) : _actor( eastl::move( actor ) ),
		_collisionShape( eastl::move( shape ) ) {}

// ---------------------------------------------------

	void TriggerArea::Enable() {
		_actor->attachShape( *_collisionShape );
	}

// ---------------------------------------------------

	void TriggerArea::Disable() {
		_actor->detachShape( *_collisionShape );
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2