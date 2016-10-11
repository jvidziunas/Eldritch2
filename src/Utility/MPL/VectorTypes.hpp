/*==================================================================*\
  VectorTypes.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/FloatTypes.hpp>
#include <Utility/MPL/Compiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	Float4x4;
}

namespace Eldritch2 {

	class Float4 {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum VectorSwizzleComponent {
			X = 0,
			Y,
			Z,
			W
		};

	// ---

		using CoefficientSet	= Eldritch2::float32[4];

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Float4 instance.
		Float4( const Eldritch2::float32 x, const Eldritch2::float32 y, const Eldritch2::float32 z, const Eldritch2::float32 w );
	//!	Constructs this @ref Float4 instance.
		Float4( const Eldritch2::float32* const values );
	//!	Constructs this @ref Float4 instance.
		Float4( const Float4& ) = default;
	//!	Constructs this @ref Float4 instance.
		Float4() = default;

		~Float4() = default;

	// ---------------------------------------------------

	public:
		template <VectorSwizzleComponent component0, VectorSwizzleComponent component1, VectorSwizzleComponent component2, VectorSwizzleComponent component3>
		Eldritch2::Float4&	Swizzle();

		Eldritch2::float32	operator[]( size_t index ) const;

	// ---------------------------------------------------

	public:
		Eldritch2::Float4&	ETSimdCall operator+=( Eldritch2::Float4 operand );
		Eldritch2::Float4&	ETSimdCall operator-=( Eldritch2::Float4 operand );
		Eldritch2::Float4&	ETSimdCall operator*=( Eldritch2::Float4 operand );
		Eldritch2::Float4&	ETSimdCall operator*=( Eldritch2::float32 scalar );
		Eldritch2::Float4&	ETSimdCall operator/=( Eldritch2::Float4 operand );
		Eldritch2::Float4&	ETSimdCall operator/=( Eldritch2::float32 scalar );

		Eldritch2::Float4&	ETSimdCall operator=( Eldritch2::Float4 operand );

	// ---------------------------------------------------

	public:
		Eldritch2::Float4&	Normalize();

	// ---------------------------------------------------

	public:
		friend ETPureFunctionHint Eldritch2::Float4	ETSimdCall	Ceil( Eldritch2::Float4 vector );

		friend ETPureFunctionHint Eldritch2::Float4	ETSimdCall	Floor( Eldritch2::Float4 vector );

	// ---------------------------------------------------

	public:
		friend ETPureFunctionHint Eldritch2::Float4 ETSimdCall	LinearInterpolate( Eldritch2::Float4 vector0, Eldritch2::Float4 vector1, Eldritch2::float32 alpha );

		friend ETPureFunctionHint Eldritch2::Float4 ETSimdCall	CrossProduct( Eldritch2::Float4 vector0, Eldritch2::Float4 vector1 );

		friend ETPureFunctionHint Eldritch2::float32 ETSimdCall	DotProduct( Eldritch2::Float4 vector0, Eldritch2::Float4 vector1 );

	// - DATA MEMBERS ------------------------------------

	public:
		CoefficientSet	coefficients;
	};

// ---------------------------------------------------

	class Quaternion {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using CoefficientSet	= Eldritch2::float32[4];

	// - CONSTRUCTOR/DESTRUCTOR --------------------------
		
	public:
	//!	Constructs this @ref Quaternion instance.
		Quaternion( Eldritch2::float32 x, Eldritch2::float32 y, Eldritch2::float32 z, Eldritch2::float32 w );
	//!	Constructs this @ref Quaternion instance.
		Quaternion( const Eldritch2::float32* const values );
	//!	Constructs this @ref Quaternion instance.
		Quaternion( const Quaternion& ) = default;
	//!	Constructs this @ref Quaternion instance.
		Quaternion() = default;

		~Quaternion() = default;

	// ---------------------------------------------------

	public:
		Eldritch2::float32	operator[]( size_t index ) const;

	// ---------------------------------------------------

	public:
		Quaternion&	Normalize();

	// ---------------------------------------------------

	public:
		ETPureFunctionHint Quaternion ETSimdCall	GetReverse() const;

	// ---------------------------------------------------

	public:
		ETPureFunctionHint Eldritch2::Float4 ETSimdCall	RotateVector( Eldritch2::Float4 point ) const;

	// ---------------------------------------------------

	public:
		friend ETPureFunctionHint Eldritch2::Quaternion ETSimdCall		LinearInterpolateNonConstantVelocity( Eldritch2::Quaternion quaternion0, Eldritch2::Quaternion quaternion1, Eldritch2::float32 alpha );

		friend ETPureFunctionHint Eldritch2::Quaternion ETSimdCall		LinearInterpolate( Eldritch2::Quaternion quaternion0, Eldritch2::Quaternion quaternion1, Eldritch2::float32 alpha );

		friend ETPureFunctionHint Eldritch2::float32 ETSimdCall			DotProduct( Eldritch2::Quaternion quaternion0, Eldritch2::Quaternion quaternion1 );

	// - DATA MEMBERS ------------------------------------

	public:
		CoefficientSet	coefficients;
	};

// ---------------------------------------------------

	class RigidTransform {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref RigidTransform instance.
		RigidTransform( Eldritch2::Float4 translation, Eldritch2::Quaternion orientation );
	//!	Constructs this @ref RigidTransform instance.
		RigidTransform( Eldritch2::Float4 translation );
	//!	Constructs this @ref RigidTransform instance.
		RigidTransform( Eldritch2::Quaternion orientation );
	//!	Constructs this @ref RigidTransform instance.
		RigidTransform() = default;

		~RigidTransform() = default;

	// - DATA MEMBERS ------------------------------------

	public:
		Eldritch2::Float4		translation;
		Eldritch2::Quaternion	orientation;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/MPL/VectorTypes.inl>
//------------------------------------------------------------------//