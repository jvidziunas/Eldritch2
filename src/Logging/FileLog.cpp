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

namespace Eldritch2 { namespace Logging {

	FileLog::FileLog() :
		_appender() {}

	// ---------------------------------------------------

	void FileLog::Write(const Utf8Char* const string, size_t lengthInOctets) {
		_appender.Append(string, lengthInOctets);
	}

	// ---------------------------------------------------

	ErrorCode FileLog::BindResources(StringView<PlatformChar> path) {
		FileAppender appender;
		Path<>       absolutePath;

		ET_FAIL_UNLESS(appender.CreateOrTruncate(absolutePath.Assign(KnownDirectory::Logs, path)));
		Swap(_appender, appender);

		return Error::None;
	}

	// ---------------------------------------------------

	void FileLog::FreeResources() {
		FileAppender appender;

		Swap(_appender, appender);
	}

}} // namespace Eldritch2::Logging
