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

	class Float4 {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum VectorSwizzleComponent {
			X = 0,
			Y = 1,
			Z = 2,
			W = 3
		};

	// ---

		using CoefficientSet	= ::Eldritch2::float32[4];

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref Float4 instance.
		ETForceInlineHint Float4( const ::Eldritch2::float32 x, const ::Eldritch2::float32 y, const ::Eldritch2::float32 z, const ::Eldritch2::float32 w );
		//!	Constructs this @ref Float4 instance.
		ETForceInlineHint Float4( const ::Eldritch2::float32* const values );
		//!	Constructs this @ref Float4 instance.
		ETForceInlineHint Float4( const ::Eldritch2::Float4& ) = default;
		//!	Constructs this @ref Float4 instance.
		ETForceInlineHint Float4() = default;

		ETForceInlineHint ~Float4() = default;

	// ---------------------------------------------------

		template <VectorSwizzleComponent component0, VectorSwizzleComponent component1, VectorSwizzleComponent component2, VectorSwizzleComponent component3>
		ETForceInlineHint ETNoAliasHint ::Eldritch2::Float4&	Swizzle();

	// ---------------------------------------------------

		friend ETForceInlineHint ETNoAliasHint ::Eldritch2::Float4	ETSIMDCall operator+( ::Eldritch2::Float4 operand0, ::Eldritch2::Float4 operand1 );
		friend ETForceInlineHint ETNoAliasHint ::Eldritch2::Float4	ETSIMDCall operator-( ::Eldritch2::Float4 operand0, ::Eldritch2::Float4 operand1 );
		friend ETForceInlineHint ETNoAliasHint ::Eldritch2::Float4	ETSIMDCall operator*( ::Eldritch2::Float4 operand0, ::Eldritch2::Float4 operand1 );
		friend ETForceInlineHint ETNoAliasHint ::Eldritch2::Float4	ETSIMDCall operator*( ::Eldritch2::Float4 operand0, ::Eldritch2::float32 scalar );
		friend ETForceInlineHint ETNoAliasHint ::Eldritch2::Float4	ETSIMDCall operator/( ::Eldritch2::Float4 operand0, ::Eldritch2::Float4 operand1 );
		friend ETForceInlineHint ETNoAliasHint ::Eldritch2::Float4	ETSIMDCall operator/( ::Eldritch2::Float4 operand0, ::Eldritch2::float32 scalar );

		ETForceInlineHint ::Eldritch2::Float4&						ETSIMDCall operator+=( ::Eldritch2::Float4 operand );
		ETForceInlineHint ::Eldritch2::Float4&						ETSIMDCall operator-=( ::Eldritch2::Float4 operand );
		ETForceInlineHint ::Eldritch2::Float4&						ETSIMDCall operator*=( ::Eldritch2::Float4 operand );
		ETForceInlineHint ::Eldritch2::Float4&						ETSIMDCall operator*=( ::Eldritch2::float32 scalar );
		ETForceInlineHint ::Eldritch2::Float4&						ETSIMDCall operator/=( ::Eldritch2::Float4 operand );
		ETForceInlineHint ::Eldritch2::Float4&						ETSIMDCall operator/=( ::Eldritch2::float32 scalar );

		ETForceInlineHint ::Eldritch2::Float4&						ETSIMDCall operator=( ::Eldritch2::Float4 operand );

	// ---------------------------------------------------

		ETForceInlineHint ::Eldritch2::Float4&	Normalize();

	// ---------------------------------------------------

		friend ETInlineHint ETNoAliasHint ::Eldritch2::Float4	ETSIMDCall Ceil( ::Eldritch2::Float4 vector );

		friend ETInlineHint ETNoAliasHint ::Eldritch2::Float4	ETSIMDCall Floor( ::Eldritch2::Float4 vector );

	// ---------------------------------------------------

		friend ETInlineHint ETNoAliasHint ::Eldritch2::float32	ETSIMDCall DotProduct( ::Eldritch2::Float4 vector0, ::Eldritch2::Float4 vector1 );

		friend ETInlineHint ETNoAliasHint ::Eldritch2::Float4	ETSIMDCall CrossProduct( ::Eldritch2::Float4 vector0, ::Eldritch2::Float4 vector1 );

		friend ETInlineHint ETNoAliasHint ::Eldritch2::Float4	ETSIMDCall LinearInterpolate( ::Eldritch2::Float4 vector0, ::Eldritch2::Float4 vector1, ::Eldritch2::float32 alpha );

	// ---------------------------------------------------

		ETForceInlineHint operator	const CoefficientSet&() const;
		ETForceInlineHint operator	CoefficientSet&();

	// - DATA MEMBERS ------------------------------------

		CoefficientSet	coefficients;
	};

