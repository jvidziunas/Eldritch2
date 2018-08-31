/*==================================================================*\
  FileReader.Win32.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/FileReader.hpp>
#include <Common/Algorithms.hpp>
#include <Common/ErrorCode.hpp>
#include <Common/Math.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

namespace {

	ETInlineHint ETForceInlineHint HANDLE MakeReader(const wchar_t* const path, DWORD disposition) ETNoexceptHint {
		return CreateFileW(path, GENERIC_READ, (FILE_SHARE_READ | FILE_SHARE_WRITE), /*lpSecurityAttributes =*/nullptr, disposition, FILE_FLAG_POSIX_SEMANTICS, /*hTemplateFile =*/nullptr);
	}

} // anonymous namespace

FileReader::FileReader() ETNoexceptHint : _file(INVALID_HANDLE_VALUE) {}

// ---------------------------------------------------

FileReader::FileReader(FileReader&& reader) ETNoexceptHint : FileReader() {
	Swap(*this, reader);
}

// ---------------------------------------------------

FileReader::~FileReader() {
	if (_file == INVALID_HANDLE_VALUE) {
		return;
	}

	CloseHandle(_file);
}

// ---------------------------------------------------

ErrorCode FileReader::Open(const PlatformChar* path) ETNoexceptHint {
	HANDLE file(MakeReader(path, OPEN_EXISTING));
	ET_ABORT_UNLESS(file != INVALID_HANDLE_VALUE ? Error::None : Error::InvalidFileName);
	ET_AT_SCOPE_EXIT(if (file != INVALID_HANDLE_VALUE) CloseHandle(file));

	Swap(_file, file);

	return Error::None;
}

// ---------------------------------------------------

ErrorCode FileReader::Read(void* const destination, size_t byteLength) ETNoexceptHint {
	/*	Since Windows can only work with reads representable with a DWORD, we need a loop to accommodate read requests
	 *	with sizes that do not fit within DWORD ranges. */
	const auto end(static_cast<char*>(destination) + byteLength);
	while (byteLength) {
		DWORD bytesRead;
		if (ReadFile(_file, end - byteLength, DWORD(Min<size_t>(byteLength, DWORD(-1))), ETAddressOf(bytesRead), /*lpOverlapped =*/nullptr) == FALSE) {
			return Error::Unspecified;
		}

		byteLength -= bytesRead;
	}

	return Error::None;
}

// ---------------------------------------------------

ErrorCode FileReader::Read(void* const destination, size_t byteLength, uint64 byteOffset) ETNoexceptHint {
	LARGE_INTEGER fileOffsetHelper;
	fileOffsetHelper.QuadPart = byteOffset;

	//	Update the file pointer to reference the desired offset...
	if (SetFilePointerEx(_file, fileOffsetHelper, /*lpNewFilePointer =*/nullptr, FILE_BEGIN) == 0) {
		return Error::Unspecified;
	}

	//	... then take the traditional write path.
	return Read(destination, byteLength);
}

// ---------------------------------------------------

uint64 FileReader::GetSizeInBytes() const ETNoexceptHint {
	LARGE_INTEGER byteSize;
	return GetFileSizeEx(_file, ETAddressOf(byteSize)) != FALSE ? uint64(byteSize.QuadPart) : 0u;
}

// ---------------------------------------------------

void Swap(FileReader& lhs, FileReader& rhs) ETNoexceptHint {
	Swap(lhs._file, rhs._file);
}

} // namespace Eldritch2
