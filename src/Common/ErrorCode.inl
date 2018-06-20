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

ETInlineHint ErrorCode::ErrorCode(Error bitfield) :
	_value(static_cast<int32>(bitfield)) {}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint int AsPosixInt(ErrorCode code) {
	return code._value;
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint bool Succeeded(ErrorCode code) {
	return code._value == static_cast<int32>(Error::None);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint bool Failed(ErrorCode code) {
	return code._value != static_cast<int32>(Error::None);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint bool operator==(ErrorCode code, ErrorCode code1) {
	return code._value == code1._value;
}

} // namespace Eldritch2
