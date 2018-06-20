/*==================================================================*\
  CharacterPhysics.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/CharacterPhysics.hpp>
#include <Physics/PhysX/PhysxMarshals.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Physics {
		namespace PhysX {

			using namespace ::Eldritch2::Animation;
			using namespace ::physx;

			CharacterPhysics::CharacterPhysics(
				PhysxPointer<PxController> controller,
				Armature& target
			) : _controller(eastl::move(controller)),
				_target(&target),
				_filters() {
				_controller->setUserData(this);
			}

		// ---------------------------------------------------

			void CharacterPhysics::Move(Vector displacement) {
				const auto	newCollisionFlags(_controller->move(AsPxVec3(displacement), 0.1f, (1.0f / 60.0f), _filters));
				const auto	delta(newCollisionFlags ^ _collisionFlags);

				_collisionFlags = newCollisionFlags;
			}

		}	// namespace PhysX
	}	// namespace Physics
}	// namespace Eldritch2