// ---------------------------------------------------

	class Quaternion {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using CoefficientSet	= ::Eldritch2::float32[4];

	// - CONSTRUCTOR/DESTRUCTOR --------------------------
		
		//!	Constructs this @ref Quaternion instance.
		ETForceInlineHint Quaternion( const ::Eldritch2::float32 x, const ::Eldritch2::float32 y, const ::Eldritch2::float32 z, const ::Eldritch2::float32 w );
		//!	Constructs this @ref Quaternion instance.
		ETForceInlineHint Quaternion( const ::Eldritch2::float32* const values );
		//!	Constructs this @ref Quaternion instance.
		ETForceInlineHint Quaternion( const ::Eldritch2::Quaternion& ) = default;
		//!	Constructs this @ref Quaternion instance.
		ETForceInlineHint Quaternion() = default;

		ETForceInlineHint ~Quaternion() = default;

	// ---------------------------------------------------

		ETForceInlineHint ::Eldritch2::Quaternion&	Normalize();

	// ---------------------------------------------------

		ETForceInlineHint ETNoAliasHint ::Eldritch2::Quaternion	ETSIMDCall GetReverse() const;

	// ---------------------------------------------------

		ETForceInlineHint ETNoAliasHint ::Eldritch2::Float4 ETSIMDCall RotateVector( ::Eldritch2::Float4 point ) const;

	// ---------------------------------------------------

		friend ETInlineHint ETNoAliasHint ::Eldritch2::float32		ETSIMDCall DotProduct( ::Eldritch2::Quaternion quaternion0, ::Eldritch2::Quaternion quaternion1 );

		friend ETInlineHint ETNoAliasHint ::Eldritch2::Quaternion	ETSIMDCall LinearInterpolate( ::Eldritch2::Quaternion quaternion0, ::Eldritch2::Quaternion quaternion1, ::Eldritch2::float32 alpha );

		friend ETInlineHint ETNoAliasHint ::Eldritch2::Quaternion	ETSIMDCall LinearInterpolateNonConstantVelocity( ::Eldritch2::Quaternion quaternion0, ::Eldritch2::Quaternion quaternion1, ::Eldritch2::float32 alpha );

	// ---------------------------------------------------

		ETForceInlineHint operator	const CoefficientSet&() const;
		ETForceInlineHint operator	CoefficientSet&();

	// - DATA MEMBERS ------------------------------------

		CoefficientSet	coefficients;
	};

// ---------------------------------------------------

	class RigidTransform {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref RigidTransform instance.
		ETForceInlineHint RigidTransform( Float4 translation, Quaternion orientation );
		//!	Constructs this @ref RigidTransform instance.
		ETForceInlineHint RigidTransform( Float4 translation );
		//!	Constructs this @ref RigidTransform instance.
		ETForceInlineHint RigidTransform( Quaternion orientation );
		//!	Constructs this @ref RigidTransform instance.
		ETForceInlineHint RigidTransform() = default;

		ETForceInlineHint ~RigidTransform() = default;

	// - DATA MEMBERS ------------------------------------

		::Eldritch2::Float4		translation;
		::Eldritch2::Quaternion	orientation;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/MPL/VectorTypes.inl>
//------------------------------------------------------------------//