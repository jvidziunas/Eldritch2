/*==================================================================*\
  SmartPointers.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace Detail {

	template <typename PhysXObject>
	ETInlineHint void ObjectDeleter<PhysXObject>::operator()( PhysXObject* const object ) {
		if( ETBranchUnlikelyHint( nullptr == object ) ) {
			return;
		}

		object->release();
	}

}	// namespace Detail
}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2

