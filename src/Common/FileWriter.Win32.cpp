/*==================================================================*\
  FileWriter.Win32.cpp
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
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

namespace {

	static ETConstexpr LARGE_INTEGER ZeroOffset { 0, 0 };

	// ---------------------------------------------------

	ETForceInlineHint HANDLE MakeWriter(const wchar_t* const path, DWORD creationDisposition) ETNoexceptHint {
		return CreateFileW(path, GENERIC_WRITE, /*dwShareMode =*/0u, /*lpSecurityAttributes =*/nullptr, creationDisposition, FILE_FLAG_POSIX_SEMANTICS, /*hTemplateFile =*/nullptr);
	}

	// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint DWORD ClampDword(size_t size) ETNoexceptHint {
		return DWORD(Minimum<size_t>(size, DWORD(-1)));
	}

} // anonymous namespace

FileWriter::FileWriter(FileWriter&& file) ETNoexceptHint : _file(Exchange(file._file, INVALID_HANDLE_VALUE)) {}

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

Result FileWriter::Write(const void* source, size_t byteLength, uint64 fileByteOffset) ETNoexceptHint {
	LARGE_INTEGER newOffset;
	newOffset.QuadPart = fileByteOffset;

	if (SetFilePointerEx(_file, newOffset, /*lpNewFilePointer =*/nullptr, FILE_BEGIN) == FALSE) {
		return Result::Unspecified;
	}

	return Append(source, byteLength);
}

// ---------------------------------------------------

Result FileWriter::Append(const void* source, size_t byteLength) ETNoexceptHint {
	const char* end(static_cast<const char*>(source) + byteLength);
	while (byteLength) {
		DWORD bytesWritten;

		if (WriteFile(_file, end - byteLength, ClampDword(byteLength), ETAddressOf(bytesWritten), /*lpOverlapped =*/nullptr) == FALSE) {
			return Result::Unspecified;
		}

		byteLength -= bytesWritten;
	}

	return Result::Success;
}

// ---------------------------------------------------

uint64 FileWriter::GetFileCursorInBytes() const ETNoexceptHint {
	LARGE_INTEGER cursor(ZeroOffset);
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
	LARGE_INTEGER cursor(ZeroOffset);
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

Result FileWriter::CreateOrTruncate(const PlatformChar* path) ETNoexceptHint {
	HANDLE file(MakeWriter(path, CREATE_ALWAYS));
	ET_ABORT_UNLESS(file != INVALID_HANDLE_VALUE ? Result::Success : Result::InvalidPath);

	Swap(_file, file);
	if (file != INVALID_HANDLE_VALUE) {
		CloseHandle(file);
	}

	return Result::Success;
}

// ---------------------------------------------------

Result FileWriter::CreateOrOpen(const PlatformChar* path) ETNoexceptHint {
	HANDLE file(MakeWriter(path, OPEN_ALWAYS));
	ET_ABORT_UNLESS(file != INVALID_HANDLE_VALUE ? Result::Success : Result::InvalidPath);

	Swap(_file, file);
	if (file != INVALID_HANDLE_VALUE) {
		CloseHandle(file);
	}

	return Result::Success;
}

// ---------------------------------------------------

Result FileWriter::Open(const PlatformChar* path) ETNoexceptHint {
	HANDLE file(MakeWriter(path, OPEN_EXISTING));
	ET_ABORT_UNLESS(file != INVALID_HANDLE_VALUE ? Result::Success : Result::InvalidPath);

	Swap(_file, file);
	if (file != INVALID_HANDLE_VALUE) {
		CloseHandle(file);
	}

	return Result::Success;
}

} // namespace Eldritch2
