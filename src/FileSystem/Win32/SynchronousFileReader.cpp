/*==================================================================*\
  SynchronousFileReader.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <FileSystem/Win32/SynchronousFileReader.hpp>
#include <Utility/Math/StandardLibrary.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2;

namespace Eldritch2 {
namespace FileSystem {
namespace Win32 {

	SynchronousFileReader::SynchronousFileReader( const ::HANDLE fileHandle, const size_t mediaSectorSizeInBytes ) : _fileHandle( fileHandle ), _mediaSectorSizeInBytes( mediaSectorSizeInBytes ) {}

// ---------------------------------------------------

	SynchronousFileReader::~SynchronousFileReader() {
		::CloseHandle( _fileHandle );
	}

// ---------------------------------------------------

	SynchronousFileReader::BlockingResult SynchronousFileReader::Read( void* const destinationBuffer, const size_t lengthToReadInBytes )  {
		enum : size_t {
			// Maximum size of an atomic read operation, specified in bytes.
			MAXIMUM_READ_SIZE_IN_BYTES = static_cast<::DWORD>(-1)
		};

	// ---

		char*	writePointer( static_cast<char*>(destinationBuffer) );
		::BOOL	readResult( TRUE );

		// Since Windows can only work with reads representable with a DWORD, we need to loop to accommodate (potential) values held in a 64-bit size_t
		while( (FALSE != readResult) & (static_cast<size_t>(writePointer - static_cast<char*>(destinationBuffer)) < lengthToReadInBytes) ) {
			// Cap the read at the maximum representable value held in a DWORD. Remember to round *after* the comparison!
			const ::DWORD	numberOfBytesToRead( static_cast<::DWORD>(Min<size_t>( lengthToReadInBytes, MAXIMUM_READ_SIZE_IN_BYTES )) );
			::DWORD			numberOfBytesReadThisIteration;

			readResult = ::ReadFile( _fileHandle, writePointer, numberOfBytesToRead, &numberOfBytesReadThisIteration, nullptr );

			writePointer += numberOfBytesReadThisIteration;
		}

		return { FALSE != readResult ? Error::NONE : Error::UNSPECIFIED, static_cast<size_t>(writePointer - static_cast<char*>(destinationBuffer)) };
	}

// ---------------------------------------------------

	SynchronousFileReader::BlockingResult SynchronousFileReader::Read( void* const destinationBuffer, const size_t lengthToReadInBytes, const uint64 fileOffsetInBytes ) {
		::LARGE_INTEGER	fileOffsetHelper;
		::LARGE_INTEGER	tempOffset;

		fileOffsetHelper.QuadPart = fileOffsetInBytes;

		// Update the file pointer to reference the desired offset...
		::SetFilePointerEx( _fileHandle, fileOffsetHelper, &tempOffset, FILE_BEGIN );

		// ... then take the traditional write path.
		return Read( destinationBuffer, lengthToReadInBytes );
	}

// ---------------------------------------------------

	uint64 SynchronousFileReader::GetSizeInBytes() const {
		::LARGE_INTEGER	result;

		return(!!::GetFileSizeEx( static_cast<::HANDLE>(_fileHandle), &result ) ? static_cast<uint64>(result.QuadPart) : 0u);
	}

// ---------------------------------------------------

	size_t SynchronousFileReader::GetPhysicalMediaSectorSizeInBytes() const {
		return _mediaSectorSizeInBytes;
	}

}	// namespace Win32
}	// namespace FileSystem
}	// namespace Eldritch2



