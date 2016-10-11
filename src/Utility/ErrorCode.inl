/*==================================================================*\
  ErrorCode.inl
  ------------------------------------------------------------------
  Purpose:
  Provides a definition/standard for an error code datatype, used
  for pinpointing failures and providing additional semantic
  information about root cause without paying the (high) cost of the
  standard C++ exception model.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

	ETForceInlineHint ErrorCode::ErrorCode( Eldritch2::Error bitfield ) : _value( static_cast<Eldritch2::int32>(bitfield) ) {}

// ---------------------------------------------------

	ETForceInlineHint ErrorCode::operator bool() const {
		return( static_cast<Eldritch2::int32>(Eldritch2::Error::None) <= _value );
	}

// ---------------------------------------------------

	ETForceInlineHint int ErrorCode::AsPosixInt() const {
		return _value;
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint bool operator==( const ErrorCode& lhs, const ErrorCode& rhs ) {
		return lhs._value == rhs._value;
	}

}	// namespace Eldritch2