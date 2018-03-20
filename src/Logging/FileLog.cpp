/*==================================================================*\
  FileLog.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Logging/FileLog.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Logging {

	FileLog::FileLog() : _appender() {}

// ---------------------------------------------------

	void FileLog::Write( const Utf8Char* const string, size_t lengthInOctets ) {
		_appender.Append( string, lengthInOctets );
	}

// ---------------------------------------------------

	ErrorCode FileLog::BindResources( const PlatformChar* path ) {
		FileAppender appender;
		ET_FAIL_UNLESS( appender.OpenOrCreate( path ) );

		Swap( _appender, appender );

		return Error::None;
	}

// ---------------------------------------------------

	void FileLog::FreeResources() {
		FileAppender appender;

		Swap( _appender, appender );
	}

}	// namespace Logging
}	// namespace Eldritch2