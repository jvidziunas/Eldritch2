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
	ETConstexpr Vector(float32 x, float32 y, float32 z, float32 w) ETNoexceptHint;
	//!	Constructs this @ref Vector instance.
	ETConstexpr Vector(const float32 values[]) ETNoexceptHint;
	//!	Constructs this @ref Vector instance.
	ETConstexpr Vector(const Vector&) ETNoexceptHint = default;
	//!	Constructs this @ref Vector instance.
	ETConstexpr Vector(__m128) ETNoexceptHint;
	//!	Constructs this @ref Vector instance.
	Vector() ETNoexceptHint = default;

	~Vector() = default;

	// ---------------------------------------------------

public:
	Vector& ETSimdCall operator+=(Vector) ETNoexceptHint;
	Vector& ETSimdCall operator-=(Vector) ETNoexceptHint;
	Vector& ETSimdCall operator*=(Vector) ETNoexceptHint;
	Vector& ETSimdCall operator*=(float32) ETNoexceptHint;
	Vector& ETSimdCall operator/=(Vector) ETNoexceptHint;
	Vector& ETSimdCall operator/=(float32) ETNoexceptHint;
	Vector& ETSimdCall operator^=(Vector) ETNoexceptHint;

	ETConstexpr Vector& ETSimdCall operator=(const Vector&) ETNoexceptHint = default;

	// ---------------------------------------------------

public:
	template <Component x, Component y, Component z, Component w>
	ETPureFunctionHint Vector ETSimdCall Swizzle() const ETNoexceptHint;

	// ---------------------------------------------------

public:
	Vector& Normalize() ETNoexceptHint;

	// ---------------------------------------------------

public:
	ETConstexpr ETSimdCall operator const __m128&() const ETNoexceptHint;
	ETConstexpr ETSimdCall operator __m128&() ETNoexceptHint;

	// - DATA MEMBERS ------------------------------------

public:
	__m128 coefficients;

	// ---------------------------------------------------

public:
	static ETConstexpr Vector ETSimdCall MakeIdentity() ETNoexceptHint;

	static ETConstexpr Vector ETSimdCall MakeZero_HomogenousPoint() ETNoexceptHint;

	static ETConstexpr Vector ETSimdCall MakeZero() ETNoexceptHint;

	static ETConstexpr Vector ETSimdCall MakeOne_HomogenousPoint() ETNoexceptHint;

	static ETConstexpr Vector ETSimdCall MakeOne() ETNoexceptHint;

	static ETConstexpr Vector ETSimdCall MakeMinusOne_HomogenousPoint() ETNoexceptHint;

	static ETConstexpr Vector ETSimdCall MakeMinusOne() ETNoexceptHint;

	static ETConstexpr Vector ETSimdCall MakeForward() ETNoexceptHint;

	static ETConstexpr Vector ETSimdCall MakeUp() ETNoexceptHint;

	static ETConstexpr Vector ETSimdCall MakeRight() ETNoexceptHint;

	// ---------------------------------------------------

public:
	friend ETPureFunctionHint Vector ETSimdCall Minimum(Vector, Vector) ETNoexceptHint;

	friend ETPureFunctionHint Vector ETSimdCall Maximum(Vector, Vector) ETNoexceptHint;

	friend ETPureFunctionHint Vector ETSimdCall Clamp(Vector value, Vector minima, Vector maxima) ETNoexceptHint;

	friend ETPureFunctionHint Vector ETSimdCall AbsoluteValue(Vector vector) ETNoexceptHint;

	friend ETPureFunctionHint Vector ETSimdCall Floor(Vector vector) ETNoexceptHint;

	friend ETPureFunctionHint Vector ETSimdCall Ceil(Vector vector) ETNoexceptHint;

	friend ETPureFunctionHint Vector ETSimdCall Interpolate(Vector lhs, Vector rhs, float32 alpha) ETNoexceptHint;

	friend ETPureFunctionHint float32 ETSimdCall DotProduct(Vector lhs, Vector rhs) ETNoexceptHint;

	friend ETPureFunctionHint float32 ETSimdCall SquaredLength(Vector vector) ETNoexceptHint;

	friend ETPureFunctionHint float32 ETSimdCall Length(Vector vector) ETNoexceptHint;

	friend ETPureFunctionHint void ETSimdCall StreamCoefficients(float32 (&output)[4], Vector) ETNoexceptHint;

	template <typename Code>
	friend ETPureFunctionHint Code ETSimdCall GetMortonCode(Vector) ETNoexceptHint;

	friend ETPureFunctionHint int ETSimdCall PackSignBits(Vector) ETNoexceptHint;
};

// ---

class Quaternion {
	// - TYPE PUBLISHING ---------------------------------

public:
	enum Component {
		I = 3,
		J = 2,
		K = 1,
		W = 0
	};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref Quaternion instance.
	ETConstexpr Quaternion(float32 i, float32 j, float32 k, float32 w) ETNoexceptHint;
	//!	Constructs this @ref Quaternion instance.
	ETConstexpr Quaternion(const float32 values[]) ETNoexceptHint;
	//!	Constructs this @ref Quaternion instance.
	ETConstexpr Quaternion(const Quaternion&) ETNoexceptHint = default;
	//!	Constructs this @ref Quaternion instance.
	ETConstexpr Quaternion(__m128) ETNoexceptHint;
	//!	Constructs this @ref Quaternion instance.
	Quaternion(uint32 packedBits) ETNoexceptHint;
	//!	Constructs this @ref Quaternion instance.
	Quaternion() ETNoexceptHint = default;

