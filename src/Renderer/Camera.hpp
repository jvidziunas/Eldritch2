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
#include <Utility/MPL/AngleTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	Float4x4;
}

namespace Eldritch2 {
namespace Renderer {

	class Camera {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Camera instance.
		Camera( Eldritch2::RigidTransform transform, Eldritch2::RadianMeasure horizontalFovAngle, Eldritch2::float32 aspectRatio );
	//!	Constructs this @ref Camera instance.
		Camera( const Camera& ) = default;

		~Camera() = default;

	// ---------------------------------------------------

	public:
		Eldritch2::RigidTransform ETSimdCall	GetTransform() const;

		void ETSimdCall							SetTransform( Eldritch2::RigidTransform transform );

	// ---------------------------------------------------

	public:
		Eldritch2::RadianMeasure	GetHorizontalFov() const;

		void						SetHorizontalFov( Eldritch2::DegreeMeasure angle );
		void						SetHorizontalFov( Eldritch2::RadianMeasure angle );

		Eldritch2::RadianMeasure	GetVerticalFov() const;

		void						SetVerticalFov( Eldritch2::DegreeMeasure angle );
		void						SetVerticalFov( Eldritch2::RadianMeasure angle );

	// ---------------------------------------------------

	public:
		void	SetHorizontalAndVerticalFov( Eldritch2::RadianMeasure horizontalAngle, Eldritch2::float32 aspectRatio );
		void	SetHorizontalAndVerticalFov( Eldritch2::DegreeMeasure horizontalAngle, Eldritch2::float32 aspectRatio );

	// ---------------------------------------------------

	public:
		Eldritch2::Float4x4	ETSimdCall GetInverseViewProjectionMatrix( Eldritch2::float32 nearPlane, Eldritch2::float32 farPlane ) const;

		Eldritch2::Float4x4	ETSimdCall GetViewProjectionMatrix( Eldritch2::float32 nearPlane, Eldritch2::float32 farPlane ) const;

		Eldritch2::Float4x4	ETSimdCall GetInverseViewMatrix() const;

		Eldritch2::Float4x4	ETSimdCall GetViewMatrix() const;

	// - DATA MEMBERS ------------------------------------

	private:
		Eldritch2::RigidTransform	_transform;

		Eldritch2::RadianMeasure	_horizontalFov;
		Eldritch2::RadianMeasure	_verticalFov;
	};

}	// namespace Renderer
}	// namespace Eldritch2