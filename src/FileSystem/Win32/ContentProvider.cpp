/*==================================================================*\
  ContentProvider.cpp
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
#include <FileSystem/Win32/ReadableMemoryMappedFile.hpp>
#include <FileSystem/SynchronousFileAppender.hpp>
#include <FileSystem/Win32/ContentProvider.hpp>
#include <Utility/Win32ApplicationHelpers.hpp>
#include <Utility/Memory/StandardLibrary.hpp>
#include <Utility/Math/StandardLibrary.hpp>
#include <System/Win32/SystemInterface.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Utility/Concurrency/Lock.hpp>
#include <Scheduler/TaskScheduler.hpp>
#include <Utility/Result.hpp>
#include <Build.hpp>
//------------------------------------------------------------------//
#include <utfcpp/v2_0/source/utf8/unchecked.h>
#include <Windows.h>
#include <Shlwapi.h>
#include <shlobj.h>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY( "Shlwapi.lib" )
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2::System;
using namespace ::Eldritch2;
using namespace ::utf8::unchecked;
using namespace ::utf8;
using namespace ::std;

namespace {

	enum FsInformationClass : ::DWORD {
		FileFsSizeInformation		= 3,
		FileFsSectorSizeInformation	= 11,
		FileStorageInfo				= 16
	};

// ---------------------------------------------------

	enum : ::DWORD {
		FILE_SHARE_NONE							= 0u,

	// ---

		AGGREGATE_SYNCHRONOUS_READ_ATTRIBUTES	= ( FILE_FLAG_POSIX_SEMANTICS | FILE_FLAG_SEQUENTIAL_SCAN ),
		AGGREGATE_SYNCHRONOUS_WRITE_ATTRIBUTES	= ( FILE_FLAG_POSIX_SEMANTICS | FILE_FLAG_SEQUENTIAL_SCAN ),
		AGGREGATE_READ_ATTRIBUTES				= ( AGGREGATE_SYNCHRONOUS_READ_ATTRIBUTES  | FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED ),
		AGGREGATE_WRITE_ATTRIBUTES				= ( AGGREGATE_SYNCHRONOUS_WRITE_ATTRIBUTES | FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED )
	};

// ---------------------------------------------------

	using GetSectorSizeFromHandleFunction	= size_t (*)( const ::HANDLE );
	using WidePath							= wchar_t[MAX_PATH];

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint static bool	FileExists( const wchar_t* const pathString ) {
		return( INVALID_FILE_ATTRIBUTES != ::GetFileAttributesW( pathString ) );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint static uint64	GetFileSizeInBytes( const ::HANDLE fileHandle ) {
		::LARGE_INTEGER	result;

		return( !!::GetFileSizeEx( fileHandle, &result ) ? static_cast<uint64>(result.QuadPart) : 0u );
	}

// ---------------------------------------------------

	template <size_t destinationBufferLength>
	static wchar_t*	UTF8FileNameToWidePath( wchar_t (&resultPath)[destinationBufferLength], const wchar_t* sourcePath, const UTF8Char* path ) {
		static_assert( destinationBufferLength >= MAX_PATH, "UTF8FileNameToWidePath() destination buffer is too small!" );

	// ---

		const size_t	pathLength( StringLength( path ) + 1 );
		wchar_t*		tempWideName( static_cast<wchar_t*>(_alloca( sizeof(wchar_t) * pathLength )) );

		if( !sourcePath || !is_valid( path, path + pathLength ) ) {
			return nullptr;
		}

		utf8to16( path, path + pathLength, tempWideName );

		return ::PathCombineW( resultPath, sourcePath, tempWideName );
	}

// ---------------------------------------------------

	static ETNoAliasHint GetSectorSizeFromHandleFunction GetSectorSizeQueryAPI( const System::Win32::SystemInterface& systemInterface ) {
		using NtStatus	= ::LONG;

	// ---

		struct FileFSSizeInformation {
			ETInlineHint FileFSSizeInformation() : BytesPerSector( 0u ) {}

			::LARGE_INTEGER	TotalAllocationUnits;
			::LARGE_INTEGER	AvailableAllocationUnits;
			::ULONG			SectorsPerAllocationUnit;
			::ULONG			BytesPerSector;
		};

	// ---

		struct IOStatusBlock {
			union {
				NtStatus	Status;
				::PVOID		Pointer;
			};

			::ULONG_PTR Information;
		};

	// ---

		static NtStatus (NTAPI* queryVolumeInformationFile)( ::HANDLE, IOStatusBlock* const, FileFSSizeInformation* const, ::ULONG, FsInformationClass );

	// ---------------------------------------------------

#if defined( _WIN32_WINNT_WIN8 ) && (_WIN32_WINNT >= _WIN32_WINNT_WIN8)
		if( systemInterface.IsWindows8OrNewer() ) {
			return [] ( const ::HANDLE fileHandle ) -> size_t {
				::FILE_ALIGNMENT_INFO	fileAlignmentInfo;
				return 0 != ::GetFileInformationByHandleEx( fileHandle, ::FileAlignmentInfo, &fileAlignmentInfo, sizeof(FileAlignmentInfo) ) ? Max<size_t>( fileAlignmentInfo.AlignmentRequirement, 1u ) : 0u;
			}
		}
#endif

		if( auto queryFunction = ::GetProcAddress( ::GetModuleHandle( SL("ntdll.dll") ), "NtQueryVolumeInformationFile" ) ) {
			queryVolumeInformationFile = reinterpret_cast<decltype(queryVolumeInformationFile)>(queryFunction);

			return [] ( const ::HANDLE fileHandle ) -> size_t {
				FileFSSizeInformation	sizeInformation;
				IOStatusBlock			statusBlock;

				if( INVALID_HANDLE_VALUE == fileHandle || (0 > queryVolumeInformationFile( fileHandle, &statusBlock, &sizeInformation, sizeof( sizeInformation ), FsInformationClass::FileFsSizeInformation )) ) {
					sizeInformation.BytesPerSector = 0u;
				}

				return static_cast<size_t>(sizeInformation.BytesPerSector);
			};
		}

		return [] ( const ::HANDLE /*fileHandle*/ ) -> size_t { return 0u; };
	}

}	// anonymous namespace

