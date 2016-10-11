/*==================================================================*\
  ChildLog.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Logging/ChildLog.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Logging {

	void ChildLog::Write( const Utf8Char* const string, size_t lengthInOctets ) {
		_parent->Write( string, lengthInOctets );
	}

}	// namespace Logging
}	// namespace Eldritch2