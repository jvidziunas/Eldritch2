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

	static ETInlineHint HANDLE MakeAppender(const wchar_t* const path, DWORD creationDisposition) {
		return CreateFileW(path, FILE_APPEND_DATA, FILE_SHARE_READ, nullptr, creationDisposition, FILE_FLAG_POSIX_SEMANTICS, nullptr);
	}

} // anonymous namespace

FileAppender::FileAppender() :
	_file(INVALID_HANDLE_VALUE) {}

// ---------------------------------------------------

FileAppender::FileAppender(FileAppender&& appender) :
	_file(eastl::exchange(appender._file, INVALID_HANDLE_VALUE)) {}

// ---------------------------------------------------

FileAppender::~FileAppender() {
	if (_file == INVALID_HANDLE_VALUE) {
		return;
	}

	CloseHandle(_file);
}

// ---------------------------------------------------

ErrorCode FileAppender::CreateOrTruncate(const PlatformChar* path) {
	using ::Eldritch2::Swap;

	HANDLE file(MakeAppender(path, CREATE_ALWAYS));
	ET_FAIL_UNLESS(file != INVALID_HANDLE_VALUE ? Error::None : Error::InvalidFileName);

	Swap(_file, file);
	if (file != INVALID_HANDLE_VALUE) {
		CloseHandle(file);
	}

	return Error::None;
}

// ---------------------------------------------------

ErrorCode FileAppender::OpenOrCreate(const PlatformChar* path) {
	using ::Eldritch2::Swap;

	HANDLE file(MakeAppender(path, OPEN_ALWAYS));
	ET_FAIL_UNLESS(file != INVALID_HANDLE_VALUE ? Error::None : Error::InvalidFileName);

	Swap(_file, file);
	if (file != INVALID_HANDLE_VALUE) {
		CloseHandle(file);
	}

	return Error::None;
}

// ---------------------------------------------------

ErrorCode FileAppender::Open(const PlatformChar* path) {
	using ::Eldritch2::Swap;

	HANDLE file(MakeAppender(path, OPEN_EXISTING));
	ET_FAIL_UNLESS(file != INVALID_HANDLE_VALUE ? Error::None : Error::InvalidFileName);

	Swap(_file, file);
	if (file != INVALID_HANDLE_VALUE) {
		CloseHandle(file);
	}

	return Error::None;
}

// ---------------------------------------------------

ErrorCode FileAppender::Append(const void* const sourceData, size_t lengthToWriteInBytes) {
	DWORD writtenLengthInBytes;

	return WriteFile(_file, sourceData, static_cast<DWORD>(lengthToWriteInBytes), &writtenLengthInBytes, nullptr) != FALSE ? Error::None : Error::Unspecified;
}

// ---------------------------------------------------

void Swap(FileAppender& file0, FileAppender& file1) {
	using ::Eldritch2::Swap;

	Swap(file0._file, file1._file);
}

} // namespace Eldritch2
