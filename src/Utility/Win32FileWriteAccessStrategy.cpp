/*==================================================================*\
  Win32FileWriteAccessStrategy.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Win32FileWriteAccessStrategy.hpp>
#include <Utility/Memory/StandardLibrary.hpp>
#include <Utility/Math/StandardLibrary.hpp>
#include <limits>
//------------------------------------------------------------------//

using namespace ::Eldritch2;
using namespace ::Eldritch2::FileSystem;
using namespace ::std;

namespace Eldritch2
{

	namespace FileSystem
	{

		Win32FileWriteAccessStrategy::Win32FileWriteAccessStrategy( ::HANDLE	fileHandle,
																	size_t		sectorSizeInBytes ) : _fileHandle( fileHandle ),
																									  _sectorSizeInBytes( sectorSizeInBytes )
		{
		}

	// ---------------------------------------------------

		Win32FileWriteAccessStrategy::~Win32FileWriteAccessStrategy()
		{
			::CloseHandle( static_cast<::HANDLE>( _fileHandle ) );
		}

	// ---------------------------------------------------

		ErrorCode Win32FileWriteAccessStrategy::BeginWrite( const void*	sourceBuffer,
															size_t		dataLengthInBytes )
		{
			if( FALSE != ::WriteFile( _fileHandle,
									  sourceBuffer,
									  static_cast<DWORD>( dataLengthInBytes ),
									  nullptr,
									  &_overlapped ) ||
				ERROR_IO_PENDING == ::GetLastError() )
			{
				return Error::NONE;
			}

			return Error::UNSPECIFIED;
		}

	// ---------------------------------------------------

		ErrorCode Win32FileWriteAccessStrategy::BeginWrite( const void*	sourceBuffer,
															size_t		dataLengthInBytes,
															uint64		fileOffsetInBytes )
		{
			::LARGE_INTEGER&	fileOffsetHelper( reinterpret_cast<::LARGE_INTEGER&>( _overlapped.Offset ) );

			fileOffsetHelper.QuadPart = fileOffsetInBytes;

			return BeginWrite( sourceBuffer, dataLengthInBytes );
		}

	// ---------------------------------------------------

		size_t Win32FileWriteAccessStrategy::FinishWrite()
		{
			DWORD	writtenBytes( 0u );

			::GetOverlappedResult( _fileHandle,
								   &_overlapped,
								   &writtenBytes,
								   TRUE );

			return writtenBytes;
		}

	// ---------------------------------------------------

		size_t Win32FileWriteAccessStrategy::GetPhysicalMediaSectorSizeInBytes() const
		{
			return _sectorSizeInBytes;
		}

	// ---------------------------------------------------

		void Win32FileWriteAccessStrategy::AdvanceToEnd()
		{
			::LARGE_INTEGER	fileOffsetHelper;

			fileOffsetHelper.QuadPart = 0;

			::SetFilePointerEx( static_cast<::HANDLE>( _fileHandle ),
								fileOffsetHelper,
								reinterpret_cast<::LARGE_INTEGER*>( &_overlapped.Offset ),
								FILE_END );
		}

	// ---------------------------------------------------

		void Win32FileWriteAccessStrategy::SetFileSize( uint64 fileSizeInBytes )
		{
			::LARGE_INTEGER	fileOffsetHelper;

			fileOffsetHelper.QuadPart = fileSizeInBytes;

			::SetFilePointerEx( static_cast<::HANDLE>( _fileHandle ),
								fileOffsetHelper,
								reinterpret_cast<::LARGE_INTEGER*>( &_overlapped.Offset ),
								FILE_BEGIN );

			::SetEndOfFile( static_cast<::HANDLE>( _fileHandle ) );
		}

	}	// namespace FileSystem

}	// namespace Eldritch2