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
#include <Utility/Math/StandardLibrary.hpp>
#include <Utility/MPL/MatrixTypes.hpp>
#include <Utility/MPL/AngleTypes.hpp>
#include <Renderer/Camera.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Renderer {

	Camera::Camera( RigidTransform transform, RadianMeasure horizontalFovAngle, float32 aspectRatio ) : _transform( transform ), _horizontalFov( horizontalFovAngle ), _verticalFov( horizontalFovAngle * aspectRatio ) {}

// ---------------------------------------------------
	
	RadianMeasure Camera::GetHorizontalFov() const {
		return _horizontalFov;
	}

// ---------------------------------------------------

	RadianMeasure Camera::GetVerticalFov() const {
		return _verticalFov;
	}

// ---------------------------------------------------

	void Camera::SetHorizontalFov( DegreeMeasure newFOV ) {
		_horizontalFov = newFOV;
	}

// ---------------------------------------------------

	void Camera::SetHorizontalFov( RadianMeasure newFOV ) {
		_horizontalFov = newFOV;
	}

// ---------------------------------------------------

	void Camera::SetVerticalFov( DegreeMeasure newFOV ) {
		_verticalFov = newFOV;
	}

// ---------------------------------------------------

	void Camera::SetVerticalFov( RadianMeasure newFOV ) {
		_verticalFov = newFOV;
	}

// ---------------------------------------------------

	void Camera::SetHorizontalAndVerticalFov( RadianMeasure horizontalFov, float32 aspectRatio ) {
		_horizontalFov	= horizontalFov;
		_verticalFov	= horizontalFov * aspectRatio;
	}

// ---------------------------------------------------

	void Camera::SetHorizontalAndVerticalFov( DegreeMeasure horizontalFOV, float32 aspectRatio ) {
		_horizontalFov	= horizontalFOV;
		_verticalFov	= horizontalFOV * aspectRatio;
	}

// ---------------------------------------------------

	RigidTransform ETSimdCall Camera::GetTransform() const {
		return _transform;
	}

// ---------------------------------------------------

	void ETSimdCall Camera::SetTransform( RigidTransform transform ) {
		_transform = transform;
	}

// ---------------------------------------------------

	Float4x4 Camera::GetInverseViewProjectionMatrix( float32 nearPlane, float32 farPlane ) const {
		return GetInverseViewMatrix() * Float4x4::Perspective( _horizontalFov, _verticalFov, nearPlane, farPlane );
	}

// ---------------------------------------------------

	Float4x4 Camera::GetViewProjectionMatrix( float32 nearPlane, float32 farPlane ) const {
		return GetViewMatrix() * Float4x4::Perspective( _horizontalFov, _verticalFov, nearPlane, farPlane );
	}

// ---------------------------------------------------

	Float4x4 Camera::GetInverseViewMatrix() const {
		return Float4x4::RigidTransformation( -_transform.translation, _transform.orientation.GetReverse() );
	}

// ---------------------------------------------------

	Float4x4 Camera::GetViewMatrix() const {
		return Float4x4::RigidTransformation( _transform );
	}

}	// namespace Renderer
}	// namespace Eldritch2