namespace Eldritch2 {
namespace FileSystem {
namespace Win32 {

	ContentProvider::ContentProvider( const System::Win32::SystemInterface& systemInterface ) : _getSectorSizeFromHandleFunction( GetSectorSizeQueryAPI( systemInterface ) ) {
		wchar_t*	folderPtr( nullptr );

		if( 0 != ::GetCurrentDirectoryW( _countof( _packageContentPath ), _packageContentPath ) ) {
			::PathAppendW( _packageContentPath, L"..\\..\\Content\\" );
		}

		// Retrieve the Documents library path for this machine.
		if( SUCCEEDED( ::SHGetKnownFolderPath( FOLDERID_Documents, 0, nullptr, &folderPtr ) ) ) {	
			::PathCombineW( _knownContentPaths[static_cast<size_t>(KnownContentLocation::USER_DOCUMENTS)], folderPtr, L"My Games\\" WPROJECT_NAME L"\\" );
			::CreateDirectoryW( _knownContentPaths[static_cast<size_t>(KnownContentLocation::USER_DOCUMENTS)], nullptr );
			::CoTaskMemFree( folderPtr );
		}

		// Retrieve the AppData_Local folder for this machine.
		if( SUCCEEDED( ::SHGetKnownFolderPath( FOLDERID_LocalAppData, 0, nullptr, &folderPtr ) ) ) {	
			::PathCombineW( _knownContentPaths[static_cast<size_t>(KnownContentLocation::APPDATA_LOCAL)], folderPtr, WPROJECT_NAME L"\\");
			::CreateDirectoryW( _knownContentPaths[static_cast<size_t>(KnownContentLocation::APPDATA_LOCAL)], nullptr );
			::CoTaskMemFree( folderPtr );
		}
	}

// ---------------------------------------------------

	Result<AsynchronousFileReader> ContentProvider::CreateAsynchronousFileReader( Allocator& allocator, const KnownContentLocation contentLocation, const UTF8Char* const fileName ) {
		WidePath		filePath;
		const ::HANDLE	file( ::CreateFileW( UTF8FileNameToWidePath( filePath, PathFromKnownContentLocation( contentLocation ), fileName ),
											 GENERIC_READ,
											 FILE_SHARE_READ,
											 nullptr,
											 OPEN_EXISTING,
											 AGGREGATE_READ_ATTRIBUTES,
											 nullptr ) );

		// GetSectorSizeFromHandle returns 0 in the event sector information is unavailable.
		if( const size_t diskSectorSizeInBytes = _getSectorSizeFromHandleFunction( file ) ) {
			if( auto accessor = new(allocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) AsynchronousFileReader( file, diskSectorSizeInBytes ) ) {
				return { move( accessor ) };
			}
		}

		// If the reader creation failed, we're responsible for closing the file handle here (the reader will not be able to do this in the destructor)
		::CloseHandle( file );

		return { Error::BAD_FILE_NAME };
	}

// ---------------------------------------------------

