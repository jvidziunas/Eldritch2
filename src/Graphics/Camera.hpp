/*==================================================================*\
  Camera.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
class Matrix4x4;
}

namespace Eldritch2 { namespace Graphics {

	class Camera {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref Camera instance.
		Camera(Transformation localToWorld, Angle verticalFov);
		//!	Constructs this @ref Camera instance.
		Camera(const Camera&) = default;

		~Camera() = default;

		// ---------------------------------------------------

	public:
		Transformation ETSimdCall GetLocalToWorld() const;

		Transformation ETSimdCall GetWorldToLocal() const;

		void ETSimdCall SetLocalToWorld(Transformation transform);

		// ---------------------------------------------------

	public:
		Angle GetHorizontalFov(float32 aspectRatio) const;

		Angle GetVerticalFov() const;

		void SetVerticalFov(Angle angle);

		// ---------------------------------------------------

	public:
		Matrix4x4 ETSimdCall GetInverseViewProjection(float32 aspectRatio, float32 nearPlane, float32 farPlane) const;

		Matrix4x4 ETSimdCall GetViewProjection(float32 aspectRatio, float32 nearPlane, float32 farPlane) const;

		Matrix4x4 ETSimdCall GetInverseView() const;

		Matrix4x4 ETSimdCall GetView() const;

		// - DATA MEMBERS ------------------------------------

	private:
		Transformation _localToWorld;
		Angle          _verticalFov;
	};

}} // namespace Eldritch2::Graphics
