/*==================================================================*\
  SynchronousFileAppender.Win32.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Platform/SynchronousFileAppender.hpp>
#include <Utility/Result.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Platform {

	SynchronousFileAppender::SynchronousFileAppender( HANDLE fileHandle ) : _fileHandle( fileHandle ) {}

// ---------------------------------------------------

	SynchronousFileAppender::SynchronousFileAppender( SynchronousFileAppender&& other ) : _fileHandle( other._fileHandle ) {
		other._fileHandle = INVALID_HANDLE_VALUE;
	}

// ---------------------------------------------------

	SynchronousFileAppender::~SynchronousFileAppender() {
		if( INVALID_HANDLE_VALUE == _fileHandle ) {
			return;
		}

		CloseHandle( _fileHandle );
	}

// ---------------------------------------------------

	Result<SynchronousFileAppender> SynchronousFileAppender::OpenOrCreate( const SystemChar* const path ) {
		const HANDLE	fileHandle( CreateFileW( path, FILE_APPEND_DATA, FILE_SHARE_READ, nullptr, OPEN_ALWAYS, FILE_FLAG_POSIX_SEMANTICS, nullptr ) );

		if( INVALID_HANDLE_VALUE == fileHandle ) {
			return nullptr;
		}

		return { fileHandle };
	}

// ---------------------------------------------------

	Result<SynchronousFileAppender> SynchronousFileAppender::Open( const SystemChar* const path ) {
		const HANDLE	fileHandle( CreateFileW( path, FILE_APPEND_DATA, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_FLAG_POSIX_SEMANTICS, nullptr ) );

		if( INVALID_HANDLE_VALUE == fileHandle ) {
			return nullptr;
		}

		return { fileHandle };
	}

// ---------------------------------------------------

	ErrorCode SynchronousFileAppender::Append( const void* const sourceData, size_t lengthToWriteInBytes ) {
		DWORD	writtenLengthInBytes;

		return FALSE != WriteFile( _fileHandle, sourceData, static_cast<DWORD>(lengthToWriteInBytes), &writtenLengthInBytes, nullptr ) ? Error::None : Error::Unspecified;
	}

}	// namespace Platform
}	// namespace Eldritch2