	Result<FileSystem::SynchronousFileReader> ContentProvider::CreateSynchronousFileReader( Allocator& allocator, const KnownContentLocation contentLocation, const UTF8Char* const fileName ) {
		WidePath		filePath;
		const ::HANDLE	file( ::CreateFileW( UTF8FileNameToWidePath( filePath, PathFromKnownContentLocation( contentLocation ), fileName ),
											 GENERIC_READ,
											 FILE_SHARE_READ,
											 nullptr,
											 OPEN_EXISTING,
											 AGGREGATE_SYNCHRONOUS_READ_ATTRIBUTES,
											 nullptr ) );

		// GetSectorSizeFromHandle returns 0 in the event sector information is unavailable.
		if( const size_t diskSectorSizeInBytes = _getSectorSizeFromHandleFunction( file ) ) {
			if( auto accessor = new(allocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) SynchronousFileReader( file, diskSectorSizeInBytes ) ) {
				return { move( accessor ) };
			}
		}

		// If the reader creation failed, we're responsible for closing the file handle here (the reader will not be able to do this in the destructor)
		::CloseHandle( file );

		return { Error::BAD_FILE_NAME };
	}

// ---------------------------------------------------

	Result<FileSystem::ReadableMemoryMappedFile> ContentProvider::CreateReadableMemoryMappedFile( Allocator& allocator, const KnownContentLocation contentLocation, const UTF8Char* const fileName ) {
		using AllocationOption	= Allocator::AllocationOption;

	// ---

		WidePath		filePath;
		const ::HANDLE	file( ::CreateFileW( UTF8FileNameToWidePath( filePath, PathFromKnownContentLocation( contentLocation ), fileName ),
											 GENERIC_READ,
											 FILE_SHARE_READ,
											 nullptr,
											 OPEN_EXISTING,
											 AGGREGATE_SYNCHRONOUS_READ_ATTRIBUTES,
											 nullptr ) );

		if( const ::HANDLE fileMapping { ::CreateFileMapping( file, nullptr, PAGE_READONLY, static_cast<::DWORD>(0u), static_cast<::DWORD>(0u), nullptr ) } ) {
			if( const void* const mappedView = ::MapViewOfFile( fileMapping, FILE_MAP_READ, static_cast<::DWORD>(0u), static_cast<::DWORD>(0u), static_cast<::SIZE_T>(0u) ) ) {
				if( auto resultObject = new(allocator, AllocationOption::TEMPORARY_ALLOCATION) ReadableMemoryMappedFile( Range<const char*>( static_cast<const char*>(mappedView), GetFileSizeInBytes( file ) ) ) ) {
					return { move( resultObject ) };
				}
			}

			::CloseHandle( fileMapping );
		}

		::CloseHandle( file );

		return { Error::BAD_FILE_NAME };
	}

// ---------------------------------------------------

	Result<AsynchronousFileWriter> ContentProvider::CreateAsynchronousFileWriter( Allocator& allocator, const KnownContentLocation contentLocation, const UTF8Char* const fileName, const FileOverwriteBehavior overwriteBehavior ) {
		WidePath		filePath;
		const ::HANDLE	file( ::CreateFileW( UTF8FileNameToWidePath( filePath, PathFromKnownContentLocation( contentLocation ), fileName ),
											 GENERIC_WRITE,
											 FILE_SHARE_WRITE,
											 nullptr,
											 FileOverwriteBehavior::FAIL_IF_FILE_EXISTS == overwriteBehavior ? CREATE_NEW : CREATE_ALWAYS,
											 AGGREGATE_WRITE_ATTRIBUTES,
											 nullptr ) );

		// GetSectorSizeFromHandle returns 0 in the event sector information is unavailable.
		if( const size_t diskSectorSizeInBytes = _getSectorSizeFromHandleFunction( file ) ) {
			if( auto accessor = new(allocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) AsynchronousFileWriter( file, diskSectorSizeInBytes ) ) {
				return { move( accessor ) };
			}
		}

		// If the writer creation failed, we're responsible for closing the file handle here (the writer will not be able to do this in the destructor)
		::CloseHandle( file );

		return { Error::BAD_FILE_NAME };
	}

// ---------------------------------------------------

