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

using namespace ::Eldritch2;

namespace Eldritch2 {

	const UTF8Char* ErrorCode::ToUTF8String() const {
		static const UTF8Char*	errorStrings[] = {
			UTF8L( "Operation successful" )
			UTF8L( "Unspecified error" ),
			UTF8L( "Insufficient free memory" ),
			UTF8L( "Invalid parameter sent as argument to function" ),
			UTF8L( "Null pointer argument sent to function expecting a valid target" ),
			UTF8L( "Invalid or nonexistent file path" ),
			UTF8L( "Invalid object internal state" ),
			UTF8L( "Access denied" ),
			UTF8L( "Operation not supported for this object" ),
			UTF8L( "<Undefined>" )
		};

	// ---

		return errorStrings[Clamp( -_value, 0, static_cast<int32>(_countof(errorStrings)) )];
	}

}	// namespace Eldritch2