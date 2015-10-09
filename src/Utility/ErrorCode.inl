/*==================================================================*\
  ErrorCode.inl
  ------------------------------------------------------------------
  Purpose:
  Provides a definition/standard for an error code datatype, used
  for pinpointing failures and providing additional semantic
  information about root cause without paying the (high) cost of the
  standard C++ exception model.

  ------------------------------------------------------------------
  ©2010-2012 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

	ETForceInlineHint ErrorCode::ErrorCode( ::Eldritch2::int32 bitfield ) : _value( bitfield ) {}

// ---------------------------------------------------

	ETForceInlineHint ErrorCode::ErrorCode( const ::Eldritch2::ErrorCode& src ) : _value( src._value ) {}

// ---------------------------------------------------

	ETForceInlineHint ErrorCode::~ErrorCode() {}

// ---------------------------------------------------

	ETForceInlineHint ErrorCode::operator bool() const {
		return( Errors::NONE <= _value );
	}

// ---------------------------------------------------

	ETForceInlineHint const ::Eldritch2::ErrorCode& ErrorCode::operator=( const ::Eldritch2::ErrorCode& src ) {
		_value = src._value;
		return( *this );
	}

// ---------------------------------------------------

	ETForceInlineHint ::Eldritch2::int32 ErrorCode::ToInt() const {
		return _value;
	}

}	// namespace Eldritch2