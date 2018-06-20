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

namespace Eldritch2 { namespace Animation {

	ETInlineHint Transformation ETSimdCall Armature::GetLocalToWorld() const {
		return _localToWorld;
	}

	// ---------------------------------------------------

	ETInlineHint Transformation ETSimdCall Armature::GetWorldToLocal() const {
		return { -_localToWorld.translation, -_localToWorld.rotation };
	}

	// ---------------------------------------------------

	ETInlineHint void ETSimdCall Armature::SetLocalToWorld(Transformation localToWorld) {
		_localToWorld = localToWorld;
	}

}} // namespace Eldritch2::Animation
