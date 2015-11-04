/*==================================================================*\
  SynchronousFileWriter.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <FileSystem/Win32/SynchronousFileWriter.hpp>
#include <Utility/Math/StandardLibrary.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2;

namespace Eldritch2 {
namespace FileSystem {
namespace Win32 {

	SynchronousFileWriter::SynchronousFileWriter( const ::HANDLE fileHandle, const size_t mediaSectorSizeInBytes ) : _fileHandle( fileHandle ), _mediaSectorSizeInBytes( mediaSectorSizeInBytes ) {}

// ---------------------------------------------------

	SynchronousFileWriter::~SynchronousFileWriter() {
		::CloseHandle( _fileHandle );
	}

// ---------------------------------------------------

	SynchronousFileWriter::BlockingResult SynchronousFileWriter::Write( const void* const sourceBuffer, const size_t lengthToWriteInBytes ) {
		enum : size_t {
			// Maximum size of an atomic write operation, specified in bytes.
			MAXIMUM_WRITE_SIZE_IN_BYTES = static_cast<size_t>(static_cast<::DWORD>(-1))
		};

	// ---

		const char*	readPointer( static_cast<const char*>(sourceBuffer) );
		::BOOL		writeResult( TRUE );

		// Since Windows can only work with writes representable with a DWORD, we need to loop to accommodate (potential) values held in a 64-bit size_t
		while( (FALSE != writeResult) & (static_cast<size_t>(readPointer - static_cast<const char*>(sourceBuffer)) < lengthToWriteInBytes) ) {
			// Cap the write at the maximum representable value held in a DWORD. Remember to round *after* the comparison!
			const ::DWORD	numberOfBytesToWrite( static_cast<::DWORD>(Min<size_t>( lengthToWriteInBytes, MAXIMUM_WRITE_SIZE_IN_BYTES )) );
			::DWORD			numberOfBytesWrittenThisIteration;

			writeResult = ::WriteFile( _fileHandle, readPointer, numberOfBytesToWrite, &numberOfBytesWrittenThisIteration, nullptr );

			readPointer += numberOfBytesWrittenThisIteration;
		}

		return { FALSE != writeResult ? Error::NONE : Error::UNSPECIFIED, static_cast<size_t>(readPointer - static_cast<const char*>(sourceBuffer)) };
	}

// ---------------------------------------------------

	SynchronousFileWriter::BlockingResult SynchronousFileWriter::Write( const void* const sourceBuffer, const size_t lengthToWriteInBytes, const uint64 fileOffsetInBytes ) {
		::LARGE_INTEGER	fileOffsetHelper;
		::LARGE_INTEGER	tempOffset;

		fileOffsetHelper.QuadPart = fileOffsetInBytes;

		// Update the file pointer to reference the desired offset...
		::SetFilePointerEx( _fileHandle, fileOffsetHelper, &tempOffset, FILE_BEGIN );

		// ... then take the traditional write path.
		return Write( sourceBuffer, lengthToWriteInBytes );
	}

// ---------------------------------------------------

	void SynchronousFileWriter::AdvanceToEnd() {
		::LARGE_INTEGER	fileOffsetHelper;
		::LARGE_INTEGER	tempOffset;

		fileOffsetHelper.QuadPart = 0u;

		::SetFilePointerEx( _fileHandle, fileOffsetHelper, &tempOffset, FILE_END );
	}

// ---------------------------------------------------

	void SynchronousFileWriter::SetFileSize( const uint64 fileSizeInBytes ) {
		::LARGE_INTEGER	fileOffsetHelper;
		::LARGE_INTEGER	tempOffset;

		fileOffsetHelper.QuadPart = fileSizeInBytes;

		::SetFilePointerEx( _fileHandle, fileOffsetHelper, &tempOffset, FILE_BEGIN );

		::SetEndOfFile( _fileHandle );
	}

// ---------------------------------------------------

	uint64 SynchronousFileWriter::GetFileCursorInBytes() const {
		::LARGE_INTEGER	offsetHelper;
		::LARGE_INTEGER	cursor;

		offsetHelper.QuadPart = 0u;

		return (FALSE != ::SetFilePointerEx( _fileHandle, offsetHelper, &cursor, FILE_CURRENT )) ? static_cast<uint64>(cursor.QuadPart) : 0u;
	}

// ---------------------------------------------------

	size_t SynchronousFileWriter::GetPhysicalMediaSectorSizeInBytes() const {
		return _mediaSectorSizeInBytes;
	}

}	// namespace Win32
}	// namespace FileSystem
}	// namespace Eldritch2