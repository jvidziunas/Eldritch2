/*==================================================================*\
  DebugController.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/DebugController.hpp>
#if( ET_COMPILER_IS_MSVC )
#	include <crtdbg.h>
#endif
#include <limits>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;
using namespace ::std;

namespace Eldritch2 {
namespace Utility {

	DebugController& DebugController::SetMemoryTrackingEnabled( bool enabled ) {
#if( ET_DEBUG_MODE_ENABLED )
#	if( ET_COMPILER_IS_MSVC )
		int	currentFlag = _crtDbgFlag;

		currentFlag |= enabled ? _CRTDBG_ALLOC_MEM_DF : 0;
		currentFlag &= enabled ? numeric_limits<int>::max() : ~_CRTDBG_ALLOC_MEM_DF;
		_CrtSetDbgFlag( currentFlag );
#	endif
#else
		ETUnreferencedParameter( enabled );
#endif
		return( *this );
	}

// ---------------------------------------------------

	DebugController& DebugController::SetLeakTrackingEnabled( bool enabled ) {
#if( ET_DEBUG_MODE_ENABLED )
#	if( ET_COMPILER_IS_MSVC )
		int	currentFlag = _crtDbgFlag;

		currentFlag |= enabled ? _CRTDBG_LEAK_CHECK_DF : 0;
		currentFlag &= enabled ? numeric_limits<int>::max() : ~_CRTDBG_LEAK_CHECK_DF;
		_CrtSetDbgFlag( currentFlag );
#	endif
#else
		ETUnreferencedParameter( enabled );
#endif
		return( *this );
	}

}	// namespace Utility
}	// namespace Eldritch2