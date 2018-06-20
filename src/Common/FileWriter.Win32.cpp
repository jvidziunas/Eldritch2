/*==================================================================*\
  FileWriter.Win32.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/FileWriter.hpp>
#include <Common/Algorithms.hpp>
#include <Common/ErrorCode.hpp>
#include <Common/Math.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace {

	ETInlineHint HANDLE MakeWriter(const wchar_t* const path, DWORD creationDisposition) {
		enum : DWORD {
			FileShareNone = 0,
			OpenSemantics = FILE_FLAG_POSIX_SEMANTICS
		};

		return CreateFileW(path, GENERIC_WRITE, FileShareNone, nullptr, creationDisposition, OpenSemantics, nullptr);
	}

} // anonymous namespace

FileWriter::FileWriter(FileWriter&& writer) :
	_file(eastl::exchange(writer._file, INVALID_HANDLE_VALUE)) {}

// ---------------------------------------------------

FileWriter::FileWriter() :
	_file(INVALID_HANDLE_VALUE) {}

// ---------------------------------------------------

FileWriter::~FileWriter() {
	if (_file == INVALID_HANDLE_VALUE) {
		return;
	}

	CloseHandle(_file);
}

// ---------------------------------------------------

ErrorCode FileWriter::CreateOrTruncate(const PlatformChar* path) {
	HANDLE file(MakeWriter(path, CREATE_ALWAYS));
	ET_FAIL_UNLESS(file != INVALID_HANDLE_VALUE ? Error::None : Error::InvalidFileName);

	Swap(_file, file);
	if (file != INVALID_HANDLE_VALUE) {
		CloseHandle(file);
	}

	return Error::None;
}

// ---------------------------------------------------

ErrorCode FileWriter::CreateOrOpen(const PlatformChar* path) {
	HANDLE file(MakeWriter(path, OPEN_ALWAYS));
	ET_FAIL_UNLESS(file != INVALID_HANDLE_VALUE ? Error::None : Error::InvalidFileName);

	Swap(_file, file);
	if (file != INVALID_HANDLE_VALUE) {
		CloseHandle(file);
	}

	return Error::None;
}

// ---------------------------------------------------

ErrorCode FileWriter::Open(const PlatformChar* path) {
	HANDLE file(MakeWriter(path, OPEN_EXISTING));
	ET_FAIL_UNLESS(file != INVALID_HANDLE_VALUE ? Error::None : Error::InvalidFileName);

	Swap(_file, file);
	if (file != INVALID_HANDLE_VALUE) {
		CloseHandle(file);
	}

	return Error::None;
}

// ---------------------------------------------------

ErrorCode FileWriter::Append(const void* const sourceBuffer, size_t lengthToWriteInBytes) {
	enum : size_t {
		//	Maximum size of a system write operation, specified in bytes.
		MaximumWriteSizeInBytes = static_cast<DWORD>(-1)
	};

	// ---

	const char* readPointer(static_cast<const char*>(sourceBuffer));

	//	Since Windows can only work with reads representable with a DWORD, we need to loop to accommodate (potential) values held in a 64-bit size_t.
	while ((static_cast<size_t>(readPointer - static_cast<const char*>(sourceBuffer)) < lengthToWriteInBytes)) {
		//	Cap the read at the maximum representable value held in a DWORD. Remember to round *after* the promotion!
		const DWORD numberOfBytesToWrite(static_cast<DWORD>(Min<size_t>(lengthToWriteInBytes, MaximumWriteSizeInBytes)));
		DWORD       numberOfBytesReadThisIteration;

		if (WriteFile(_file, readPointer, numberOfBytesToWrite, &numberOfBytesReadThisIteration, nullptr) == FALSE) {
			return Error::Unspecified;
		}

		readPointer += numberOfBytesReadThisIteration;
	}

	return Error::None;
}

// ---------------------------------------------------

ErrorCode FileWriter::Write(const void* const sourceBuffer, size_t lengthToWriteInBytes, uint64 fileOffsetInBytes) {
	LARGE_INTEGER fileOffsetHelper;
	LARGE_INTEGER tempOffset;

	fileOffsetHelper.QuadPart = fileOffsetInBytes;

	//	Update the file pointer to reference the desired offset then take the traditional write path.
	SetFilePointerEx(_file, fileOffsetHelper, &tempOffset, FILE_BEGIN);
	return Append(sourceBuffer, lengthToWriteInBytes);
}

} // namespace Eldritch2
