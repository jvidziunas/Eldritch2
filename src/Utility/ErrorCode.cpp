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
#include <Utility/Math/StandardLibrary.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//
#include <cstdlib>
//------------------------------------------------------------------//

namespace Eldritch2 {

	ETPureFunctionHint const Utf8Char* GetErrorString( const ErrorCode& errorCode ) {
		static const Utf8Char*	errorStrings[] = {
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

		return errorStrings[Clamp( -errorCode._value, 0, static_cast<int32>(_countof(errorStrings)) )];
	}

}	// namespace Eldritch2