/*==================================================================*\
  SynchronousFileReader.Win32.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Platform/SynchronousFileReader.hpp>
#include <Utility/Math/StandardLibrary.hpp>
#include <Utility/Result.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Platform {

	SynchronousFileReader::SynchronousFileReader( HANDLE fileHandle ) : _fileHandle( fileHandle ) {}

// ---------------------------------------------------

	SynchronousFileReader::SynchronousFileReader( SynchronousFileReader&& other ) : _fileHandle( other._fileHandle ) {
		other._fileHandle = INVALID_HANDLE_VALUE;
	}

// ---------------------------------------------------

	SynchronousFileReader::~SynchronousFileReader() {
		if( _fileHandle == INVALID_HANDLE_VALUE ) {
			return;
		}

		CloseHandle( _fileHandle );
	}

// ---------------------------------------------------

	Result<SynchronousFileReader> SynchronousFileReader::Open( const SystemChar* const path ) {
		const auto	fileHandle( CreateFileW( path, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_FLAG_POSIX_SEMANTICS, nullptr ) );

		if( INVALID_HANDLE_VALUE == fileHandle ) {
			return Error::Unspecified;
		}

		return fileHandle;
	}

// ---------------------------------------------------

	ErrorCode SynchronousFileReader::Read( void* const destinationBuffer, size_t lengthToReadInBytes ) {
		enum : size_t {
		//	Maximum size of an atomic read operation, specified in bytes.
			MaximumReadSizeInBytes = static_cast<DWORD>(-1)
		};

	// ---

		char*	writePointer( static_cast<char*>(destinationBuffer) );

	//	Since Windows can only work with reads representable with a DWORD, we need to loop to accommodate (potential) values held in a 64-bit size_t
		while( static_cast<size_t>(writePointer - static_cast<char*>(destinationBuffer)) < lengthToReadInBytes ) {
		//	Cap the read at the maximum representable value held in a DWORD. Remember to round *after* the comparison!
			const DWORD	numberOfBytesToRead( static_cast<DWORD>(Min<size_t>( lengthToReadInBytes, MaximumReadSizeInBytes )) );
			DWORD		numberOfBytesReadThisIteration;

			if( FALSE == ReadFile( _fileHandle, writePointer, numberOfBytesToRead, &numberOfBytesReadThisIteration, nullptr ) ) {
				return Error::Unspecified;
			}

			writePointer += numberOfBytesReadThisIteration;
		}

		return Error::None;
	}

// ---------------------------------------------------

	ErrorCode SynchronousFileReader::Read( void* const destinationBuffer, size_t lengthToReadInBytes, uint64 fileOffsetInBytes ) {
		LARGE_INTEGER	fileOffsetHelper;
		LARGE_INTEGER	tempOffset;

		fileOffsetHelper.QuadPart = fileOffsetInBytes;

	//	Update the file pointer to reference the desired offset...
		SetFilePointerEx( _fileHandle, fileOffsetHelper, &tempOffset, FILE_BEGIN );

	//	... then take the traditional write path.
		return Read( destinationBuffer, lengthToReadInBytes );
	}

// ---------------------------------------------------

	uint64 SynchronousFileReader::GetSizeInBytes() const {
		LARGE_INTEGER	result;

		return(!!GetFileSizeEx( static_cast<HANDLE>(_fileHandle), &result ) ? static_cast<uint64>(result.QuadPart) : 0u);
	}

}	// namespace Platform
}	// namespace Eldritch2