	Result<SynchronousFileWriter> ContentProvider::CreateSynchronousFileWriter( Allocator& allocator, const KnownContentLocation contentLocation, const UTF8Char* const fileName, const FileOverwriteBehavior overwriteBehavior ) {
		WidePath		filePath;
		const ::HANDLE	file( ::CreateFileW( UTF8FileNameToWidePath( filePath, PathFromKnownContentLocation( contentLocation ), fileName ),
											 GENERIC_WRITE,
											 FILE_SHARE_WRITE,
											 nullptr,
											 FileOverwriteBehavior::FAIL_IF_FILE_EXISTS == overwriteBehavior ? CREATE_NEW : CREATE_ALWAYS,
											 AGGREGATE_SYNCHRONOUS_WRITE_ATTRIBUTES,
											 nullptr ) );

		// GetSectorSizeFromHandle returns 0 in the event sector information is unavailable.
		if( const size_t diskSectorSizeInBytes = _getSectorSizeFromHandleFunction( file ) ) {
			if( auto accessor = new(allocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) SynchronousFileWriter( file, diskSectorSizeInBytes ) ) {
				return { move( accessor ) };
			}
		}

		// If the writer creation failed, we're responsible for closing the file handle here (the writer will not be able to do this in the destructor)
		::CloseHandle( file );

		return { Error::BAD_FILE_NAME };
	}

// ---------------------------------------------------

	Result<SynchronousFileAppender> ContentProvider::CreateSynchronousFileAppender( Allocator& allocator, const KnownContentLocation contentLocation, const UTF8Char* const fileName ) {
		class Win32SynchronousFileAppender : public FileSystem::SynchronousFileAppender {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref Win32SynchronousFileAppender instance.
			Win32SynchronousFileAppender( const ::HANDLE fileHandle ) : _fileHandle( fileHandle ) {}

			//! Destroys this @ref Win32SynchronousFileAppender instance.
			~Win32SynchronousFileAppender() {
				::CloseHandle( _fileHandle );
			}

		// ---------------------------------------------------

			ErrorCode Append( const void* const sourceData, const size_t lengthToWriteInBytes ) override {
				::DWORD	writtenLengthInBytes;

				return FALSE != ::WriteFile( _fileHandle, sourceData, static_cast<::DWORD>(lengthToWriteInBytes), &writtenLengthInBytes, nullptr ) ? Error::NONE : Error::UNSPECIFIED;
			}

		// - DATA MEMBERS ------------------------------------

		private:
			const ::HANDLE	_fileHandle;
		};

	// ---

		WidePath		filePath;
		const ::HANDLE	file( ::CreateFileW( UTF8FileNameToWidePath( filePath, PathFromKnownContentLocation( contentLocation ), fileName ),
											 FILE_APPEND_DATA,
											 FILE_SHARE_READ,
											 nullptr,
											 OPEN_ALWAYS,
											 AGGREGATE_SYNCHRONOUS_WRITE_ATTRIBUTES,
											 nullptr ) );

		if( INVALID_HANDLE_VALUE != file ) {
			if( auto appender = new(allocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) Win32SynchronousFileAppender( file ) ) {
				return { move( appender ) };
			}

			return { Error::OUT_OF_MEMORY };
		}

		// If the writer creation failed, we're responsible for closing the file handle here (the writer will not be able to do this in the destructor)
		::CloseHandle( file );

		return { Error::BAD_FILE_NAME };
	}

// ---------------------------------------------------

	void ContentProvider::CopyFreeFile( const KnownContentLocation contentLocation, const UTF8Char* const destinationFileName, const UTF8Char* const sourceFileName, const FileOverwriteBehavior overwriteBehavior ) {
		WidePath	destinationFilePath;
		WidePath	sourceFilePath;

		if( UTF8FileNameToWidePath( destinationFilePath, PathFromKnownContentLocation( contentLocation ), destinationFileName ) && UTF8FileNameToWidePath( sourceFilePath, PathFromKnownContentLocation( contentLocation ), sourceFileName ) ) {
			::CopyFileW( sourceFilePath, destinationFilePath, FileOverwriteBehavior::OVERWRITE_IF_FILE_EXISTS == overwriteBehavior ? FALSE : TRUE );
		}
	}

// ---------------------------------------------------

	void ContentProvider::DeleteFreeFile( KnownContentLocation contentLocation, const UTF8Char* const fileName ) {
		WidePath	filePath;

		if( UTF8FileNameToWidePath( filePath, PathFromKnownContentLocation( contentLocation ), fileName ) ) {
			::DeleteFileW( filePath );
		}
	}

}	// namespace Win32
}	// namespace Foundation
}	// namespace Eldritch2