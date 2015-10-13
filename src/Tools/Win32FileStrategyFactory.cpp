/*==================================================================*\
  Win32FileStrategyFactory.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Tools/Win32FileStrategyFactory.hpp>
#include <Utility/Win32FileWriteAccessStrategy.hpp>
#include <Utility/Win32FileReadAccessStrategy.hpp>
#include <Utility/Memory/StandardLibrary.hpp>
#include <Utility/ResultPair.hpp>
#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#	define NOMINMAX
#endif
#include <Windows.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2::Tools;
using namespace ::Eldritch2;

namespace {
	
	ENUM_CLASS( ::DWORD ) {
		FILE_SHARE_NONE				= 0u,
		AGGREGATE_READ_ATTRIBUTES	= ( FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED ),
		AGGREGATE_WRITE_ATTRIBUTES	= ( FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED )
	};

// ---------------------------------------------------

	static ::DWORD Win32FileAttributesFromAccessorBehaviors( FileReadAccessorBehaviors behaviors ) {
		return AGGREGATE_READ_ATTRIBUTES | ( behaviors & FileReadAccessorBehavior::UNBUFFERED ? FILE_FLAG_NO_BUFFERING : 0u );
	}

// ---------------------------------------------------

	static ::DWORD Win32FileAttributesFromAccessorBehaviors( FileWriteAccessorBehaviors behaviors ) {
		return AGGREGATE_WRITE_ATTRIBUTES | ( behaviors & FileWriteAccessorBehavior::DELETE_FILE_ON_DESTRUCTION ? FILE_FLAG_DELETE_ON_CLOSE : 0u ) | ( behaviors & FileWriteAccessorBehavior::UNBUFFERED ? FILE_FLAG_NO_BUFFERING : 0u );
	}

}	// anonymous namespace

namespace Eldritch2 {
namespace Tools {

	Win32FileStrategyFactory::Win32FileStrategyFactory( Allocator& fileDataAllocator ) : _fileDataAllocator( fileDataAllocator, UTF8L("Win32 File Strategy Factory Content Allocator") ) {}

// ---------------------------------------------------

	Win32FileStrategyFactory::~Win32FileStrategyFactory() {}

// ---------------------------------------------------

	ResultPair<FileReadAccessStrategy> Win32FileStrategyFactory::GetAccessor( Allocator& allocator, const char* filePath, FileReadAccessorBehaviors behaviors ) {
		ResultPair<FileReadAccessStrategy>	result = { nullptr, Error::UNSPECIFIED };
		const ::HANDLE						fileHandle( ::CreateFileA( filePath, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, Win32FileAttributesFromAccessorBehaviors( behaviors ), nullptr ) );

		if( ETBranchLikelyHint( INVALID_HANDLE_VALUE != fileHandle ) ) {
			result.object = new(allocator) Win32FileReadAccessStrategy( fileHandle, GetSectorSizeFromHandle( fileHandle ) );
		}

		return result;
	}

// ---------------------------------------------------

	ResultPair<FileReadAccessStrategy> Win32FileStrategyFactory::GetAccessor( Allocator& allocator, const wchar_t* filePath, FileReadAccessorBehaviors behaviors ) {
		ResultPair<FileReadAccessStrategy>	result = { nullptr, Error::UNSPECIFIED };
		const ::HANDLE						fileHandle( ::CreateFileW( filePath, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, Win32FileAttributesFromAccessorBehaviors( behaviors ), nullptr ) );

		if( ETBranchLikelyHint( INVALID_HANDLE_VALUE != fileHandle ) ) {
			result.object = new(allocator) Win32FileReadAccessStrategy( fileHandle, GetSectorSizeFromHandle( fileHandle ) );

			if( result.object ) {
				result.resultCode = Error::NONE;
			}
		}	

		return result;
	}

// ---------------------------------------------------

	ResultPair<FileWriteAccessStrategy> Win32FileStrategyFactory::GetAccessor( Allocator& allocator, const char* filePath, FileWriteAccessorBehaviors behaviors ) {
		ResultPair<FileWriteAccessStrategy>	result = { nullptr, Error::UNSPECIFIED };
		const ::HANDLE						fileHandle( ::CreateFileA( filePath, GENERIC_WRITE, FILE_SHARE_NONE, nullptr, OPEN_ALWAYS, Win32FileAttributesFromAccessorBehaviors( behaviors ), nullptr ) );

		if( ETBranchLikelyHint( INVALID_HANDLE_VALUE != fileHandle ) ) {
			result.object = new(allocator) Win32FileWriteAccessStrategy( fileHandle, GetSectorSizeFromHandle( fileHandle ) );

			if( result.object ) {
				result.resultCode = Error::NONE;
			}
		}

		return result;
	}

// ---------------------------------------------------

	ResultPair<FileWriteAccessStrategy> Win32FileStrategyFactory::GetAccessor( Allocator& allocator, const wchar_t* filePath, FileWriteAccessorBehaviors behaviors ) {
		ResultPair<FileWriteAccessStrategy>	result = { nullptr, Error::UNSPECIFIED };
		const ::HANDLE						fileHandle( ::CreateFileW( filePath, GENERIC_WRITE, FILE_SHARE_NONE, nullptr, OPEN_ALWAYS, Win32FileAttributesFromAccessorBehaviors( behaviors ), nullptr ) );

		if( ETBranchLikelyHint( INVALID_HANDLE_VALUE != fileHandle ) ) {
			result.object = new(allocator) Win32FileWriteAccessStrategy( fileHandle, GetSectorSizeFromHandle( fileHandle ) );

			if( result.object ) {
				result.resultCode = Error::NONE;
			}
		}

		return result;
	}

// ---------------------------------------------------

	Allocator& Win32FileStrategyFactory::GetFileDataAllocator() {
		return _fileDataAllocator;
	}

}	// namespace Tools
}	// namespace Eldritch2