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

	ETInlineHint ETForceInlineHint Transformation ETSimdCall Armature::GetLocalToWorld() const ETNoexceptHint {
		return _localToWorld;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint Transformation ETSimdCall Armature::GetWorldToLocal() const ETNoexceptHint {
		return ~_localToWorld;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void ETSimdCall Armature::SetLocalToWorld(Transformation localToWorld) ETNoexceptHint {
		_localToWorld = localToWorld;
	}

}} // namespace Eldritch2::Animation
