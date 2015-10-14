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
#include <Utility/Win32FileWriteAccessStrategy.hpp>
#include <Utility/Win32FileReadAccessStrategy.hpp>
#include <Tools/Win32FileStrategyFactory.hpp>
#include <Utility/Memory/StandardLibrary.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Utility/Result.hpp>
//------------------------------------------------------------------//
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
	
	enum : ::DWORD {
		FILE_SHARE_NONE				= 0u,
		AGGREGATE_READ_ATTRIBUTES	= ( FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED ),
		AGGREGATE_WRITE_ATTRIBUTES	= ( FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED )
	};

// ---------------------------------------------------

	static ::DWORD Win32FileAttributesFromAccessorBehaviors( FileReadAccessorBehavior behaviors ) {
		return AGGREGATE_READ_ATTRIBUTES | ( behaviors & FileReadAccessorBehavior::UNBUFFERED ? FILE_FLAG_NO_BUFFERING : 0u );
	}

// ---------------------------------------------------

	static ::DWORD Win32FileAttributesFromAccessorBehaviors( FileWriteAccessorBehavior behaviors ) {
		return AGGREGATE_WRITE_ATTRIBUTES | ( behaviors & FileWriteAccessorBehavior::DELETE_FILE_ON_DESTRUCTION ? FILE_FLAG_DELETE_ON_CLOSE : 0u ) | ( behaviors & FileWriteAccessorBehavior::UNBUFFERED ? FILE_FLAG_NO_BUFFERING : 0u );
	}

}	// anonymous namespace

namespace Eldritch2 {
namespace Tools {

	Win32FileStrategyFactory::Win32FileStrategyFactory( Allocator& fileDataAllocator ) : _fileDataAllocator( fileDataAllocator, UTF8L("Win32 File Strategy Factory Content Allocator") ) {}

// ---------------------------------------------------

	Result<FileReadAccessStrategy> Win32FileStrategyFactory::GetAccessor( Allocator& allocator, const char* filePath, FileReadAccessorBehavior behaviors ) {
		const ::HANDLE	fileHandle( ::CreateFileA( filePath, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, Win32FileAttributesFromAccessorBehaviors( behaviors ), nullptr ) );

		if( ETBranchLikelyHint( INVALID_HANDLE_VALUE != fileHandle ) ) {
			if( auto* const strategy = new(allocator, Allocator::AllocationOption::PERMANENT_ALLOCATION) Win32FileReadAccessStrategy( fileHandle, GetSectorSizeFromHandle( fileHandle ) ) ) {
				return { *strategy };
			}
			
			::CloseHandle( fileHandle );

			return { Error::OUT_OF_MEMORY };
		}

		return { Error::BAD_FILE_NAME };
	}

// ---------------------------------------------------

	Result<FileReadAccessStrategy> Win32FileStrategyFactory::GetAccessor( Allocator& allocator, const wchar_t* filePath, FileReadAccessorBehavior behaviors ) {
		const ::HANDLE	fileHandle( ::CreateFileW( filePath, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, Win32FileAttributesFromAccessorBehaviors( behaviors ), nullptr ) );

		if( ETBranchLikelyHint( INVALID_HANDLE_VALUE != fileHandle ) ) {
			if( auto* const strategy = new(allocator, Allocator::AllocationOption::PERMANENT_ALLOCATION) Win32FileReadAccessStrategy( fileHandle, GetSectorSizeFromHandle( fileHandle ) ) ) {
				return { *strategy };
			}

			::CloseHandle( fileHandle );

			return { Error::OUT_OF_MEMORY };
		}	

		return { Error::BAD_FILE_NAME };
	}

// ---------------------------------------------------

	Result<FileWriteAccessStrategy> Win32FileStrategyFactory::GetAccessor( Allocator& allocator, const char* filePath, FileWriteAccessorBehavior behaviors ) {
		const ::HANDLE	fileHandle( ::CreateFileA( filePath, GENERIC_WRITE, FILE_SHARE_NONE, nullptr, OPEN_ALWAYS, Win32FileAttributesFromAccessorBehaviors( behaviors ), nullptr ) );

		if( ETBranchLikelyHint( INVALID_HANDLE_VALUE != fileHandle ) ) {
			if( auto* const strategy = new(allocator, Allocator::AllocationOption::PERMANENT_ALLOCATION) Win32FileWriteAccessStrategy( fileHandle, GetSectorSizeFromHandle( fileHandle ) ) ) {
				return { *strategy };
			}

			::CloseHandle( fileHandle );

			return { Error::OUT_OF_MEMORY };
		}

		return { Error::BAD_FILE_NAME };
	}

// ---------------------------------------------------

	Result<FileWriteAccessStrategy> Win32FileStrategyFactory::GetAccessor( Allocator& allocator, const wchar_t* filePath, FileWriteAccessorBehavior behaviors ) {
		const ::HANDLE	fileHandle( ::CreateFileW( filePath, GENERIC_WRITE, FILE_SHARE_NONE, nullptr, OPEN_ALWAYS, Win32FileAttributesFromAccessorBehaviors( behaviors ), nullptr ) );

		if( ETBranchLikelyHint( INVALID_HANDLE_VALUE != fileHandle ) ) {
			if( auto* const strategy = new(allocator, Allocator::AllocationOption::PERMANENT_ALLOCATION) Win32FileWriteAccessStrategy( fileHandle, GetSectorSizeFromHandle( fileHandle ) ) ) {
				return { *strategy };
			}

			::CloseHandle( fileHandle );

			return { Error::OUT_OF_MEMORY };
		}

		return { Error::BAD_FILE_NAME };
	}

// ---------------------------------------------------

	Allocator& Win32FileStrategyFactory::GetFileDataAllocator() {
		return _fileDataAllocator;
	}

}	// namespace Tools
}	// namespace Eldritch2