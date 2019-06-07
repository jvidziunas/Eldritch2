/*==================================================================*\
  AnimationApi.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Animation {

	ETInlineHint ETForceInlineHint Span<const SoaTransformation*> ArmatureDefinition::GetBindPose() const ETNoexceptHint {
		return { reinterpret_cast<const SoaTransformation*>(_bindPose.Begin()), reinterpret_cast<const SoaTransformation*>(_bindPose.End()) };
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint Span<const ArmatureDefinition::Joint*> ArmatureDefinition::GetJoints() const ETNoexceptHint {
		return { _joints.Begin(), _joints.End() };
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint Armature::Armature(Transformation localToWorld) ETNoexceptHint : _localToWorld(localToWorld) {}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint Transformation ETSimdCall Armature::GetLocalToWorld() const ETNoexceptHint {
		return _localToWorld;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint void ETSimdCall Armature::SetLocalToWorld(Transformation localToWorld) ETNoexceptHint {
		_localToWorld = localToWorld;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint Clip::Clip(MicrosecondTime startTime) ETNoexceptHint : _startTime(startTime) {}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint MicrosecondTime Clip::GetStartTime() const ETNoexceptHint {
		return _startTime;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint void Clip::SetStartTime(MicrosecondTime worldTime) ETNoexceptHint {
		_startTime = worldTime;
	}

}} // namespace Eldritch2::Animation
