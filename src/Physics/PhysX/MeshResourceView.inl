/*==================================================================*\
  MeshResourceView.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/Range.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	ETInlineHint ::Eldritch2::uint16 MeshResourceView::RigidBody::GetBodyIndex() const {
		return _bodyIndex;
	}

// ---------------------------------------------------

	ETInlineHint ::physx::PxShape& MeshResourceView::RigidBody::GetShape() const {
		return *_shape.get();
	}

// ---------------------------------------------------

	ETInlineHint ::Eldritch2::Range<const MeshResourceView::RigidBody*> MeshResourceView::GetRigidBodies() const {
		return { _rigidBodies.ConstBegin(), _rigidBodies.ConstEnd() };
	}

// ---------------------------------------------------

	ETInlineHint::Eldritch2::Range<const MeshResourceView::SoftBody*> MeshResourceView::GetSoftBodies() const {
		return { _softBodies.ConstBegin(), _softBodies.ConstEnd() };
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2