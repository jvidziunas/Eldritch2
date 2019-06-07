/*==================================================================*\
  FileAppender.Win32.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/FileAppender.hpp>
#include <Common/Algorithms.hpp>
#include <Common/Result.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

namespace {

	ETInlineHint ETForceInlineHint HANDLE MakeAppender(const wchar_t* const path, DWORD disposition) ETNoexceptHint {
		return CreateFileW(path, FILE_APPEND_DATA, FILE_SHARE_READ, nullptr, disposition, FILE_FLAG_POSIX_SEMANTICS, nullptr);
	}

} // anonymous namespace

FileAppender::FileAppender() ETNoexceptHint : _file(INVALID_HANDLE_VALUE) {}

// ---------------------------------------------------

FileAppender::FileAppender(FileAppender&& appender) ETNoexceptHint : FileAppender() {
	Swap(*this, appender);
}

// ---------------------------------------------------

FileAppender::~FileAppender() {
	if (_file == INVALID_HANDLE_VALUE) {
		return;
	}

	CloseHandle(_file);
}

// ---------------------------------------------------

Result FileAppender::Append(const void* const bytes, size_t byteSize) ETNoexceptHint {
	DWORD writtenBytes;
	return WriteFile(_file, bytes, DWORD(byteSize), ETAddressOf(writtenBytes), nullptr) != FALSE ? Result::Success : Result::Unspecified;
}

// ---------------------------------------------------

Result FileAppender::CreateOrTruncate(const PlatformChar* path) ETNoexceptHint {
	using ::Eldritch2::Swap;

	HANDLE file(MakeAppender(path, CREATE_ALWAYS));
	ET_ABORT_UNLESS(file != INVALID_HANDLE_VALUE ? Result::Success : Result::InvalidPath);
	ET_AT_SCOPE_EXIT(if (file != INVALID_HANDLE_VALUE) CloseHandle(file));

	Swap(_file, file);

	return Result::Success;
}

// ---------------------------------------------------

Result FileAppender::OpenOrCreate(const PlatformChar* path) ETNoexceptHint {
	using ::Eldritch2::Swap;

	HANDLE file(MakeAppender(path, OPEN_ALWAYS));
	ET_ABORT_UNLESS(file != INVALID_HANDLE_VALUE ? Result::Success : Result::InvalidPath);
	ET_AT_SCOPE_EXIT(if (file != INVALID_HANDLE_VALUE) CloseHandle(file));

	Swap(_file, file);

	return Result::Success;
}

// ---------------------------------------------------

Result FileAppender::Open(const PlatformChar* path) ETNoexceptHint {
	using ::Eldritch2::Swap;

	HANDLE file(MakeAppender(path, OPEN_EXISTING));
	ET_ABORT_UNLESS(file != INVALID_HANDLE_VALUE ? Result::Success : Result::InvalidPath);
	ET_AT_SCOPE_EXIT(if (file != INVALID_HANDLE_VALUE) CloseHandle(file));

	Swap(_file, file);

	return Result::Success;
}

// ---------------------------------------------------

void Swap(FileAppender& lhs, FileAppender& rhs) ETNoexceptHint {
	using ::Eldritch2::Swap;

	Swap(lhs._file, rhs._file);
}

} // namespace Eldritch2
