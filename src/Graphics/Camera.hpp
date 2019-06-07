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
class Matrix;
} // namespace Eldritch2

namespace Eldritch2 { namespace Graphics {

	class Camera {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref Camera instance.
		ETConstexpr Camera(Transformation localToWorld, Angle verticalFov) ETNoexceptHint;
		//!	Constructs this @ref Camera instance.
		ETConstexpr Camera(const Camera&) ETNoexceptHint = default;

		~Camera() = default;

		// ---------------------------------------------------

	public:
		ETConstexpr Transformation ETSimdCall GetLocalToWorld() const ETNoexceptHint;

		ETConstexpr void ETSimdCall SetLocalToWorld(Transformation transform) ETNoexceptHint;

		// ---------------------------------------------------

	public:
		ETConstexpr Angle GetHorizontalFov(float32 aspectRatio) const ETNoexceptHint;

		ETConstexpr Angle GetVerticalFov() const ETNoexceptHint;

		ETConstexpr void SetVerticalFov(Angle angle) ETNoexceptHint;

		// ---------------------------------------------------

	public:
		Matrix ETSimdCall GetInverseViewProjection(float32 aspectRatio, float32 nearPlane, float32 farPlane) const ETNoexceptHint;

		Matrix ETSimdCall GetViewProjection(float32 aspectRatio, float32 nearPlane, float32 farPlane) const ETNoexceptHint;
		Matrix ETSimdCall GetViewProjection(float32 aspectRatio, float32 nearPlane) const ETNoexceptHint;

		Matrix ETSimdCall GetInverseView() const ETNoexceptHint;

		Matrix ETSimdCall GetView() const ETNoexceptHint;

		// - DATA MEMBERS ------------------------------------

	private:
		Transformation _localToWorld;
		Angle          _verticalFov;
	};

}} // namespace Eldritch2::Graphics

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Camera.inl>
//------------------------------------------------------------------//
