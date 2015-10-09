/*==================================================================*\
  VectorTypes.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2013 Eldritch Entertainment, LLC.
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
	public:
		enum VectorSwizzleComponent {
			X = 0,
			Y = 1,
			Z = 2,
			W = 3
		};

	// ---

		typedef ::Eldritch2::float32	CoefficientSet[4];

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		// Constructs this Float4 instance.
		ETForceInlineHint Float4();
		// Constructs this Float4 instance.
		ETForceInlineHint Float4( const ::Eldritch2::float32 x, const ::Eldritch2::float32 y, const ::Eldritch2::float32 z, const ::Eldritch2::float32 w );
		// Constructs this Float4 instance.
		ETForceInlineHint Float4( const ::Eldritch2::float32* const values );

	// ---------------------------------------------------

		template <VectorSwizzleComponent component0, VectorSwizzleComponent component1, VectorSwizzleComponent component2, VectorSwizzleComponent component3>
		ETForceInlineHint ETNoAliasHint ::Eldritch2::Float4&	Swizzle();

	// ---------------------------------------------------

		ETForceInlineHint ETNoAliasHint ::Eldritch2::Float4	operator +( const ::Eldritch2::Float4 operand ) const;
		ETForceInlineHint ETNoAliasHint ::Eldritch2::Float4	operator -( const ::Eldritch2::Float4 operand ) const;
		ETForceInlineHint ETNoAliasHint ::Eldritch2::Float4	operator *( const ::Eldritch2::float32 scalar ) const;
		ETForceInlineHint ETNoAliasHint ::Eldritch2::Float4	operator /( const ::Eldritch2::float32 scalar ) const;

		ETForceInlineHint ::Eldritch2::Float4&				operator +=( const ::Eldritch2::Float4 operand );
		ETForceInlineHint ::Eldritch2::Float4&				operator -=( const ::Eldritch2::Float4 operand );
		ETForceInlineHint ::Eldritch2::Float4&				operator *=( const ::Eldritch2::float32 scalar );
		ETForceInlineHint ::Eldritch2::Float4&				operator /=( const ::Eldritch2::float32 scalar );

		ETForceInlineHint ::Eldritch2::Float4&				operator =( const ::Eldritch2::Float4 operand );

	// ---------------------------------------------------

		ETForceInlineHint ::Eldritch2::Float4&	Normalize();

	// ---------------------------------------------------

		ETForceInlineHint operator	const CoefficientSet&() const;
		ETForceInlineHint operator	CoefficientSet&();

	// - DATA MEMBERS ------------------------------------

		CoefficientSet	coefficients;
	};

// ---------------------------------------------------

	class Quaternion {
	public:
		typedef ::Eldritch2::float32	CoefficientSet[4];

	// - CONSTRUCTOR/DESTRUCTOR --------------------------
		
		// Constructs this Quaternion instance.
		ETForceInlineHint Quaternion();
		// Constructs this Quaternion instance.
		ETForceInlineHint Quaternion( const ::Eldritch2::float32 x, const ::Eldritch2::float32 y, const ::Eldritch2::float32 z, const ::Eldritch2::float32 w );
		// Constructs this Quaternion instance.
		ETForceInlineHint Quaternion( const ::Eldritch2::float32* const values );

	// ---------------------------------------------------

		ETForceInlineHint ::Eldritch2::Quaternion&	Normalize();

		ETForceInlineHint ETNoAliasHint ::Eldritch2::Quaternion	Reverse() const;

	// ---------------------------------------------------

		ETForceInlineHint ETNoAliasHint ::Eldritch2::Float4 RotateVector( const ::Eldritch2::Float4 point ) const;

	// ---------------------------------------------------

		ETForceInlineHint operator	const CoefficientSet&() const;
		ETForceInlineHint operator	CoefficientSet&();

	// - DATA MEMBERS ------------------------------------

		CoefficientSet	coefficients;
	};

// ---------------------------------------------------

	ETInlineHint ETNoAliasHint ::Eldritch2::float32	DotProduct( const ::Eldritch2::Float4 vector0, const ::Eldritch2::Float4 vector1 );

	ETInlineHint ETNoAliasHint ::Eldritch2::Float4	CrossProduct( const ::Eldritch2::Float4 vector0, const ::Eldritch2::Float4 vector1 );

	ETInlineHint ETNoAliasHint ::Eldritch2::Float4	LinearInterpolate( const ::Eldritch2::Float4 vector0, const ::Eldritch2::Float4 vector1, const ::Eldritch2::float32 alpha );

// ---------------------------------------------------

	ETInlineHint ETNoAliasHint ::Eldritch2::float32		DotProduct( const ::Eldritch2::Quaternion quaternion0, const ::Eldritch2::Quaternion quaternion1 );

	ETInlineHint ETNoAliasHint ::Eldritch2::Quaternion	LinearInterpolate( const ::Eldritch2::Quaternion quaternion0, const ::Eldritch2::Quaternion quaternion1, const ::Eldritch2::float32 alpha );

	ETInlineHint ETNoAliasHint ::Eldritch2::Quaternion	LinearInterpolateNonConstantVelocity( const ::Eldritch2::Quaternion quaternion0, const ::Eldritch2::Quaternion quaternion1, const ::Eldritch2::float32 alpha );

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/MPL/VectorTypes.inl>
//------------------------------------------------------------------//