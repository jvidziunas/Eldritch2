/*==================================================================*\
  PhysxPointer.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <common/PxBase.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX { namespace Detail {

	template <typename PhysxObject>
	ETInlineHint void Deleter::operator()(PhysxObject* const object) {
		if (ET_UNLIKELY(object == nullptr)) {
			return;
		}

		object->release();
	}

}}}} // namespace Eldritch2::Physics::PhysX::Detail
