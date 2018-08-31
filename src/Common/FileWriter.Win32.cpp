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

	static ETConstexpr LARGE_INTEGER ZeroOffset { 0, 0 };

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint HANDLE MakeWriter(const wchar_t* const path, DWORD creationDisposition) {
		return CreateFileW(path, GENERIC_WRITE, /*dwShareMode =*/0u, /*lpSecurityAttributes =*/nullptr, creationDisposition, FILE_FLAG_POSIX_SEMANTICS, /*hTemplateFile =*/nullptr);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint ETPureFunctionHint DWORD ClampDword(size_t size) {
		return DWORD(Min<size_t>(size, DWORD(-1)));
	}

} // anonymous namespace

FileWriter::FileWriter(FileWriter&& file) ETNoexceptHint : _file(eastl::exchange(file._file, INVALID_HANDLE_VALUE)) {}

// ---------------------------------------------------

FileWriter::FileWriter() ETNoexceptHint : _file(INVALID_HANDLE_VALUE) {}

// ---------------------------------------------------

FileWriter::~FileWriter() {
	if (_file == INVALID_HANDLE_VALUE) {
		return;
	}

	CloseHandle(_file);
}

// ---------------------------------------------------

ErrorCode FileWriter::Write(const void* source, size_t byteLength, uint64 fileByteOffset) ETNoexceptHint {
	LARGE_INTEGER newOffset;
	newOffset.QuadPart = fileByteOffset;

	if (SetFilePointerEx(_file, newOffset, /*lpNewFilePointer =*/nullptr, FILE_BEGIN) == FALSE) {
		return Error::Unspecified;
	}

	return Append(source, byteLength);
}

// ---------------------------------------------------

ErrorCode FileWriter::Append(const void* source, size_t byteLength) ETNoexceptHint {
	const char* end(static_cast<const char*>(source) + byteLength);
	while (byteLength) {
		DWORD bytesWritten;

		if (WriteFile(_file, end - byteLength, ClampDword(byteLength), ETAddressOf(bytesWritten), /*lpOverlapped =*/nullptr) == FALSE) {
			return Error::Unspecified;
		}

		byteLength -= bytesWritten;
	}

	return Error::None;
}

// ---------------------------------------------------

uint64 FileWriter::GetFileCursorInBytes() const ETNoexceptHint {
	LARGE_INTEGER cursor { 0, 0 };

	/* Win32 APIs combine set/get file pointer into SetFilePointer()/SetFilePointerEx().
	 * See https://docs.microsoft.com/en-us/windows/desktop/FileIO/positioning-a-file-pointer for details. */
	SetFilePointerEx(_file, ZeroOffset, ETAddressOf(cursor), FILE_CURRENT);

	return uint64(cursor.QuadPart);
}

// ---------------------------------------------------

void FileWriter::AdvanceToEnd() ETNoexceptHint {
	SetFilePointerEx(_file, ZeroOffset, /*lpNewFilePointer =*/nullptr, FILE_END);
}

// ---------------------------------------------------

void FileWriter::SetSize(uint64 sizeInBytes) ETNoexceptHint {
	LARGE_INTEGER cursor { 0, 0 };
	SetFilePointerEx(_file, ZeroOffset, ETAddressOf(cursor), FILE_CURRENT);

	LARGE_INTEGER newCursor;
	newCursor.QuadPart = sizeInBytes;
	SetFilePointerEx(_file, newCursor, /*lpNewFilePointer =*/nullptr, FILE_BEGIN);
	SetEndOfFile(_file);

	// Restore old file pointer, if still valid.
	if (uint64(cursor.QuadPart) < sizeInBytes) {
		SetFilePointerEx(_file, cursor, /*lpNewFilePointer =*/nullptr, FILE_BEGIN);
	}
}

// ---------------------------------------------------

ErrorCode FileWriter::CreateOrTruncate(const PlatformChar* path) ETNoexceptHint {
	HANDLE file(MakeWriter(path, CREATE_ALWAYS));
	ET_ABORT_UNLESS(file != INVALID_HANDLE_VALUE ? Error::None : Error::InvalidFileName);

	Swap(_file, file);
	if (file != INVALID_HANDLE_VALUE) {
		CloseHandle(file);
	}

	return Error::None;
}

// ---------------------------------------------------

ErrorCode FileWriter::CreateOrOpen(const PlatformChar* path) ETNoexceptHint {
	HANDLE file(MakeWriter(path, OPEN_ALWAYS));
	ET_ABORT_UNLESS(file != INVALID_HANDLE_VALUE ? Error::None : Error::InvalidFileName);

	Swap(_file, file);
	if (file != INVALID_HANDLE_VALUE) {
		CloseHandle(file);
	}

	return Error::None;
}

// ---------------------------------------------------

ErrorCode FileWriter::Open(const PlatformChar* path) ETNoexceptHint {
	HANDLE file(MakeWriter(path, OPEN_EXISTING));
	ET_ABORT_UNLESS(file != INVALID_HANDLE_VALUE ? Error::None : Error::InvalidFileName);

	Swap(_file, file);
	if (file != INVALID_HANDLE_VALUE) {
		CloseHandle(file);
	}

	return Error::None;
}

} // namespace Eldritch2
