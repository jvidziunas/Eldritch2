/*==================================================================*\
  SynchronousFileWriter.Win32.cpp
  ------------------------------------------------------------------
  Purpose:
    
  
  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Platform/SynchronousFileWriter.hpp>
#include <Utility/Math/StandardLibrary.hpp>
#include <Utility/Result.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Platform {
namespace {

	enum : DWORD {
		FileShareNone	= 0,
		OpenSemantics	= FILE_FLAG_POSIX_SEMANTICS
	};

}	// anonymous namespace

	SynchronousFileWriter::SynchronousFileWriter( HANDLE fileHandle ) : _fileHandle( fileHandle ) {}
		
// ---------------------------------------------------
		
	SynchronousFileWriter::SynchronousFileWriter( SynchronousFileWriter&& other ) : _fileHandle( other._fileHandle ) {
		other._fileHandle = INVALID_HANDLE_VALUE;
	}
		
// ---------------------------------------------------
		
	SynchronousFileWriter::~SynchronousFileWriter() {
		if( _fileHandle == INVALID_HANDLE_VALUE ) {
			return;
		}
		
		CloseHandle( _fileHandle );
	}

// ---------------------------------------------------

	Result<SynchronousFileWriter> SynchronousFileWriter::CreateOrTruncate( const SystemChar* const path ) {
		const auto	fileHandle( CreateFileW( path, GENERIC_WRITE, FileShareNone, nullptr, CREATE_ALWAYS, OpenSemantics, nullptr ) );

		if( INVALID_HANDLE_VALUE == fileHandle ) {
			return Error::Unspecified;
		}

		return fileHandle;
	}

// ---------------------------------------------------

	Result<SynchronousFileWriter> SynchronousFileWriter::CreateOrOpen( const SystemChar* const path ) {
		const auto	fileHandle( CreateFileW( path, GENERIC_WRITE, FileShareNone, nullptr, OPEN_ALWAYS, OpenSemantics, nullptr ) );

		if( INVALID_HANDLE_VALUE == fileHandle ) {
			return Error::Unspecified;
		}

		return fileHandle;
	}

// ---------------------------------------------------

	Result<SynchronousFileWriter> SynchronousFileWriter::Open( const SystemChar* const path ) {
		const auto	fileHandle( CreateFileW( path, GENERIC_WRITE, FileShareNone, nullptr, OPEN_EXISTING, OpenSemantics, nullptr ) );

		if( INVALID_HANDLE_VALUE == fileHandle ) {
			return Error::Unspecified;
		}

		return fileHandle;
	}

// ---------------------------------------------------
		
	ErrorCode SynchronousFileWriter::Write( const void* const sourceBuffer, size_t lengthToWriteInBytes ) {
		enum : size_t {
		//	Maximum size of a system write operation, specified in bytes.
			MaximumWriteSizeInBytes = static_cast<DWORD>(-1)
		};
		
	// ---
		
		const char*	readPointer( static_cast<const char*>( sourceBuffer ) );
		
	//	Since Windows can only work with reads representable with a DWORD, we need to loop to accommodate (potential) values held in a 64-bit size_t
		while( ( static_cast<size_t>( readPointer - static_cast<const char*>( sourceBuffer ) ) < lengthToWriteInBytes ) ) {
		//	Cap the read at the maximum representable value held in a DWORD. Remember to round *after* the comparison!
			const DWORD	numberOfBytesToWrite( static_cast<DWORD>( Min<size_t>( lengthToWriteInBytes, MaximumWriteSizeInBytes ) ) );
			DWORD		numberOfBytesReadThisIteration;
		
			if( FALSE == WriteFile( _fileHandle, readPointer, numberOfBytesToWrite, &numberOfBytesReadThisIteration, nullptr ) ) {
				return Error::Unspecified;
			}
		
			readPointer += numberOfBytesReadThisIteration;
		}
		
		return Error::None;
	}
		
// ---------------------------------------------------
		
	ErrorCode SynchronousFileWriter::Write( const void* const sourceBuffer, size_t lengthToWriteInBytes, uint64 fileOffsetInBytes ) {
		LARGE_INTEGER	fileOffsetHelper;
		LARGE_INTEGER	tempOffset;
		
		fileOffsetHelper.QuadPart = fileOffsetInBytes;
		
	//	Update the file pointer to reference the desired offset...
		SetFilePointerEx( _fileHandle, fileOffsetHelper, &tempOffset, FILE_BEGIN );
		
	//	... then take the traditional write path.
		return Write( sourceBuffer, lengthToWriteInBytes );
	}

}	// namespace Platform
}	// namespace Eldritch2

