/*==================================================================*\
  FileAppender.Win32.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/FileAppender.hpp>
#include <Common/Algorithms.hpp>
#include <Common/ErrorCode.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace {

	ETInlineHint ETForceInlineHint HANDLE MakeAppender(const wchar_t* const path, DWORD creationDisposition) ETNoexceptHint {
		return CreateFileW(path, FILE_APPEND_DATA, FILE_SHARE_READ, nullptr, creationDisposition, FILE_FLAG_POSIX_SEMANTICS, nullptr);
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

ErrorCode FileAppender::Append(const void* const sourceData, size_t lengthToWriteInBytes) ETNoexceptHint {
	DWORD writtenLengthInBytes;

	return WriteFile(_file, sourceData, DWORD(lengthToWriteInBytes), ETAddressOf(writtenLengthInBytes), nullptr) != FALSE ? Error::None : Error::Unspecified;
}

// ---------------------------------------------------

ErrorCode FileAppender::CreateOrTruncate(const PlatformChar* path) ETNoexceptHint {
	using ::Eldritch2::Swap;

	HANDLE file(MakeAppender(path, CREATE_ALWAYS));
	ET_ABORT_UNLESS(file != INVALID_HANDLE_VALUE ? Error::None : Error::InvalidFileName);

	Swap(_file, file);
	if (file != INVALID_HANDLE_VALUE) {
		CloseHandle(file);
	}

	return Error::None;
}

// ---------------------------------------------------

ErrorCode FileAppender::OpenOrCreate(const PlatformChar* path) ETNoexceptHint {
	using ::Eldritch2::Swap;

	HANDLE file(MakeAppender(path, OPEN_ALWAYS));
	ET_ABORT_UNLESS(file != INVALID_HANDLE_VALUE ? Error::None : Error::InvalidFileName);

	Swap(_file, file);
	if (file != INVALID_HANDLE_VALUE) {
		CloseHandle(file);
	}

	return Error::None;
}

// ---------------------------------------------------

ErrorCode FileAppender::Open(const PlatformChar* path) ETNoexceptHint {
	using ::Eldritch2::Swap;

	HANDLE file(MakeAppender(path, OPEN_EXISTING));
	ET_ABORT_UNLESS(file != INVALID_HANDLE_VALUE ? Error::None : Error::InvalidFileName);

	Swap(_file, file);
	if (file != INVALID_HANDLE_VALUE) {
		CloseHandle(file);
	}

	return Error::None;
}

// ---------------------------------------------------

void Swap(FileAppender& lhs, FileAppender& rhs) ETNoexceptHint {
	using ::Eldritch2::Swap;

	Swap(lhs._file, rhs._file);
}

} // namespace Eldritch2
