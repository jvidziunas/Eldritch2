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

	static ETInlineHint HANDLE MakeReader( const wchar_t* const path, DWORD creationDisposition ) {
		return CreateFileW( path, GENERIC_READ, (FILE_SHARE_READ | FILE_SHARE_WRITE), nullptr, creationDisposition, FILE_FLAG_POSIX_SEMANTICS, nullptr );
	}

}	// anonymous namespace

	FileReader::FileReader( FileReader&& reader ) : _file( eastl::exchange( reader._file, INVALID_HANDLE_VALUE ) ) {}

// ---------------------------------------------------

	FileReader::FileReader() : _file( INVALID_HANDLE_VALUE ) {}

// ---------------------------------------------------

	FileReader::~FileReader() {
		if (_file == INVALID_HANDLE_VALUE) {
			return;
		}

		CloseHandle( _file );
	}

// ---------------------------------------------------

	ErrorCode FileReader::Open( const PlatformChar* path ) {
		HANDLE file( MakeReader( path, OPEN_EXISTING ) );
		ET_FAIL_UNLESS( file != INVALID_HANDLE_VALUE ? Error::None : Error::InvalidFileName );

		Swap( _file, file );
		if (file != INVALID_HANDLE_VALUE) {
			CloseHandle( file );
		}

		return Error::None;
	}

// ---------------------------------------------------

	ErrorCode FileReader::Read( void* const destinationBuffer, size_t lengthToReadInBytes ) {
		enum : size_t {
		//	Maximum size of an atomic read operation, specified in bytes.
			MaximumReadSizeInBytes = static_cast<DWORD>(-1)
		};

	// ---

		char*	writePointer( static_cast<char*>(destinationBuffer) );

	//	Since Windows can only work with reads representable with a DWORD, we need to loop to accommodate (potential) values held in a 64-bit size_t
		while (static_cast<size_t>(writePointer - static_cast<char*>(destinationBuffer)) < lengthToReadInBytes) {
		//	Cap the read at the maximum representable value held in a DWORD. Remember to round *after* the comparison!
			const DWORD	numberOfBytesToRead( static_cast<DWORD>(Min<size_t>( lengthToReadInBytes, MaximumReadSizeInBytes )) );
			DWORD		numberOfBytesReadThisIteration;

			if (ReadFile( _file, writePointer, numberOfBytesToRead, &numberOfBytesReadThisIteration, nullptr ) == FALSE) {
				return Error::Unspecified;
			}

			writePointer += numberOfBytesReadThisIteration;
		}

		return Error::None;
	}

// ---------------------------------------------------

	ErrorCode FileReader::Read( void* const destinationBuffer, size_t lengthToReadInBytes, uint64 fileOffsetInBytes ) {
		LARGE_INTEGER	fileOffsetHelper;
		LARGE_INTEGER	tempOffset;

		fileOffsetHelper.QuadPart = fileOffsetInBytes;

	//	Update the file pointer to reference the desired offset...
		SetFilePointerEx( _file, fileOffsetHelper, &tempOffset, FILE_BEGIN );

	//	... then take the traditional write path.
		return Read( destinationBuffer, lengthToReadInBytes );
	}

// ---------------------------------------------------

	uint64 FileReader::GetSizeInBytes() const {
		LARGE_INTEGER	result;

		return GetFileSizeEx( _file, &result ) != FALSE ? static_cast<uint64>(result.QuadPart) : 0u;
	}

}	// namespace Eldritch2