	~Quaternion() = default;

	// ---------------------------------------------------

public:
	Quaternion& ETSimdCall operator*=(Quaternion) ETNoexceptHint;

	ETConstexpr Quaternion& ETSimdCall operator=(const Quaternion&) ETNoexceptHint = default;

	// ---------------------------------------------------

public:
	ETPureFunctionHint Vector ETSimdCall Rotate(Vector point) const ETNoexceptHint;

	Vector ETSimdCall GetForward() const ETNoexceptHint;

	Vector ETSimdCall GetRight() const ETNoexceptHint;

	Vector ETSimdCall GetUp() const ETNoexceptHint;

	Quaternion& Normalize() ETNoexceptHint;

	uint32 PackBits() const ETNoexceptHint;

	// ---------------------------------------------------

public:
	ETConstexpr ETSimdCall operator const __m128&() const ETNoexceptHint;
	ETConstexpr ETSimdCall operator __m128&() ETNoexceptHint;

	// - DATA MEMBERS ------------------------------------

public:
	__m128 coefficients;

	// ---------------------------------------------------

public:
	static ETConstexpr Quaternion ETSimdCall MakeIdentity() ETNoexceptHint;

	// ---------------------------------------------------

public:
	friend ETPureFunctionHint Quaternion ETSimdCall AsBasis(Vector forward, Vector up) ETNoexceptHint;

	friend ETPureFunctionHint Quaternion ETSimdCall InterpolateNonuniformVelocity(Quaternion lhs, Quaternion rhs, float32 alpha) ETNoexceptHint;

	friend ETPureFunctionHint Quaternion ETSimdCall Interpolate(Quaternion lhs, Quaternion rhs, float32 alpha) ETNoexceptHint;

	friend ETPureFunctionHint float32 ETSimdCall DotProduct(Quaternion lhs, Quaternion rhs) ETNoexceptHint;

	friend ETPureFunctionHint void ETSimdCall StreamCoefficients(float32 (&coefficients)[4], Quaternion) ETNoexceptHint;
};

// ---

class Transformation {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref Transformation instance.
	ETConstexpr Transformation(Vector translation, Quaternion rotation) ETNoexceptHint;
	//!	Constructs this @ref Transformation instance.
	ETConstexpr Transformation(const Transformation&) ETNoexceptHint = default;
	//!	Constructs this @ref Transformation instance.
	ETConstexpr Transformation(Quaternion rotation) ETNoexceptHint;
	//!	Constructs this @ref Transformation instance.
	ETConstexpr Transformation(Vector translation) ETNoexceptHint;
	//!	Constructs this @ref Transformation instance.
	Transformation() ETNoexceptHint = default;

	~Transformation() = default;

	// ---------------------------------------------------

public:
	Transformation& ETSimdCall operator*=(Transformation) ETNoexceptHint;

	ETConstexpr Transformation& ETSimdCall operator=(const Transformation&) ETNoexceptHint = default;

	// - DATA MEMBERS ------------------------------------

public:
	Vector     translation;
	Quaternion rotation;

	// ---------------------------------------------------

public:
	static ETConstexpr Transformation ETSimdCall MakeIdentity() ETNoexceptHint;
};

// ---

class Plane {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//! Constructs this @ref Plane instance.
	ETConstexpr Plane(float32 x, float32 y, float32 z, float32 w) ETNoexceptHint;
	//! Constructs this @ref Plane instance.
	ETConstexpr Plane(const float32 values[]) ETNoexceptHint;
	//! Constructs this @ref Plane instance.
	ETConstexpr Plane(const Plane&) ETNoexceptHint = default;
	//! Constructs this @ref Plane instance.
	ETConstexpr Plane(__m128) ETNoexceptHint;
	//! Constructs this @ref Plane instance.
	Plane() ETNoexceptHint = default;

	// ---------------------------------------------------

public:
	ETConstexpr Plane& ETSimdCall operator=(const Plane&) ETNoexceptHint = default;

	// ---------------------------------------------------

public:
	Transformation ETSimdCall Reflect(Transformation) const ETNoexceptHint;

	// - DATA MEMBERS ------------------------------------

public:
	__m128 coefficients;
};

// ---

class Box {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref Box instance.
	ETConstexpr Box(Vector minima, Vector maxima) ETNoexceptHint;
	//!	Constructs this @ref Box instance.
	ETConstexpr Box(const Box&) ETNoexceptHint = default;
	//!	Constructs this @ref Box instance.
	Box() ETNoexceptHint = default;

	~Box() = default;

	// ---------------------------------------------------

public:
	ETPureFunctionHint Box& ETSimdCall operator*=(Transformation) ETNoexceptHint;

	ETConstexpr Box& ETSimdCall operator=(const Box&) ETNoexceptHint = default;

	// ---------------------------------------------------

public:
	ETPureFunctionHint bool ETSimdCall Covers(Vector point) const ETNoexceptHint;

	// - DATA MEMBERS ------------------------------------

public:
	Vector minima;
	Vector maxima;

	// ---------------------------------------------------

public:
	static ETConstexpr Box ETSimdCall Make0_0Cube() ETNoexceptHint;

	static ETConstexpr Box ETSimdCall Make0_1Cube() ETNoexceptHint;

	static ETConstexpr Box ETSimdCall MakeMinus1_1Cube() ETNoexceptHint;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Mpl/Vector.inl>
//------------------------------------------------------------------//
