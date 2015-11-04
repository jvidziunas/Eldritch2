/*==================================================================*\
  AsynchronousFileWriter.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <FileSystem/Win32/AsynchronousFileWriter.hpp>
#include <Utility/Math/StandardLibrary.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2;
using namespace ::std;

namespace {

	enum : ::DWORD {
		//!	Maximum size of an atomic write operation, specified in bytes.
		MAXIMUM_WRITE_SIZE_IN_BYTES	= ~static_cast<::DWORD>(0u)
	};

// ---

	enum : size_t {
		//!	Maximum size of an atomic write operation, specified in bytes and expanded into a size_t.
		EXPANDED_MAXIMUM_WRITE_SIZE_IN_BYTES	= static_cast<size_t>(MAXIMUM_WRITE_SIZE_IN_BYTES)
	};

// ---

	enum : ::BOOL {
		BLOCK_UNTIL_COMPLETE	= TRUE,
		RETURN_IF_NOT_COMPLETE	= FALSE
	};

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint static bool	IsFatalErrorCondition( const ::DWORD errorCode ) {
		return !((NO_ERROR == errorCode) | (ERROR_IO_PENDING == errorCode));
	}

}	// anonymous namespace

namespace Eldritch2 {
namespace FileSystem {
namespace Win32 {

	AsynchronousFileWriter::AsynchronousFileWriter( const ::HANDLE fileHandle, const size_t mediaSectorSizeInBytes ) : _fileHandle( fileHandle ), _mediaSectorSizeInBytes( mediaSectorSizeInBytes ) {
		_overlapped.hEvent		= ::CreateEvent( nullptr, FALSE, TRUE, nullptr );
		_additionalWrittenBytes	= 0u;
		_cancellationFlag		= 0u;
	}

// ---------------------------------------------------

	AsynchronousFileWriter::~AsynchronousFileWriter() {
		::CancelIoEx( _fileHandle, &_overlapped );
		::CloseHandle( _fileHandle );
		::CloseHandle( _overlapped.hEvent );
	}

// ---------------------------------------------------

	ErrorCode AsynchronousFileWriter::BeginWrite( const void* sourceBuffer, size_t lengthToWriteInBytes, uint64 fileOffsetInBytes ) {
		::LARGE_INTEGER&	fileOffsetHelper( reinterpret_cast<::LARGE_INTEGER&>( _overlapped.Offset ) );

		fileOffsetHelper.QuadPart = fileOffsetInBytes;

		// Win32 functions only accept DWORD-sized write length arguments, but we use size_t length arguments.
		// We need to manually issue write commands to ensure the entirety of the write request is satisfied, though a portion of the write
		// will not effectively be asynchronous!

		// Update the additional write counter so the blocking calls report the complete, aggregate write size instead of the size of the last
		// write operation.
		_additionalWrittenBytes = 0u;
		// Reset the cancellation flag.
		_cancellationFlag.store( 0u, memory_order_release );

		for( ::DWORD writtenAmountInBytes; (0u == _cancellationFlag.load( memory_order_acquire )) & (lengthToWriteInBytes > EXPANDED_MAXIMUM_WRITE_SIZE_IN_BYTES); lengthToWriteInBytes -= writtenAmountInBytes ) {
			// Ensure the *true* async write is as big as possible, defer any of the final fixup to the async call.
			const ::DWORD amountToWriteInBytes( static_cast<::DWORD>( Min<size_t>( EXPANDED_MAXIMUM_WRITE_SIZE_IN_BYTES, (lengthToWriteInBytes - EXPANDED_MAXIMUM_WRITE_SIZE_IN_BYTES) ) ) );
			// While slightly cumbersome, we need to use asynchronous writes and then block in order for cancellation to work.
			::WriteFile( _fileHandle, sourceBuffer, amountToWriteInBytes, nullptr, &_overlapped );

			::GetOverlappedResult( _fileHandle, &_overlapped, &writtenAmountInBytes, BLOCK_UNTIL_COMPLETE );

			// Update the aggregate write counter for the user finalization call...
			_additionalWrittenBytes += writtenAmountInBytes;

			// ... and advance the read pointer to pull in new data.
			sourceBuffer = static_cast<const void*>( static_cast<const char*>( sourceBuffer ) + writtenAmountInBytes );
		}

		// Finally, finish writing any remaining bytes to the file. If the total size of the write operation was representable within the range of a DWORD, we should actually
		// end up here directly, only ever issuing one write command.
		return FALSE == ::WriteFile( _fileHandle,
									 sourceBuffer,
									 static_cast<::DWORD>(lengthToWriteInBytes),
									 nullptr,
									 &_overlapped ) && IsFatalErrorCondition( ::GetLastError() ) ? Error::UNSPECIFIED : Error::NONE;
	}

// ---------------------------------------------------

	bool AsynchronousFileWriter::IsWriteComplete() {
		::DWORD	writtenBytes( 0u );

		// GetOverlappedResult specifically returns FALSE if the operation is incomplete, otherwise the result can be anything.
		return FALSE != ::GetOverlappedResult( _fileHandle, &_overlapped, &writtenBytes, RETURN_IF_NOT_COMPLETE );
	}

// ---------------------------------------------------

	AsynchronousFileWriter::BlockingResult AsynchronousFileWriter::BlockUntilWriteComplete() {
		::DWORD			writtenBytes( 0u );
		const ::BOOL	overlappedResult( ::GetOverlappedResult( _fileHandle, &_overlapped, &writtenBytes, BLOCK_UNTIL_COMPLETE ) );

		return AsynchronousFileWriter::BlockingResult( ( ( FALSE == overlappedResult ) | IsFatalErrorCondition( ::GetLastError() ) ) ? Error::UNSPECIFIED : Error::NONE, _additionalWrittenBytes + writtenBytes );
	}

// ---------------------------------------------------

	void AsynchronousFileWriter::CancelWrite() {
		// Raise the internal cancellation flag. CancelIoEx will stop an actual write, but we need to
		// account for the case where a large write request must be split into several individual calls
		// to WriteFile.
		_cancellationFlag.store( 1u, memory_order_release );
		::CancelIoEx( _fileHandle, &_overlapped );
	}

// ---------------------------------------------------

	size_t AsynchronousFileWriter::GetMediaSectorSizeInBytes() const {
		return _mediaSectorSizeInBytes;
	}

// ---------------------------------------------------

	void AsynchronousFileWriter::AdvanceToEnd() {
		::LARGE_INTEGER	fileOffsetHelper;

		fileOffsetHelper.QuadPart = 0;

		::SetFilePointerEx( _fileHandle, fileOffsetHelper, reinterpret_cast<::LARGE_INTEGER*>(&_overlapped.Offset), FILE_END );
	}

// ---------------------------------------------------

	void AsynchronousFileWriter::SetFileSize( uint64 fileSizeInBytes ) {
		::LARGE_INTEGER	fileOffsetHelper;

		fileOffsetHelper.QuadPart = fileSizeInBytes;

		::SetFilePointerEx( _fileHandle, fileOffsetHelper, reinterpret_cast<::LARGE_INTEGER*>(&_overlapped.Offset), FILE_BEGIN );

		::SetEndOfFile( _fileHandle );
	}

}	// namespace Win32
}	// namespace FileSystem
}	// namespace Eldritch2



