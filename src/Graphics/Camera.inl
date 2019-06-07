/*==================================================================*\
  Camera.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2019 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {

	ETConstexpr ETForceInlineHint Camera::Camera(Transformation localToWorld, Angle verticalFov) ETNoexceptHint : _localToWorld(localToWorld), _verticalFov(verticalFov) {}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint Angle Camera::GetHorizontalFov(float32 aspectRatio) const ETNoexceptHint {
		return _verticalFov * aspectRatio;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint Angle Camera::GetVerticalFov() const ETNoexceptHint {
		return _verticalFov;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint void Camera::SetVerticalFov(Angle fov) ETNoexceptHint {
		_verticalFov = fov;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint Transformation ETSimdCall Camera::GetLocalToWorld() const ETNoexceptHint {
		return _localToWorld;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint void ETSimdCall Camera::SetLocalToWorld(Transformation transform) ETNoexceptHint {
		_localToWorld = transform;
	}

}} // namespace Eldritch2::Graphics
