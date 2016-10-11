/*==================================================================*\
  AngleTypes.hpp
  ------------------------------------------------------------------
  Purpose:
  Definitions for helper classes that encapsulate angles measured in
  both degrees and radians, using the built-in C++ type system to
  convert between the two automatically.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/FloatTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class RadianMeasure;
	class DegreeMeasure;
}

namespace Eldritch2 {

	class RadianMeasure {
	// - CONSTRUCTOR/DESTRUCTOR -------------------------- 

	public:
	//!	Constructs this @ref RadianMeasure instance.
		explicit RadianMeasure( Eldritch2::float32 value );
	//!	Constructs this @ref RadianMeasure instance.
		RadianMeasure( const Eldritch2::DegreeMeasure& value );
	//!	Constructs this @ref RadianMeasure instance.
		RadianMeasure( const RadianMeasure& ) = default;
	//!	Constructs this @ref RadianMeasure instance.
		RadianMeasure() = default;

		~RadianMeasure() = default;

	// ---------------------------------------------------

	public:
		explicit operator	Eldritch2::float32() const;

	// ---------------------------------------------------

	public:
		friend ETInlineHint ETPureFunctionHint RadianMeasure	operator*( RadianMeasure left, Eldritch2::float32 scalar );

		friend ETInlineHint ETPureFunctionHint RadianMeasure	operator/( RadianMeasure left, Eldritch2::float32 scalar );

		RadianMeasure&									operator*=( Eldritch2::float32 scalar );

		RadianMeasure&									operator/=( Eldritch2::float32 scalar );

	// ---------------------------------------------------

	public:
		RadianMeasure&	operator=( const Eldritch2::DegreeMeasure& value );
		RadianMeasure&	operator=( const RadianMeasure& ) = default;
		RadianMeasure&	operator=( Eldritch2::float32 value );

	// ---------------------------------------------------

	private:
		Eldritch2::float32	_measure;
	};

// ---

	class DegreeMeasure {
	// - CONSTRUCTOR/DESTRUCTOR -------------------------- 

	public:
	//!	Constructs this @ref DegreeMeasure instance.
		explicit DegreeMeasure( Eldritch2::float32 value );
	//!	Constructs this @ref DegreeMeasure instance.
		DegreeMeasure( const Eldritch2::RadianMeasure& value );
	//!	Constructs this @ref DegreeMeasure instance.
		DegreeMeasure( const DegreeMeasure& ) = default;
	//!	Constructs this @ref DegreeMeasure instance.
		DegreeMeasure() = default;

		~DegreeMeasure() = default;

	// ---------------------------------------------------

	public:
		explicit operator	Eldritch2::float32() const;

	// ---------------------------------------------------

		friend ETInlineHint ETPureFunctionHint DegreeMeasure		operator*( DegreeMeasure left, Eldritch2::float32 scalar );

		friend ETInlineHint ETPureFunctionHint DegreeMeasure		operator/( DegreeMeasure left, Eldritch2::float32 scalar );

		DegreeMeasure&										operator*=( Eldritch2::float32 scalar );

		DegreeMeasure&										operator/=( Eldritch2::float32 scalar );

	// ---------------------------------------------------

		DegreeMeasure&	operator=( const Eldritch2::RadianMeasure& value );
		DegreeMeasure&	operator=( Eldritch2::float32 value );
		DegreeMeasure&	operator=( const DegreeMeasure& ) = default;

	// ---------------------------------------------------

	private:
		Eldritch2::float32	_measure;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/MPL/AngleTypes.inl>
//------------------------------------------------------------------//
