/*==================================================================*\
  AngleTypes.hpp
  ------------------------------------------------------------------
  Purpose:
  Definitions for helper classes that encapsulate angles measured in
  both degrees and radians, using the built-in C++ type system to
  convert between the two automatically.

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
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
		// Constructs this RadianMeasure instance.
		ETForceInlineHint RadianMeasure();
		// Constructs this RadianMeasure instance.
		ETForceInlineHint explicit RadianMeasure( ::Eldritch2::float32 value );
		// Constructs this RadianMeasure instance.
		ETForceInlineHint RadianMeasure( const ::Eldritch2::DegreeMeasure& value );
		// Constructs this RadianMeasure instance.
		ETForceInlineHint RadianMeasure( const ::Eldritch2::RadianMeasure& value );

		// Destroys this RadianMeasure instance.
		ETForceInlineHint ~RadianMeasure();

	// ---------------------------------------------------

		ETForceInlineHint	operator ::Eldritch2::float32() const;

	// ---------------------------------------------------

		ETForceInlineHint const Eldritch2::RadianMeasure&	operator=( ::Eldritch2::float32 value );
		ETForceInlineHint const Eldritch2::RadianMeasure&	operator=( const ::Eldritch2::DegreeMeasure& value );
		ETForceInlineHint const Eldritch2::RadianMeasure&	operator=( const ::Eldritch2::RadianMeasure& value );

	// ---------------------------------------------------

		ETForceInlineHint bool	operator==( const ::Eldritch2::RadianMeasure& other ) const;
		ETForceInlineHint bool	operator==( const ::Eldritch2::DegreeMeasure& other ) const;

	// ---------------------------------------------------

	private:
		::Eldritch2::float32	_measure;
	};

// ---

	class DegreeMeasure {
	// - CONSTRUCTOR/DESTRUCTOR -------------------------- 

	public:
		// Constructs this DegreeMeasure instance.
		ETForceInlineHint DegreeMeasure();
		// Constructs this DegreeMeasure instance.
		ETForceInlineHint explicit DegreeMeasure( ::Eldritch2::float32 value );
		// Constructs this DegreeMeasure instance.
		ETForceInlineHint DegreeMeasure( const ::Eldritch2::DegreeMeasure& value );
		// Constructs this DegreeMeasure instance.
		ETForceInlineHint DegreeMeasure( const ::Eldritch2::RadianMeasure& value );

		// Destroys this DegreeMeasure instance.
		ETForceInlineHint ~DegreeMeasure();

	// ---------------------------------------------------

		ETForceInlineHint	operator ::Eldritch2::float32() const;

	// ---------------------------------------------------

		ETForceInlineHint const Eldritch2::DegreeMeasure&	operator=( ::Eldritch2::float32 value );
		ETForceInlineHint const Eldritch2::DegreeMeasure&	operator=( const ::Eldritch2::RadianMeasure& value );
		ETForceInlineHint const Eldritch2::DegreeMeasure&	operator=( const ::Eldritch2::DegreeMeasure& value );

	// ---------------------------------------------------

		ETForceInlineHint bool	operator==( const ::Eldritch2::RadianMeasure& other ) const;
		ETForceInlineHint bool	operator==( const ::Eldritch2::DegreeMeasure& other ) const;

	// ---------------------------------------------------

	private:
		::Eldritch2::float32	_measure;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/MPL/AngleTypes.inl>
//------------------------------------------------------------------//
