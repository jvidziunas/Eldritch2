/*==================================================================*\
  Win32FileReadAccessStrategy.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Win32FileReadAccessStrategy.hpp>
#include <Utility/Memory/StandardLibrary.hpp>
#include <Utility/Math/StandardLibrary.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace FileSystem {

	Win32FileReadAccessStrategy::Win32FileReadAccessStrategy( ::HANDLE fileHandle, size_t sectorSizeInBytes ) : _fileHandle( fileHandle ), _sectorSizeInBytes( sectorSizeInBytes ) {
		ZeroMemory( static_cast<void*>(&_overlapped), sizeof( _overlapped ) );
		_overlapped.hEvent = ::CreateEvent( nullptr, TRUE, FALSE, nullptr );
	}

// ---------------------------------------------------

	Win32FileReadAccessStrategy::~Win32FileReadAccessStrategy() {
		::CancelIoEx( _fileHandle, &_overlapped );
		::CloseHandle( _overlapped.hEvent );
		::CloseHandle( _fileHandle );
	}

// ---------------------------------------------------

	ErrorCode Win32FileReadAccessStrategy::BeginRead( void* destinationBuffer, size_t lengthToReadInBytes ) {
		if( TRUE == ::ReadFile( _fileHandle, destinationBuffer, static_cast<::DWORD>(lengthToReadInBytes), nullptr, &_overlapped ) ) {
			return Error::NONE;
		}

		const ::DWORD	lastError( ::GetLastError() );

		return ( ( ERROR_IO_PENDING == lastError ) | ( ERROR_HANDLE_EOF == lastError ) ) ? Error::NONE : Error::UNSPECIFIED;
	}

// ---------------------------------------------------

	ErrorCode Win32FileReadAccessStrategy::BeginRead( void* destinationBuffer, size_t lengthToReadInBytes, uint64 fileOffsetInBytes ) {
		::LARGE_INTEGER&	fileOffsetHelper( reinterpret_cast<::LARGE_INTEGER&>(_overlapped.Offset) );

		fileOffsetHelper.QuadPart = fileOffsetInBytes;

		return BeginRead( destinationBuffer, lengthToReadInBytes );
	}

// ---------------------------------------------------

	bool Win32FileReadAccessStrategy::IsReadComplete() const {
		::DWORD	readBytes( 0u );

		return TRUE == ::GetOverlappedResult( _fileHandle, &_overlapped, &readBytes, FALSE );
	}

// ---------------------------------------------------

	Win32FileReadAccessStrategy::BlockingResult Win32FileReadAccessStrategy::FinishRead() {
		::ULARGE_INTEGER&	fileOffsetHelper( reinterpret_cast<::ULARGE_INTEGER&>(_overlapped.Offset) );
		BlockingResult		result;
		::DWORD				readBytes( 0u );
		::BOOL				funcResult( ::GetOverlappedResult( _fileHandle, &_overlapped, &readBytes, TRUE ) );

		fileOffsetHelper.QuadPart += readBytes;

		result.result				= funcResult ? Error::NONE : Error::UNSPECIFIED;
		result.readAmountInBytes	= readBytes;

		return result;
	}

// ---------------------------------------------------

	uint64 Win32FileReadAccessStrategy::GetSizeInBytes() const {
		::LARGE_INTEGER	result;

		return( !!::GetFileSizeEx( static_cast<::HANDLE>( _fileHandle ), &result ) ? static_cast<uint64>( result.QuadPart ) : 0u );
	}

// ---------------------------------------------------

	size_t Win32FileReadAccessStrategy::GetPhysicalMediaSectorSizeInBytes() const {
		return _sectorSizeInBytes;
	}

}	// namespace FileSystem
}	// namespace Eldritch2