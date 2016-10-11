/*==================================================================*\
  FileAppenderLog.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/StandardLibrary.hpp>
#include <Logging/FileAppenderLog.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//
#include <EASTL/utility.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Platform;

namespace Eldritch2 {
namespace Logging {

	FileAppenderLog::FileAppenderLog( SynchronousFileAppender&& appender ) : _appender( eastl::forward<SynchronousFileAppender>( appender ) ) {
		static const Utf8Char	initializationString[] = ET_UTF8_NEWLINE_LITERAL "- BEGINNING LOG OF APPLICATION RUN -" ET_UTF8_NEWLINE_LITERAL;

		Write( initializationString, StringLength( initializationString ) );
	}

// ---------------------------------------------------

	FileAppenderLog::~FileAppenderLog() {
		static const Utf8Char	terminationString[] = ET_UTF8_NEWLINE_LITERAL "- TERMINATING LOG -" ET_UTF8_NEWLINE_LITERAL;

		Write( terminationString, StringLength( terminationString ));
	}

// ---------------------------------------------------

	void FileAppenderLog::Write( const Utf8Char* const string, size_t lengthInOctets ) {
		_appender.Append( string, lengthInOctets );
	}

}	// namespace Logging
}	// namespace Eldritch2