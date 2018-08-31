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

	void FileLog::Write(const Utf8Char* string, size_t lengthInOctets) ETNoexceptHint {
		_appender.Append(string, lengthInOctets);
	}

	// ---------------------------------------------------

	ErrorCode FileLog::BindResources(PlatformStringView path) {
		const Path   absolutePath(MallocAllocator("Log Path Temporary Allocator"), KnownDirectory::Logs, path);
		FileAppender appender;

		ET_ABORT_UNLESS(appender.CreateOrTruncate(absolutePath));
		Swap(_appender, appender);

		return Error::None;
	}

	// ---------------------------------------------------

	void FileLog::FreeResources() {
		FileAppender appender;

		Swap(_appender, appender);
	}

}} // namespace Eldritch2::Logging
