/*==================================================================*\
  Vectors.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/FloatTypes.hpp>
#if ET_COMPILER_IS_MSVC
#	include <intrin.h>
#endif
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	Matrix4x4;
}

namespace Eldritch2 {

	class Vector {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum Component {
			X = 3,
			Y = 2,
			Z = 1,
			W = 0
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Vector instance.
		Vector( float32 x, float32 y, float32 z, float32 w );
	//!	Constructs this @ref Vector instance.
		Vector( const float32* values );
	//!	Constructs this @ref Vector instance.
		Vector( const Vector& ) = default;
	//!	Constructs this @ref Vector instance.
		Vector( const __m128& );
	//!	Constructs this @ref Vector instance.
		Vector() = default;

		~Vector() = default;

	// ---------------------------------------------------

	public:
		template <Component component0, Component component1, Component component2, Component component3>
		Vector&	Swizzle();

	// ---------------------------------------------------

	public:
		Vector&	ETSimdCall operator+=( Vector operand );
		Vector&	ETSimdCall operator-=( Vector operand );
		Vector&	ETSimdCall operator*=( Vector operand );
		Vector&	ETSimdCall operator*=( float32 scalar );
		Vector&	ETSimdCall operator/=( Vector operand );
		Vector&	ETSimdCall operator/=( float32 scalar );

	// ---------------------------------------------------

	public:
		friend ETPureFunctionHint Vector ETSimdCall	AbsoluteValue( Vector vector );

		friend ETPureFunctionHint Vector ETSimdCall	Floor( Vector vector );

		friend ETPureFunctionHint Vector ETSimdCall	Ceil( Vector vector );

	// ---------------------------------------------------

	public:
		friend ETPureFunctionHint Vector ETSimdCall		LinearInterpolate( Vector vector0, Vector vector1, float32 alpha );

		friend ETPureFunctionHint Vector ETSimdCall		CrossProduct( Vector vector0, Vector vector1 );

		friend ETPureFunctionHint float32 ETSimdCall	DotProduct( Vector vector0, Vector vector1 );

		friend ETPureFunctionHint float32 ETSimdCall	SquaredLength( Vector vector );

		friend ETPureFunctionHint float32 ETSimdCall	Length( Vector vector );

	// ---------------------------------------------------

	public:
		Vector&	Normalize();

	// ---------------------------------------------------

	public:
		void	ExtractCoefficients( float32 (&output)[4] ) const;

	// ---------------------------------------------------

	public:
		operator const __m128&() const;
		operator __m128&();

	// - DATA MEMBERS ------------------------------------

	public:
		__m128	coefficients;
	};

// ---------------------------------------------------

	class Quaternion {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------
		
	public:
	//!	Constructs this @ref Quaternion instance.
		Quaternion( float32 i, float32 j, float32 k, float32 w );
	//!	Constructs this @ref Quaternion instance.
		Quaternion( const float32* values );
	//!	Constructs this @ref Quaternion instance.
		Quaternion( const Quaternion& ) = default;
	//!	Constructs this @ref Quaternion instance.
		Quaternion( const __m128& );
	//!	Constructs this @ref Quaternion instance.
		Quaternion() = default;

		~Quaternion() = default;

	// ---------------------------------------------------

	public:
		Quaternion&	Normalize();

	// ---------------------------------------------------

	public:
		Quaternion&	ETSimdCall operator*=( Quaternion );

	// ---------------------------------------------------

	public:
		ETPureFunctionHint Quaternion ETSimdCall	GetReverse() const;

	// ---------------------------------------------------

	public:
		ETPureFunctionHint Vector ETSimdCall	Rotate( Vector point ) const;

		Vector ETSimdCall						GetForward() const;

		Vector ETSimdCall						GetRight() const;

		Vector ETSimdCall						GetUp() const;

	// ---------------------------------------------------

	public:
		void	ExtractCoefficients( float32 (&coefficients)[4] ) const;

	// ---------------------------------------------------

	public:
		friend ETPureFunctionHint Quaternion ETSimdCall	AsBasis( Vector forward, Vector up );

		friend ETPureFunctionHint Quaternion ETSimdCall	LinearInterpolateNonConstantVelocity( Quaternion quaternion0, Quaternion quaternion1, float32 alpha );

		friend ETPureFunctionHint Quaternion ETSimdCall	LinearInterpolate( Quaternion quaternion0, Quaternion quaternion1, float32 alpha );

		friend ETPureFunctionHint float32 ETSimdCall	DotProduct( Quaternion quaternion0, Quaternion quaternion1 );

	// ---------------------------------------------------

	public:
		operator const __m128&() const;
		operator __m128&();

	// - DATA MEMBERS ------------------------------------

	public:
		__m128	coefficients;
	};

// ---------------------------------------------------

	class Bounds {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Bounds instance.
		Bounds( Vector minima, Vector maxima );
	//!	Constructs this @ref Bounds instance.
		Bounds( const Bounds& ) = default;

		~Bounds() = default;

	// - DATA MEMBERS ------------------------------------

	public:
		Vector	minima;
		Vector	maxima;
	};

// ---------------------------------------------------

	class Transformation {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Transformation instance.
		Transformation( Vector translation, Quaternion rotation );
	//!	Constructs this @ref Transformation instance.
		Transformation( const Transformation& ) = default;
	//!	Constructs this @ref Transformation instance.
		Transformation( Quaternion rotation );
	//!	Constructs this @ref Transformation instance.
		Transformation( Vector translation );
	//!	Constructs this @ref Transformation instance.
		Transformation() = default;

		~Transformation() = default;

	// ---------------------------------------------------

	public:
		Transformation ETSimdCall	GetInverse() const;

	// ---------------------------------------------------

	public:
		Transformation&	ETSimdCall operator*=( Transformation operand );

	// - DATA MEMBERS ------------------------------------

	public:
		Vector		translation;
		Quaternion	rotation;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Mpl/Vectors.inl>
//------------------------------------------------------------------//