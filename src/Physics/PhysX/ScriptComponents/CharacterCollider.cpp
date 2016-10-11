/*==================================================================*\
  CharacterCollider.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/ScriptComponents/CharacterCollider.hpp>
#include <Physics/PhysX/TypeMarshalers.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::physx;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace ScriptComponents {

	CharacterCollider::CharacterCollider( UniquePointer<PxCapsuleController> controller, SkeletalArmature& armature ) : _controller( eastl::move( controller ) ), _armature( armature ) {
		_controller->setUserData( this );
	}

// ---------------------------------------------------

	Float4 CharacterCollider::GetFootPosition() const {
		return Float4FromExtendedVec3( _controller->getFootPosition() );
	}

}	// namespace ScriptComponents
}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2