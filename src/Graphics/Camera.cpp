/*==================================================================*\
  Camera.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Camera.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Graphics {

		Camera::Camera(Transformation localToWorld, Angle verticalFov) : _localToWorld(localToWorld), _verticalFov(verticalFov) {}

	// ---------------------------------------------------

		Angle Camera::GetHorizontalFov(float32 aspectRatio) const {
			return _verticalFov * aspectRatio;
		}

	// ---------------------------------------------------

		Angle Camera::GetVerticalFov() const {
			return _verticalFov;
		}

	// ---------------------------------------------------

		void Camera::SetVerticalFov(Angle fov) {
			_verticalFov = fov;
		}

	// ---------------------------------------------------

		Transformation ETSimdCall Camera::GetLocalToWorld() const {
			return _localToWorld;
		}

	// ---------------------------------------------------

		Transformation ETSimdCall Camera::GetWorldToLocal() const {
			return Transformation{ -_localToWorld.translation, _localToWorld.rotation.GetReverse() };
		}

	// ---------------------------------------------------

		void ETSimdCall Camera::SetLocalToWorld(Transformation transform) {
			_localToWorld = transform;
		}

	// ---------------------------------------------------

		Matrix4x4 ETSimdCall Camera::GetInverseViewProjection(float32 aspectRatio, float32 nearPlane, float32 farPlane) const {
			return GetInverseView() * Matrix4x4::AsPerspectiveProjection(_verticalFov, aspectRatio, nearPlane, farPlane);
		}

	// ---------------------------------------------------

		Matrix4x4 ETSimdCall Camera::GetViewProjection(float32 aspectRatio, float32 nearPlane, float32 farPlane) const {
			return GetView() * Matrix4x4::AsPerspectiveProjection(_verticalFov, aspectRatio, nearPlane, farPlane);
		}

	// ---------------------------------------------------

		Matrix4x4 ETSimdCall Camera::GetInverseView() const {
			return Matrix4x4::AsRigidTransformation(GetLocalToWorld());
		}

	// ---------------------------------------------------

		Matrix4x4 ETSimdCall Camera::GetView() const {
			return Matrix4x4::AsRigidTransformation(GetWorldToLocal());
		}

	}	// namespace Graphics
}	// namespace Eldritch2