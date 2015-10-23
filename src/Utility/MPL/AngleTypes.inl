/*==================================================================*\
  AngleTypes.inl
  ------------------------------------------------------------------
  Purpose:
  Definitions for helper classes that encapsulate angles measured in
  both degrees and radians, using the built-in C++ type system to
  convert between the two automatically.

  ------------------------------------------------------------------
  (c)2010 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

// Fake anonymous/static functions; these will be cleaned up at the
// end of this file
#if !defined( RAD2DEG )
#	define RAD2DEG(x) ((x) * 180.0f / 3.14159265358972f )
#	define RAD2DEGCLEANUP
#endif

#if !defined( DEG2RAD )
#	define DEG2RAD(x) ((x) * 3.14159265358972f / 180.0f )
#	define DEG2RADCLEANUP
#endif

namespace Eldritch2 {

	ETForceInlineHint RadianMeasure::RadianMeasure( ::Eldritch2::float32 value ) : _measure( value ) {}

// ---------------------------------------------------

	ETForceInlineHint RadianMeasure::RadianMeasure( const ::Eldritch2::DegreeMeasure& value ) : _measure( DEG2RAD( static_cast<::Eldritch2::float32>( value ) ) ) {}

// ---------------------------------------------------

	ETForceInlineHint RadianMeasure::RadianMeasure( const Eldritch2::RadianMeasure& value ) : _measure( value._measure ) {}

// ---------------------------------------------------

	ETForceInlineHint RadianMeasure::operator ::Eldritch2::float32() const {
		return _measure;
	}

// ---------------------------------------------------

	ETForceInlineHint const Eldritch2::RadianMeasure& RadianMeasure::operator=( ::Eldritch2::float32 value ) {
		_measure = value;
		return( *this );
	}
	
// ---------------------------------------------------

	ETForceInlineHint const Eldritch2::RadianMeasure& RadianMeasure::operator=( const ::Eldritch2::DegreeMeasure& value ) {
		_measure = DEG2RAD( static_cast<::Eldritch2::float32>( value ) );
		return( *this );
	}

// ---------------------------------------------------

	ETForceInlineHint const Eldritch2::RadianMeasure& RadianMeasure::operator=( const ::Eldritch2::RadianMeasure& value ) {
		_measure = value._measure;
		return( *this );
	}

// ---------------------------------------------------

	ETForceInlineHint bool RadianMeasure::operator==( const ::Eldritch2::RadianMeasure& other ) const {
		return( _measure == other._measure );
	}

// ---------------------------------------------------

	ETForceInlineHint bool RadianMeasure::operator==( const ::Eldritch2::DegreeMeasure& other ) const {
		return( *this == ::Eldritch2::RadianMeasure( other ) );
	}

// ---------------------------------------------------

	ETForceInlineHint DegreeMeasure::DegreeMeasure( ::Eldritch2::float32 value ) : _measure( value ) {}

// ---------------------------------------------------

	ETForceInlineHint DegreeMeasure::DegreeMeasure( const ::Eldritch2::DegreeMeasure& value ) : _measure( value._measure ) {}

// ---------------------------------------------------

	ETForceInlineHint DegreeMeasure::DegreeMeasure( const ::Eldritch2::RadianMeasure& value ) : _measure( RAD2DEG( static_cast<::Eldritch2::float32>( value ) ) ) {}

// ---------------------------------------------------

	ETForceInlineHint DegreeMeasure::operator ::Eldritch2::float32() const{
		return _measure;
	}

// ---------------------------------------------------

	ETForceInlineHint const Eldritch2::DegreeMeasure& DegreeMeasure::operator=( ::Eldritch2::float32 value ) {
		_measure = value;
		return( *this );
	}
	
// ---------------------------------------------------

	ETForceInlineHint const Eldritch2::DegreeMeasure& DegreeMeasure::operator=( const ::Eldritch2::RadianMeasure& value ) {
		return ( *this = ::Eldritch2::DegreeMeasure( value ) );
	}

// ---------------------------------------------------

	ETForceInlineHint const Eldritch2::DegreeMeasure& DegreeMeasure::operator=( const ::Eldritch2::DegreeMeasure& value ) {
		_measure = value._measure;
		return( *this );
	}

// ---------------------------------------------------

	ETForceInlineHint bool DegreeMeasure::operator==( const ::Eldritch2::RadianMeasure& other ) const {
		return( *this == ::Eldritch2::DegreeMeasure( other ) );
	}

// ---------------------------------------------------

	ETForceInlineHint bool DegreeMeasure::operator==( const ::Eldritch2::DegreeMeasure& other ) const {
		return( _measure == other._measure );
	}

}	// namespace Eldritch2

#if defined( RAD2DEGCLEANUP )
#	undef RAD2DEG
#	undef RAD2DEGCLEANUP
#endif

#if defined( DEG2RADCLEANUP )
#	undef DEG2RAD
#	undef DEG2RADCLEANUP
#endif