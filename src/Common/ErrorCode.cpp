/*==================================================================*\
  ErrorCode.cpp
  ------------------------------------------------------------------
  Purpose:
  Provides a definition/standard for an error code datatype, used
  for pinpointing failures and providing additional semantic
  information about root cause without paying the (high) cost of the
  standard C++ exception model.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/ErrorCode.hpp>
#include <Common/Math.hpp>
//------------------------------------------------------------------//
#include <cstdlib>
//------------------------------------------------------------------//

namespace Eldritch2 {

ETPureFunctionHint const Utf8Char* AsCString(ErrorCode code) {
	static const Utf8Char* strings[] = {
		"Operation successful",
		"Unspecified error",
		"Insufficient free memory",
		"Invalid parameter sent as argument to function",
		"Null pointer argument sent to function expecting a valid target",
		"Invalid or nonexistent file path",
		"Invalid object internal state",
		"Access denied",
		"Operation not supported for this object",
		"<Undefined>"
	};

	// ---

	return strings[Clamp(-code._value, 0, static_cast<int32>(_countof(strings)))];
}

} // namespace Eldritch2
