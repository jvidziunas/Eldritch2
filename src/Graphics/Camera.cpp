/*==================================================================*\
  Camera.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Camera.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {

	Matrix ETSimdCall Camera::GetInverseViewProjection(float32 aspectRatio, float32 nearPlane, float32 farPlane) const ETNoexceptHint {
		return GetInverseView() * Matrix::AsPerspectiveProjection(_verticalFov, GetHorizontalFov(aspectRatio), nearPlane, farPlane);
	}

	// ---------------------------------------------------

	Matrix ETSimdCall Camera::GetViewProjection(float32 aspectRatio, float32 nearPlane, float32 farPlane) const ETNoexceptHint {
		return GetView() * Matrix::AsPerspectiveProjection(_verticalFov, GetHorizontalFov(aspectRatio), nearPlane, farPlane);
	}

	// ---------------------------------------------------

	Matrix ETSimdCall Camera::GetViewProjection(float32 aspectRatio, float32 nearPlane) const ETNoexceptHint {
		return GetView() * Matrix::AsPerspectiveProjection(_verticalFov, GetHorizontalFov(aspectRatio), nearPlane);
	}

	// ---------------------------------------------------

	Matrix ETSimdCall Camera::GetInverseView() const ETNoexceptHint {
		return Matrix::AsRigidTransformation(GetLocalToWorld());
	}

	// ---------------------------------------------------

	Matrix ETSimdCall Camera::GetView() const ETNoexceptHint {
		return Matrix::AsRigidTransformation(~GetLocalToWorld());
	}

}} // namespace Eldritch2::Graphics
