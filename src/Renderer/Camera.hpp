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
#include <Utility/MPL/VectorTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	RadianMeasure;
	class	DegreeMeasure;
	class	Float4x4;
}

namespace Eldritch2 {
namespace Renderer {

	class Camera {
	// - TYPE PUBLISHING ---------------------------------

	public:
		struct Viewport {
			::Eldritch2::float32	offsetX;
			::Eldritch2::float32	offsetY;
			::Eldritch2::float32	width;
			::Eldritch2::float32	height;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref Camera instance.
		Camera();
		//!	Constructs this @ref Camera instance.
		Camera( const Camera& ) = default;

		~Camera() = default;

	// ---------------------------------------------------

		::Eldritch2::RadianMeasure	GetHorizontalFOV() const;

		void						SetHorizontalFOV( const ::Eldritch2::DegreeMeasure newFOV );
		void						SetHorizontalFOV( const ::Eldritch2::RadianMeasure newFOV );

		::Eldritch2::RadianMeasure	GetVerticalFOV() const;

		void						SetVerticalFOV( const ::Eldritch2::DegreeMeasure newFOV );
		void						SetVerticalFOV( const ::Eldritch2::RadianMeasure newFOV );

	// ---------------------------------------------------

		void	GetHorizontalAndVerticalFOV( ::Eldritch2::RadianMeasure& horizontalFOV, ::Eldritch2::RadianMeasure& verticalFOV ) const;
		void	GetHorizontalAndVerticalFOV( ::Eldritch2::DegreeMeasure& horizontalFOV, ::Eldritch2::DegreeMeasure& verticalFOV ) const;

		void	SetHorizontalAndVerticalFOV( const ::Eldritch2::RadianMeasure horizontalFOV, const ::Eldritch2::RadianMeasure verticalFOV );
		void	SetHorizontalAndVerticalFOV( const ::Eldritch2::DegreeMeasure horizontalFOV, const ::Eldritch2::DegreeMeasure verticalFOV );
		void	SetHorizontalAndVerticalFOV( const ::Eldritch2::RadianMeasure horizontalFOV, const ::Eldritch2::float32 aspectRatio );
		void	SetHorizontalAndVerticalFOV( const ::Eldritch2::DegreeMeasure horizontalFOV, const ::Eldritch2::float32 aspectRatio );

	// ---------------------------------------------------

		::Eldritch2::Float4	GetPosition() const;

		void				SetPosition( const ::Eldritch2::Float4 newPosition );

	// ---------------------------------------------------

		::Eldritch2::Quaternion	GetOrientation() const;

		void					SetOrientation( const ::Eldritch2::Quaternion newOrientation );

	// ---------------------------------------------------

		void					SetNearPlane( const ::Eldritch2::float32 newDepth );

		::Eldritch2::float32	GetNearPlane() const;

		void					SetFarPlane( const ::Eldritch2::float32 newDepth );

		::Eldritch2::float32	GetFarPlane() const;

	// ---------------------------------------------------

		::Eldritch2::Float4x4	ComputeViewMatrix() const;

		::Eldritch2::Float4x4	ComputePerspectiveProjectionMatrix() const;
		::Eldritch2::Float4x4	ComputePerspectiveProjectionMatrix( const ::Eldritch2::float32 nearPlaneOverride, const ::Eldritch2::float32 farPlaneOverride ) const;

		::Eldritch2::Float4x4	ComputePerspectiveViewProjectionMatrix() const;
		::Eldritch2::Float4x4	ComputePerspectiveViewProjectionMatrix( const ::Eldritch2::float32 nearPlaneOverride, const ::Eldritch2::float32 farPlaneOverride ) const;

	// ---------------------------------------------------

	private:
		::Eldritch2::Float4		_position;
		::Eldritch2::Quaternion	_orientation;

		::Eldritch2::float32	_nearDepth;
		::Eldritch2::float32	_farDepth;

		::Eldritch2::float32	_horizontalFOVInRadians;
		::Eldritch2::float32	_verticalFOVInRadians;
	};

}	// namespace Renderer
}	// namespace Eldritch2