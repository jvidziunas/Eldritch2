/*==================================================================*\
  AsynchronousFileAccessor.cpp
  ------------------------------------------------------------------
  Purpose:
  Windows implementation for the content provider interface. This
  object translates high-level calls to read/serialize content from
  the hard disk into low-level API commands for paging file contents
  into the running process' address space.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <FileSystem/Win32/AsynchronousFileAccessor.hpp>
#include <Utility/Math/StandardLibrary.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;
using namespace ::std;

namespace {

	enum : ::DWORD {
		// Maximum size of an atomic read operation, specified in bytes.
		MAXIMUM_READ_SIZE_IN_BYTES	= ~static_cast<::DWORD>(0u),
		// Maximum size of an atomic write operation, specified in bytes.
		MAXIMUM_WRITE_SIZE_IN_BYTES	= ~static_cast<::DWORD>(0u)
	};

// ---

	enum : size_t {
		// Maximum size of an atomic read operation, specified in bytes and expanded into a size_t.
		EXPANDED_MAXIMUM_READ_SIZE_IN_BYTES		= static_cast<size_t>(MAXIMUM_READ_SIZE_IN_BYTES),
		// Maximum size of an atomic write operation, specified in bytes and expanded into a size_t.
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

	AsynchronousFileAccessor::AsynchronousFileAccessor( const ::HANDLE fileHandle, const size_t mediaSectorSizeInBytes ) : _fileHandle( fileHandle ), _mediaSectorSizeInBytes( mediaSectorSizeInBytes ) {
		_overlapped.hEvent = ::CreateEvent( nullptr, FALSE, TRUE, nullptr );
#	if( ETIsPlatform64Bit() )
		_additionalReadBytes	= 0u;
		_cancellationFlag		= 0u;
#	endif
	}

// ---------------------------------------------------

	AsynchronousFileAccessor::~AsynchronousFileAccessor() {
		::CancelIoEx( _fileHandle, &_overlapped );
		::CloseHandle( _fileHandle );
		::CloseHandle( _overlapped.hEvent );
	}

// ---------------------------------------------------

	ErrorCode AsynchronousFileAccessor::BeginRead( void* destinationBuffer, size_t lengthToReadInBytes, uint64 fileOffsetInBytes ) {
		::LARGE_INTEGER&	fileOffsetHelper( reinterpret_cast<::LARGE_INTEGER&>( _overlapped.Offset ) );

		fileOffsetHelper.QuadPart = fileOffsetInBytes;

		// Win32 functions only accept DWORD-sized read length arguments, but we use size_t length arguments.
		// We need to manually issue read commands to ensure the entirety of the read request is satisfied, though a portion of the read
		// will not effectively be asynchronous!
#	if( ETIsPlatform64Bit() )
		// Update the additional read counter so the blocking calls report the complete, aggregate read size instead of the size of the last
		// read operation.
		_additionalReadBytes = 0u;
		// Reset the cancellation flag.
		_cancellationFlag.store( 0u, memory_order_release );

		// Loop through the large, size_t sized write operation, breaking it into chunks that can be accomplished with a single ReadFile call.
		for( ::DWORD readAmountInBytes; (0u == _cancellationFlag.load( memory_order_acquire )) & ( lengthToReadInBytes > EXPANDED_MAXIMUM_READ_SIZE_IN_BYTES ); lengthToReadInBytes -= readAmountInBytes ) {
			// Ensure the *true* async read is as big as possible, defer any of the final fixup to the async call.
			const ::DWORD amountToReadInBytes( static_cast<::DWORD>( Min<size_t>( EXPANDED_MAXIMUM_READ_SIZE_IN_BYTES,
																				  lengthToReadInBytes - EXPANDED_MAXIMUM_READ_SIZE_IN_BYTES ) ) );
			// While slightly cumbersome, we need to use asynchronous reads and then block in order for cancellation to work.
			::ReadFile( _fileHandle, destinationBuffer, amountToReadInBytes, nullptr, &_overlapped );

			::GetOverlappedResult( _fileHandle, &_overlapped, &readAmountInBytes, TRUE );

			// Update the aggregate read counter for the user finalization call...
			_additionalReadBytes += readAmountInBytes;

			// ... and advance the read pointer to pull in new data.
			destinationBuffer = static_cast<void*>( static_cast<char*>( destinationBuffer ) + readAmountInBytes );
		}
#	endif

		// Finally, finish reading any remaining bytes from the file. If the total size of the read operation was representable within the range of a DWORD, we should actually
		// end up here directly, only ever issuing one read command.
		return FALSE == ::ReadFile( _fileHandle,
									destinationBuffer,
									static_cast<::DWORD>( lengthToReadInBytes ),
									nullptr,
									&_overlapped ) && IsFatalErrorCondition( ::GetLastError() ) ? Error::UNSPECIFIED : Error::NONE;
	}

// ---------------------------------------------------

	ErrorCode AsynchronousFileAccessor::BeginWrite( const void* sourceBuffer, size_t lengthToWriteInBytes, uint64 fileOffsetInBytes ) {
		::LARGE_INTEGER&	fileOffsetHelper( reinterpret_cast<::LARGE_INTEGER&>( _overlapped.Offset ) );

		fileOffsetHelper.QuadPart = fileOffsetInBytes;

		// Win32 functions only accept DWORD-sized write length arguments, but we use size_t length arguments.
		// We need to manually issue write commands to ensure the entirety of the write request is satisfied, though a portion of the write
		// will not effectively be asynchronous!
#	if( ETIsPlatform64Bit() )
		// Update the additional write counter so the blocking calls report the complete, aggregate write size instead of the size of the last
		// write operation.
		_additionalWrittenBytes = 0u;
		// Reset the cancellation flag.
		_cancellationFlag.store( 0u, memory_order_release );

		for( ::DWORD writtenAmountInBytes; (0u == _cancellationFlag.load( memory_order_acquire )) & (lengthToWriteInBytes > EXPANDED_MAXIMUM_WRITE_SIZE_IN_BYTES); lengthToWriteInBytes -= writtenAmountInBytes ) {
			// Ensure the *true* async write is as big as possible, defer any of the final fixup to the async call.
			const ::DWORD amountToWriteInBytes( static_cast<::DWORD>( Min<size_t>( EXPANDED_MAXIMUM_WRITE_SIZE_IN_BYTES,
																				   lengthToWriteInBytes - EXPANDED_MAXIMUM_WRITE_SIZE_IN_BYTES ) ) );
			// While slightly cumbersome, we need to use asynchronous writes and then block in order for cancellation to work.
			::WriteFile( _fileHandle, sourceBuffer, amountToWriteInBytes, nullptr, &_overlapped );

			::GetOverlappedResult( _fileHandle, &_overlapped, &writtenAmountInBytes, BLOCK_UNTIL_COMPLETE );

			// Update the aggregate write counter for the user finalization call...
			_additionalWrittenBytes += writtenAmountInBytes;

			// ... and advance the read pointer to pull in new data.
			sourceBuffer = static_cast<const void*>( static_cast<const char*>( sourceBuffer ) + writtenAmountInBytes );
		}
#	endif

		// Finally, finish writing any remaining bytes to the file. If the total size of the write operation was representable within the range of a DWORD, we should actually
		// end up here directly, only ever issuing one write command.
		return FALSE == ::WriteFile( _fileHandle,
									 sourceBuffer,
									 static_cast<::DWORD>(lengthToWriteInBytes),
									 nullptr,
									 &_overlapped ) && IsFatalErrorCondition( ::GetLastError() ) ? Error::UNSPECIFIED : Error::NONE;
	}

// ---------------------------------------------------

	bool AsynchronousFileAccessor::IsReadComplete() {
		::DWORD	readBytes( 0u );

		// GetOverlappedResult specifically returns FALSE if the operation is incomplete, otherwise the result can be anything.
		return FALSE != ::GetOverlappedResult( _fileHandle, &_overlapped, &readBytes, RETURN_IF_NOT_COMPLETE );
	}

// ---------------------------------------------------

	bool AsynchronousFileAccessor::IsWriteComplete() {
		::DWORD	writtenBytes( 0u );

		// GetOverlappedResult specifically returns FALSE if the operation is incomplete, otherwise the result can be anything.
		return FALSE != ::GetOverlappedResult( _fileHandle, &_overlapped, &writtenBytes, RETURN_IF_NOT_COMPLETE );
	}

// ---------------------------------------------------

	AsynchronousFileReader::BlockingResult AsynchronousFileAccessor::BlockUntilReadComplete() {
		::DWORD			readBytes( 0u );
		const ::BOOL	overlappedResult( ::GetOverlappedResult( _fileHandle, &_overlapped, &readBytes, BLOCK_UNTIL_COMPLETE ) );

		return AsynchronousFileReader::BlockingResult( ( ( FALSE == overlappedResult ) | IsFatalErrorCondition( ::GetLastError() ) ) ? Error::UNSPECIFIED : Error::NONE,
													   _additionalReadBytes + readBytes );
	}

// ---------------------------------------------------

	AsynchronousFileWriter::BlockingResult AsynchronousFileAccessor::BlockUntilWriteComplete() {
		::DWORD			writtenBytes( 0u );
		const ::BOOL	overlappedResult( ::GetOverlappedResult( _fileHandle, &_overlapped, &writtenBytes, BLOCK_UNTIL_COMPLETE ) );

		return AsynchronousFileWriter::BlockingResult( ( ( FALSE == overlappedResult ) | IsFatalErrorCondition( ::GetLastError() ) ) ? Error::UNSPECIFIED : Error::NONE,
													   _additionalWrittenBytes + writtenBytes );
	}

// ---------------------------------------------------

	void AsynchronousFileAccessor::CancelRead() {
#	if( ETIsPlatform64Bit() )
		// Raise the internal cancellation flag. CancelIoEx will stop an actual read, but we need to
		// account for the case where a large read request must be split into several individual calls
		// to ReadFile.
		_cancellationFlag.store( 1u, memory_order_release );
#	endif
		::CancelIoEx( _fileHandle, &_overlapped );
	}

// ---------------------------------------------------

	void AsynchronousFileAccessor::CancelWrite() {
#	if( ETIsPlatform64Bit() )
		// Raise the internal cancellation flag. CancelIoEx will stop an actual write, but we need to
		// account for the case where a large write request must be split into several individual calls
		// to WriteFile.
		_cancellationFlag.store( 1u, memory_order_release );
#	endif
		::CancelIoEx( _fileHandle, &_overlapped );
	}

// ---------------------------------------------------

	uint64 AsynchronousFileAccessor::GetSizeInBytes() const {
		::LARGE_INTEGER	result;

		return( !!::GetFileSizeEx( static_cast<::HANDLE>(_fileHandle), &result ) ? static_cast<uint64>(result.QuadPart) : 0u );
	}

// ---------------------------------------------------

	size_t AsynchronousFileAccessor::GetMediaSectorSizeInBytes() const {
		return _mediaSectorSizeInBytes;
	}

// ---------------------------------------------------

	void AsynchronousFileAccessor::AdvanceToEnd() {
		::LARGE_INTEGER	fileOffsetHelper;

		fileOffsetHelper.QuadPart = 0;

		::SetFilePointerEx( _fileHandle, fileOffsetHelper, reinterpret_cast<::LARGE_INTEGER*>(&_overlapped.Offset), FILE_END );
	}

// ---------------------------------------------------

	void AsynchronousFileAccessor::SetFileSize( uint64 fileSizeInBytes ) {
		::LARGE_INTEGER	fileOffsetHelper;

		fileOffsetHelper.QuadPart = fileSizeInBytes;

		::SetFilePointerEx( _fileHandle, fileOffsetHelper, reinterpret_cast<::LARGE_INTEGER*>(&_overlapped.Offset), FILE_BEGIN );

		::SetEndOfFile( _fileHandle );
	}

}	// namespace Win32
}	// namespace FileSystem
}	// namespace Eldritch2