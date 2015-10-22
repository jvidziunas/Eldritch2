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
#include <Utility/Memory/StandardLibrary.hpp>
#include <Utility/Assert.hpp>
#include <cstdarg>
#include <cstdio>
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

namespace {

	AssertionFailure DefaultHandler( const char* condition, const char* file, uint32 line, const char* message ) {
		char	debugOutputString[1024u];
		size_t	remainingChars( _countof(debugOutputString) );
		char*	writePtr( debugOutputString );
		int		writeAdjust( sprintf_s( debugOutputString, remainingChars, "%s(%u): Assert Failure: ", file, line ) );

		if( condition && writeAdjust ) {
			writeAdjust = ( writePtr += writeAdjust, remainingChars -= writeAdjust, sprintf_s( writePtr, remainingChars, "'%s' ", condition ) );
		}

		if( message && writeAdjust ) {
			writeAdjust = ( writePtr += writeAdjust, remainingChars -= writeAdjust, sprintf_s( writePtr, remainingChars, "%s", message ) );
		}

		if( writeAdjust ) {
			writePtr += writeAdjust, remainingChars -= writeAdjust, sprintf_s( writePtr, remainingChars, "\n" );
		}

#		if( ET_PLATFORM_WINDOWS )
		::OutputDebugStringA( debugOutputString );
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
		assertHandler = (nullptr != newHandler) ? newHandler : &DefaultHandler;
	}

// ---------------------------------------------------
	
	AssertionFailure ReportFailure( const char* condition, const char* file, uint32 line, const char* message, ... ) {
		char formattedMessage[512u];

		if( message ) {
			va_list	args;

			va_start( args, message );
				::Eldritch2::PrintFormatted( formattedMessage, message, args );
			va_end( args );
		}

		return assertHandler( condition, file, line, (message ? formattedMessage : nullptr) );
	}

}	// namespace Eldritch2