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

	ETForceInlineHint ErrorCode::ErrorCode( const ::Eldritch2::Error bitfield ) : _value( static_cast<::Eldritch2::int32>(bitfield) ) {}

// ---------------------------------------------------

	ETForceInlineHint ErrorCode::operator bool() const {
		return( static_cast<::Eldritch2::int32>(::Eldritch2::Error::NONE) <= _value );
	}

// ---------------------------------------------------

	ETForceInlineHint int ErrorCode::ToInt() const {
		return _value;
	}

}	// namespace Eldritch2