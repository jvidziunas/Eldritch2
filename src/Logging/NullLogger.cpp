/*==================================================================*\
  NullLogger.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Logging/NullLogger.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2;

// The forced assert triggers a 'constant conditional' warning in MSVC.
// We are aware and don't particularly care, so tell it to shush for a bit.
#if( ET_COMPILER_IS_MSVC )
#	pragma warning( push )
#		pragma warning( disable : 4127 )
#endif

namespace Eldritch2 {
namespace Foundation {

	NullLogger::NullLogger() {}

// ---------------------------------------------------

	NullLogger::~NullLogger() {}

// ---------------------------------------------------

	void NullLogger::WriteString( const UTF8Char* const /*string*/, const size_t /*length*/ ) {
		ETRuntimeVerificationWithMsg( 0, UTF8L( "Attempted to use null logger object!" ) );
	}

}	// namespace Foundation
}	// namespace Eldritch2

#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif