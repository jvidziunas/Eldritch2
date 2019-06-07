/*==================================================================*\
  FileLog.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Logging/FileLog.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Logging {

	FileLog::FileLog() ETNoexceptHint {}

	// ---------------------------------------------------

	void FileLog::Write(const Utf8Char* string, size_t lengthInOctets) ETNoexceptHint {
		_sink.Append(string, lengthInOctets);
	}

	// ---------------------------------------------------

	Result FileLog::BindResources(PlatformStringSpan sourcePath, StringSpan header) {
		FileAppender sink;
		const Path   path(Path::AllocatorType("Log Path Temporary Allocator"), KnownDirectory::Logs, sourcePath);

		EnsureDirectoryExists(KnownDirectory::Logs, SL(""));
		ET_ABORT_UNLESS(sink.CreateOrTruncate(path));
		Swap(_sink, sink);

		Write(header.GetData(), header.GetSize());

		return Result::Success;
	}

	// ---------------------------------------------------

	void FileLog::FreeResources(StringSpan footer) ETNoexceptHint {
		Write(footer.GetData(), footer.GetSize());

		FileAppender sink;
		Swap(_sink, sink);
	}

}} // namespace Eldritch2::Logging
