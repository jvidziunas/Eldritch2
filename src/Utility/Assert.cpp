/*==================================================================*\
  Assert.cpp
  ------------------------------------------------------------------
  Purpose:
  Provides faculties for testing various conditions in standard
  C/C++ code at both compile and runtime, aborting execution if the
  specified condition(s) are not met and emitting an error message.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Assert.hpp>
#include <cstdio>
#include <cstdarg>
#if( ET_PLATFORM_WINDOWS )
#	ifndef NOMINMAX
#		define NOMINMAX
#	endif
#	ifndef WIN32_LEAN_AND_MEAN
#		define WIN32_LEAN_AND_MEAN
#	endif
#	include <Windows.h>
#endif
//------------------------------------------------------------------//

using namespace ::Eldritch2;
using namespace ::std;

namespace
{
	ENUM_CLASS( size_t ) {
		ASSERT_BUFFER_SIZE	= 512u,
		HANDLER_BUFFER_SIZE	= 1024u
	};

// ---------------------------------------------------

	AssertionFailure DefaultHandler( const char* condition, const char* file, uint32 line, const char* msg ) {
		char	messageBuffer[HANDLER_BUFFER_SIZE];
		size_t	remainingChars( _countof(messageBuffer) );
		char*	writePtr( messageBuffer );
		int		writeAdjust( sprintf_s( messageBuffer, remainingChars, "%s(%d): Assert Failure: ", file, line ) );

		if( condition && writeAdjust ) {
			writeAdjust = ( writePtr += writeAdjust, remainingChars -= writeAdjust, sprintf_s( writePtr, remainingChars, "'%s' ", condition ) );
		}

		if( msg && writeAdjust ) {
			writeAdjust = ( writePtr += writeAdjust, remainingChars -= writeAdjust, sprintf_s( writePtr, remainingChars, "%s", msg ) );
		}

		if( writeAdjust ) {
			writePtr += writeAdjust, remainingChars -= writeAdjust, sprintf_s( writePtr, remainingChars, "\n" );
		}

#		if( ET_PLATFORM_WINDOWS )
		OutputDebugStringA( messageBuffer );
#		endif

		return AssertionFailures::FATAL;
	}

	static AssertionHandler	assertHandler = &DefaultHandler;

}	// anonymous namespace

namespace Eldritch2 {

	AssertionHandler GetAssertHandler() {
		return assertHandler;
	}

// ---------------------------------------------------

    void InstallHandler( AssertionHandler newHandler ) {
		if( nullptr == newHandler ) {
			newHandler = &DefaultHandler;
		}

		assertHandler = newHandler;
	}

// ---------------------------------------------------
	
	AssertionFailure ReportFailure( const char* condition, const char* file, uint32 line, const char* msg, ... ) {
		char messageBuffer[ASSERT_BUFFER_SIZE];

		if( msg ) {
			{
				va_list	args;
				va_start( args, msg );
				_vsnprintf_s( messageBuffer, ASSERT_BUFFER_SIZE, ASSERT_BUFFER_SIZE, msg, args );
				va_end( args );
			}
		}

		return assertHandler( condition, file, line, msg ? messageBuffer : NULL );
	}

}	// namespace Eldritch2