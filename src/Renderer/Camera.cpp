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

	Camera::Camera() : _nearDepth( 0.1f ), _farDepth( 1000.0f ), /* 80-degree horizontal FOV */ _horizontalFOVInRadians( 0.44444f * 3.14159265358972f ), /* 16:9 aspect ratio */ _verticalFOVInRadians( _horizontalFOVInRadians * 0.5625f ) {}

// ---------------------------------------------------
	
	RadianMeasure Camera::GetHorizontalFOV() const {
		return RadianMeasure( _horizontalFOVInRadians );
	}

// ---------------------------------------------------

	RadianMeasure Camera::GetVerticalFOV() const {
		return RadianMeasure( _verticalFOVInRadians );
	}

// ---------------------------------------------------

	void Camera::SetHorizontalFOV( const DegreeMeasure newFOV ) {
		_horizontalFOVInRadians = newFOV;
	}

// ---------------------------------------------------

	void Camera::SetHorizontalFOV( const RadianMeasure newFOV ) {
		_horizontalFOVInRadians = newFOV;
	}

// ---------------------------------------------------

	void Camera::SetVerticalFOV( const DegreeMeasure newFOV ) {
		_verticalFOVInRadians = newFOV;
	}

// ---------------------------------------------------

	void Camera::SetVerticalFOV( const RadianMeasure newFOV ) {
		_verticalFOVInRadians = newFOV;
	}

// ---------------------------------------------------

	void Camera::GetHorizontalAndVerticalFOV( RadianMeasure& horizontalFOV, RadianMeasure& verticalFOV ) const {
		horizontalFOV	= _horizontalFOVInRadians;
		verticalFOV		= _verticalFOVInRadians;
	}

// ---------------------------------------------------

	void Camera::GetHorizontalAndVerticalFOV( DegreeMeasure& horizontalFOV, DegreeMeasure& verticalFOV ) const {
		horizontalFOV	= _horizontalFOVInRadians;
		verticalFOV		= _verticalFOVInRadians;
	}

// ---------------------------------------------------

	void Camera::SetHorizontalAndVerticalFOV( const RadianMeasure horizontalFOV, const RadianMeasure verticalFOV ) {
		_horizontalFOVInRadians	= horizontalFOV;
		_verticalFOVInRadians	= verticalFOV;
	}

// ---------------------------------------------------

	void Camera::SetHorizontalAndVerticalFOV( const DegreeMeasure horizontalFOV, const DegreeMeasure verticalFOV ) {
		_horizontalFOVInRadians	= horizontalFOV;
		_verticalFOVInRadians	= verticalFOV;
	}

// ---------------------------------------------------

	void Camera::SetHorizontalAndVerticalFOV( const RadianMeasure horizontalFOV, const float32 aspectRatio ) {
		RadianMeasure	verticalFOV( horizontalFOV * aspectRatio );

		_horizontalFOVInRadians	= horizontalFOV;
		_verticalFOVInRadians	= verticalFOV;
	}

// ---------------------------------------------------

	void Camera::SetHorizontalAndVerticalFOV( const DegreeMeasure horizontalFOV, const float32 aspectRatio ) {
		DegreeMeasure	verticalFOV( horizontalFOV * aspectRatio );

		_horizontalFOVInRadians	= horizontalFOV;
		_verticalFOVInRadians	= verticalFOV;
	}

// ---------------------------------------------------

	Float4 Camera::GetPosition() const {
		return _position;
	}

// ---------------------------------------------------

	void Camera::SetPosition( const Float4 newPosition ) {
		_position = newPosition;
	}

// ---------------------------------------------------

	Quaternion Camera::GetOrientation() const {
		return _orientation;
	}

// ---------------------------------------------------

	void Camera::SetOrientation( const Quaternion newOrientation ) {
		_orientation = newOrientation;
	}

// ---------------------------------------------------

#if 0
	void Camera::GetOrientation( RadianMeasure& rotationX, RadianMeasure& rotationY, RadianMeasure& rotationZ ) const {
		const Quaternion	heading( GetOrientation() );

		rotationX = atan2(  2.0f * ( heading.y() * heading.z() + heading.w() * heading.x() ), heading.w() * heading.w() - heading.x() * heading.x() - heading.y() * heading.y() + heading.z() * heading.z() );
		rotationY = asin ( -2.0f * ( heading.x() * heading.z() - heading.w() * heading.y() ) );
		rotationZ = atan2(  2.0f * ( heading.x() * heading.y() + heading.w() * heading.z() ), heading.w() * heading.w() + heading.x() * heading.x() - heading.y() * heading.y() - heading.z() * heading.z() );
	}

// ---------------------------------------------------

	void Camera::SetOrientation( const Float4& lookAt, const Float4& up ) {
		const Float4	finalLookAt( ( ( GetPosition() - lookAt ).normalized() ) );
		Quaternion&		heading( _transform.current.realPart );
		Float4			finalRight;
		Float4			finalUp( up.normalized() );
		float32			scale;

		finalRight.block<3, 1>( 0, 0 ) = finalLookAt.block<3, 1>( 0, 0 ).cross( finalUp.block<3, 1>( 0, 0 ) );
		finalUp.block<3, 1>( 0, 0 ) = finalRight.block<3, 1>( 0, 0 ).cross( finalRight.block<3, 1>( 0, 0 ) );

		float3x3 viewMatrix;
		viewMatrix <<	 finalRight.x(),	 finalRight.y(),	 finalRight.z(),
		finalUp.x(),		 finalUp.y(),		 finalUp.z(),
		-finalLookAt.x(),	-finalLookAt.y(),	-finalLookAt.z();

		heading.w()	= sqrtf( 1.0f + finalRight.x() + finalUp.y() + -finalLookAt.z() ) * 0.5f;
		scale = Reciprocal( 4.0f * heading.w() );
		heading.x()	= ( -finalLookAt.y() - finalUp.z() ) * scale;
		heading.y()	= (  finalRight.z() + finalLookAt.x() ) * scale;
		heading.z()	= (  finalUp.x() - finalRight.y() ) * scale;
	}
#endif // 0

// ---------------------------------------------------

	void Camera::SetNearPlane( const float32 newDepth ) {
		_nearDepth = newDepth;
	}

// ---------------------------------------------------

	float32 Camera::GetNearPlane() const {
		return _nearDepth;
	}

// ---------------------------------------------------

	void Camera::SetFarPlane( const float32 newDepth ) {
		_farDepth = newDepth;
	}

// ---------------------------------------------------

	float32 Camera::GetFarPlane() const {
		return _farDepth;
	}

// ---------------------------------------------------

#if 0
	void Camera::ComputeWorldSpacePickingEndpoints( Float4& startPoint, Float4& endPoint, const Float2& screenCoordinates ) {
		const Float4x4	inverseViewProjection( ComputePerspectiveViewProjectionMatrix().inverse() );
		Float2			screenConv( screenCoordinates.cwiseProduct( Float2( 2.0f, 2.0f ) ) - Float2( 1.0f, 1.0f ) );
		Float4			temp( screenConv.x(), screenConv.y(), _nearDepth, 1.0f );

		startPoint = temp * inverseViewProjection;

		temp.z() = _farDepth;

		endPoint = temp * inverseViewProjection;
	}
#endif

// ---------------------------------------------------

	Float4x4 Camera::ComputeViewMatrix() const {
		return Float4x4::RigidTransformation( _position, _orientation );
	}

// ---------------------------------------------------

	Float4x4 Camera::ComputePerspectiveProjectionMatrix() const {
		return ComputePerspectiveProjectionMatrix( _nearDepth, _farDepth );
	}

// ---------------------------------------------------

	Float4x4 Camera::ComputePerspectiveProjectionMatrix( const float32 nearPlaneOverride, const float32 farPlaneOverride ) const {
		return Float4x4::Perspective( RadianMeasure( _horizontalFOVInRadians ), RadianMeasure( _verticalFOVInRadians ), nearPlaneOverride, farPlaneOverride );
	}

// ---------------------------------------------------

	Float4x4 Camera::ComputePerspectiveViewProjectionMatrix() const {	
		return ComputePerspectiveViewProjectionMatrix( _nearDepth, _farDepth );
	}

// ---------------------------------------------------

	Float4x4 Camera::ComputePerspectiveViewProjectionMatrix( const float32 nearPlaneOverride, const float32 farPlaneOverride ) const {	
		return ComputeViewMatrix() *= this->ComputePerspectiveProjectionMatrix( nearPlaneOverride, farPlaneOverride );
	}

}	// namespace Renderer
}	// namespace Eldritch2