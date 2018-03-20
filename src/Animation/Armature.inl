/*==================================================================*\
  Armature.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Animation {

	ETInlineHint Transformation Armature::GetLocalToWorld() const {
		return _localToWorld;
	}

// ---------------------------------------------------

	ETInlineHint Transformation Armature::GetWorldToLocal() const {
		return { -_localToWorld.translation, -_localToWorld.rotation };
	}

}	// namespace Animation
}	// namespace Eldritch2