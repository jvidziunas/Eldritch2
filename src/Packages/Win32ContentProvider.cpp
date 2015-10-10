/*==================================================================*\
  Win32ContentProvider.cpp
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
#include <Utility/ReadableMemoryMappedFile.hpp>
#include <Utility/SynchronousFileAppender.hpp>
#include <Utility/Win32ApplicationHelpers.hpp>
#include <Packages/Win32ContentProvider.hpp>
#include <System/Win32SystemInterface.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Utility/Memory/MemStdLib.hpp>
#include <Utility/Concurrency/Lock.hpp>
#include <Scheduler/TaskScheduler.hpp>
#include <Utility/ResultPair.hpp>
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
using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;
using namespace ::utf8::unchecked;
using namespace ::utf8;
using namespace ::std;

#ifdef ZeroMemory
#	if( ET_COMPILER_IS_MSVC )
#		define POP_ZeroMemory 1
		COMPILERMESSAGEGENERIC( "Cleaning up ZeroMemory macro for use in the Windows content provider." )
#		pragma push_macro( "ZeroMemory" )
#		undef ZeroMemory
#	else
#		error ZeroMemory macro creates symbol/name resolution errors, replace me with cleanup code!
#	endif
#endif
#ifdef StrCat
#	if( ET_COMPILER_IS_MSVC )
#		define POP_StrCat 1
		COMPILERMESSAGEGENERIC( "Cleaning up StrCmp macro for use in the Windows content provider." )
#		pragma push_macro( "StrCat" )
#		undef StrCat
#	else
#		error StrCat macro creates symbol/name resolution errors, replace me with cleanup code!
#	endif
#endif

#if defined( _WIN32_WINNT_WIN8 ) && ( _WIN32_WINNT >= _WIN32_WINNT_WIN8 )
#	define WIN8_MEMORY_MAPPED_FILE_AVAILABLE 0
#else
#	if( ET_COMPILER_IS_MSVC )
#	define WIN8_MEMORY_MAPPED_FILE_AVAILABLE 0
	COMPILERMESSAGEGENERIC( "Windows 8 PrefetchVirtualMemory-optimized file mapping object unavailable, use the Windows 8 SDK or newer to enable" )
#	endif
#endif

namespace {

	namespace FileSystemInformationClass {

		enum FsInformationClass : ::DWORD {
			FileFsSizeInformation = 3,
			FileFsSectorSizeInformation = 11,
			FileStorageInfo = 16
		};

	}	// namespace FileSystemInformationClass

	typedef FileSystemInformationClass::FsInformationClass	FsInformationClass;

// ---------------------------------------------------

	enum : size_t {
		DIR_LENGTH = MAX_PATH
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

	typedef size_t (*GetSectorSizeFromHandleFunction)( const ::HANDLE );

// ---------------------------------------------------

	static const UTF8Char	configSectionName[]	= UTF8L("Content.Win32ContentProvider");

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint static bool	FileExists( const wchar_t* const pathString ) {
		return( INVALID_FILE_ATTRIBUTES != ::GetFileAttributesW( pathString ) );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint static bool	IsFatalErrorCondition( const ::DWORD errorCode ) {
		return !( ( NO_ERROR == errorCode ) | ( ERROR_IO_PENDING == errorCode ) );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint static uint64	GetFileSizeInBytes( const ::HANDLE fileHandle ) {
		::LARGE_INTEGER	result;

		return( !!::GetFileSizeEx( fileHandle, &result ) ? static_cast<uint64>(result.QuadPart) : 0u );
	}

// ---------------------------------------------------

	template <size_t destinationBufferLength>
	static wchar_t*	UTF8FileNameToWidePath( wchar_t (&resultPath)[destinationBufferLength], const wchar_t* sourcePath, const UTF8Char* path ) {
		ETCompileTimeAssert( destinationBufferLength >= MAX_PATH );

		const size_t	pathLength( StringLength( path ) + 1 );
		wchar_t*		tempWideName( static_cast<wchar_t*>(_alloca( sizeof(wchar_t) * pathLength )) );

		if( !sourcePath || !is_valid( path, path + pathLength ) ) {
			return nullptr;
		}

		utf8to16( path, path + pathLength, tempWideName );

		return ::PathCombineW( resultPath, sourcePath, tempWideName );
	}

// ---------------------------------------------------

	static ETNoAliasHint GetSectorSizeFromHandleFunction GetSectorSizeQueryAPI( const Win32SystemInterface& systemInterface ) {
		typedef ::LONG	NtStatus;

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

		struct FunctionContainer {
#if defined( _WIN32_WINNT_WIN8 ) && (_WIN32_WINNT >= _WIN32_WINNT_WIN8)
			static size_t GetSectorSizeFromHandleWindows8( const ::HANDLE fileHandle ) {
				::FILE_ALIGNMENT_INFO	fileAlignmentInfo;
				return 0 != ::GetFileInformationByHandleEx( fileHandle, ::FileAlignmentInfo, &fileAlignmentInfo, sizeof(FileAlignmentInfo) ) ? Max<size_t>( fileAlignmentInfo.AlignmentRequirement, 1u ) : 0u;
			}
#endif

		// ---------------------------------------------------

			static size_t GetSectorSizeFromHandleWindows7( const ::HANDLE fileHandle ) {
				FileFSSizeInformation	sizeInformation;
				IOStatusBlock			statusBlock;

				if( INVALID_HANDLE_VALUE == fileHandle || (0 > queryVolumeInformationFile( fileHandle, &statusBlock, &sizeInformation, sizeof(sizeInformation), FileSystemInformationClass::FileFsSizeInformation )) ) {
					sizeInformation.BytesPerSector = 0u;
				}

				return static_cast<size_t>(sizeInformation.BytesPerSector);
			}

		// ---------------------------------------------------

			static size_t GetSectorSizeFromHandleNull( const ::HANDLE /*fileHandle*/ ) {
				return 0u;
			}
		};

	// ---

#if defined( _WIN32_WINNT_WIN8 ) && (_WIN32_WINNT >= _WIN32_WINNT_WIN8)
		if( systemInterface.IsWindows8OrNewer() ) {
			return &FunctionContainer::GetSectorSizeFromHandleWindows8;
		}
#endif

		if( auto* const queryFunction = reinterpret_cast<decltype(queryVolumeInformationFile)>(::GetProcAddress( GetModuleHandle( SL("ntdll.dll") ), "NtQueryVolumeInformationFile" )) ) {
			queryVolumeInformationFile = queryFunction;
			return &FunctionContainer::GetSectorSizeFromHandleWindows7;
		}

		return &FunctionContainer::GetSectorSizeFromHandleNull;
	}

}	// anonymous namespace

#include <Packages/Win32/Win32SynchronousFileAccessor.cpp>
#include <Packages/Win32/Win32AsynchronousFileAccessor.cpp>

namespace Eldritch2 {
namespace FileSystem {

	Win32ContentProvider::Win32ContentProvider( const Win32SystemInterface& systemInterface ) : ContentProvider(), _getSectorSizeFromHandleFunction( GetSectorSizeQueryAPI( systemInterface ) ) {
		wchar_t*	folderPtr( nullptr );

		::GetCurrentDirectoryW( _countof(_packageContentPath), _packageContentPath );
		::PathAppendW( _packageContentPath, L"..\\..\\Content\\" );

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

	Win32ContentProvider::~Win32ContentProvider() {}

// ---------------------------------------------------

	ResultPair<AsynchronousFileReader> Win32ContentProvider::CreateAsynchronousFileReader( Allocator& allocator, const KnownContentLocation contentLocation, const UTF8Char* const fileName ) {
		ResultPair<AsynchronousFileReader>	result { nullptr, Errors::BAD_FILE_NAME };
		wchar_t								filePath[DIR_LENGTH];
		const ::HANDLE						file( ::CreateFileW( UTF8FileNameToWidePath( filePath, PathFromKnownContentLocation( contentLocation ), fileName ),
																 GENERIC_READ,
																 FILE_SHARE_READ,
																 nullptr,
																 OPEN_EXISTING,
																 AGGREGATE_READ_ATTRIBUTES,
																 nullptr ) );

		// GetSectorSizeFromHandle returns 0 in the event sector information is unavailable.
		if( const size_t diskSectorSizeInBytes = _getSectorSizeFromHandleFunction( file ) ) {
			result.object		= new( allocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION ) Win32AsynchronousFileAccessor( file, diskSectorSizeInBytes );
			result.resultCode	= result.object ? Errors::NONE : Errors::OUT_OF_MEMORY;
		}

		// If the reader creation failed, we're responsible for closing the file handle here (the reader will not be able to do this in the destructor)
		if( !result ) {
			::CloseHandle( file );
		}

		return result;
	}

// ---------------------------------------------------

	ResultPair<SynchronousFileReader> Win32ContentProvider::CreateSynchronousFileReader( Allocator& allocator, const KnownContentLocation contentLocation, const UTF8Char* const fileName ) {
		ResultPair<SynchronousFileReader>	result { nullptr, Errors::BAD_FILE_NAME };
		wchar_t								filePath[DIR_LENGTH];
		const ::HANDLE						file( ::CreateFileW( UTF8FileNameToWidePath( filePath, PathFromKnownContentLocation( contentLocation ), fileName ),
																 GENERIC_READ,
																 FILE_SHARE_READ,
																 nullptr,
																 OPEN_EXISTING,
																 AGGREGATE_SYNCHRONOUS_READ_ATTRIBUTES,
																 nullptr ) );

		// GetSectorSizeFromHandle returns 0 in the event sector information is unavailable.
		if( const size_t diskSectorSizeInBytes = _getSectorSizeFromHandleFunction( file ) ) {
			result.object		= new( allocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION ) Win32SynchronousFileAccessor( file, diskSectorSizeInBytes );
			result.resultCode	= result.object ? Errors::NONE : Errors::OUT_OF_MEMORY;
		}

		// If the reader creation failed, we're responsible for closing the file handle here (the reader will not be able to do this in the destructor)
		if( !result ) {
			::CloseHandle( file );
		}

		return result;
	}

// ---------------------------------------------------

	ResultPair<ReadableMemoryMappedFile> Win32ContentProvider::CreateReadableMemoryMappedFile( Allocator& allocator, const KnownContentLocation contentLocation, const UTF8Char* const fileName ) {
		class Win32ReadableMemoryMappedFile : public ReadableMemoryMappedFile {
		public:
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			// Constructs this Win32ReadableMemoryMappedFile instance.
			Win32ReadableMemoryMappedFile( const Range<const char*> mappedRegion ) : _region( mappedRegion ) {}

			// Destroys this Win32ReadableMemoryMappedFile instance.
			~Win32ReadableMemoryMappedFile() {
				::UnmapViewOfFile( _region.first );
			}

		// ---------------------------------------------------

			size_t GetAccessibleRegionSizeInBytes() const override sealed {
				return _region.Size();
			}

		// ---------------------------------------------------

			void PrefetchRangeForRead( const Range<const char*> memoryRange ) const override {
				enum : size_t {
					PREFETCH_STRIDE	= 4096u
				};

			// ---

				char readTarget;

				for( const char* temp( memoryRange.first ); temp < memoryRange.onePastLast; temp += PREFETCH_STRIDE ) {
					readTarget = *temp;
				}
			}

		// ---------------------------------------------------

			const void* GetAddressForFileByteOffset( const size_t rawFileOffsetInBytes ) const override sealed {
				return _region.first + rawFileOffsetInBytes;
			}

		// - DATA MEMBERS ------------------------------------

		private:
			const Range<const char*>	_region;
		};

	// ---------------------------------------------------

#if( WIN8_MEMORY_MAPPED_FILE_AVAILABLE )
		class Windows8ReadableMemoryMappedFile : public Win32ReadableMemoryMappedFile {
		public:
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			// Constructs this Windows8ReadableMemoryMappedFile instance.
			Windows8ReadableMemoryMappedFile( const Range<const char*> mappedRegion ) : Win32ReadableMemoryMappedFile( mappedRegion ) {}

			// Destroys this Windows8ReadableMemoryMappedFile instance.
			~Windows8ReadableMemoryMappedFile() {}

		// ---------------------------------------------------

			void PrefetchRangeForRead( const Range<const char*> memoryRange ) const override {
				::WIN32_MEMORY_RANGE_ENTRY	ranges[] = { { const_cast<char*>(memoryRange.first), memoryRange.Size() } };

				::PrefetchVirtualMemory( ::GetCurrentProcess(), _countof( ranges ), ranges, 0 );
			}
		};
#endif

	// ---

		ResultPair<ReadableMemoryMappedFile>	result { nullptr, Errors::BAD_FILE_NAME };
		wchar_t									filePath[DIR_LENGTH];
		const ::HANDLE							file( ::CreateFileW( UTF8FileNameToWidePath( filePath, PathFromKnownContentLocation( contentLocation ), fileName ),
																	 GENERIC_READ,
																	 FILE_SHARE_READ,
																	 nullptr,
																	 OPEN_EXISTING,
																	 AGGREGATE_SYNCHRONOUS_READ_ATTRIBUTES,
																	 nullptr ) );

		if( const ::HANDLE fileMapping = ::CreateFileMapping( file, nullptr, PAGE_READONLY, static_cast<::DWORD>(0u), static_cast<::DWORD>(0u), nullptr ) ) {
			if( const void* const mappedView = ::MapViewOfFile( fileMapping, FILE_MAP_READ, static_cast<::DWORD>(0u), static_cast<::DWORD>(0u), static_cast<::SIZE_T>(0u) ) ) {
				result.object		= new( allocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION ) Win32ReadableMemoryMappedFile( Range<const char*>( static_cast<const char*>(mappedView), GetFileSizeInBytes( file ) ) );
				result.resultCode	= result.object ? Errors::NONE : Errors::OUT_OF_MEMORY;
			}

			::CloseHandle( fileMapping );
		}

		::CloseHandle( file );

		return result;
	}

// ---------------------------------------------------

	ResultPair<AsynchronousFileWriter> Win32ContentProvider::CreateAsynchronousFileWriter( Allocator& allocator, const KnownContentLocation contentLocation, const UTF8Char* const fileName, const FileOverwriteBehavior overwriteBehavior ) {
		ResultPair<AsynchronousFileWriter>	result { nullptr, Errors::BAD_FILE_NAME };
		wchar_t								filePath[DIR_LENGTH];
		const ::HANDLE						file( ::CreateFileW( UTF8FileNameToWidePath( filePath, PathFromKnownContentLocation( contentLocation ), fileName ),
																 GENERIC_WRITE,
																 FILE_SHARE_WRITE,
																 nullptr,
																 FileOverwriteBehavior::FAIL_IF_FILE_EXISTS == overwriteBehavior ? CREATE_NEW : CREATE_ALWAYS,
																 AGGREGATE_WRITE_ATTRIBUTES,
																 nullptr ) );

		// GetSectorSizeFromHandle returns 0 in the event sector information is unavailable.
		if( const size_t diskSectorSizeInBytes = _getSectorSizeFromHandleFunction( file ) ) {
			result.object		= new( allocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION ) Win32AsynchronousFileAccessor( file, diskSectorSizeInBytes );
			result.resultCode	= result.object ? Errors::NONE : Errors::OUT_OF_MEMORY;
		}

		// If the writer creation failed, we're responsible for closing the file handle here (the writer will not be able to do this in the destructor)
		if( !result ) {
			::CloseHandle( file );
		}

		return result;
	}

// ---------------------------------------------------

	ResultPair<SynchronousFileWriter> Win32ContentProvider::CreateSynchronousFileWriter( Allocator& allocator, const KnownContentLocation contentLocation, const UTF8Char* const fileName, const FileOverwriteBehavior overwriteBehavior ) {
		ResultPair<SynchronousFileWriter>	result{ nullptr, Errors::BAD_FILE_NAME };
		wchar_t								filePath[DIR_LENGTH];
		const ::HANDLE						file( ::CreateFileW( UTF8FileNameToWidePath( filePath, PathFromKnownContentLocation( contentLocation ), fileName ),
																 GENERIC_WRITE,
																 FILE_SHARE_WRITE,
																 nullptr,
																 FileOverwriteBehavior::FAIL_IF_FILE_EXISTS == overwriteBehavior ? CREATE_NEW : CREATE_ALWAYS,
																 AGGREGATE_SYNCHRONOUS_WRITE_ATTRIBUTES,
																 nullptr ) );

		// GetSectorSizeFromHandle returns 0 in the event sector information is unavailable.
		if( const size_t diskSectorSizeInBytes = _getSectorSizeFromHandleFunction( file ) ) {
			result.object		= new( allocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION ) Win32SynchronousFileAccessor( file, diskSectorSizeInBytes );
			result.resultCode	= result.object ? Errors::NONE : Errors::OUT_OF_MEMORY;
		}

		// If the writer creation failed, we're responsible for closing the file handle here (the writer will not be able to do this in the destructor)
		if( !result ) {
			::CloseHandle( file );
		}

		return result;
	}

// ---------------------------------------------------

	ResultPair<SynchronousFileAppender> Win32ContentProvider::CreateSynchronousFileAppender( Allocator& allocator, const KnownContentLocation contentLocation, const UTF8Char* const fileName ) {
		class Win32SynchronousFileAppender : public SynchronousFileAppender {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			// Constructs this Win32SynchronousFileAppender instance.
			Win32SynchronousFileAppender( const ::HANDLE fileHandle ) : _fileHandle( fileHandle ) {}

			// Destroys this Win32SynchronousFileAppender instance.
			~Win32SynchronousFileAppender() {
				::CloseHandle( _fileHandle );
			}

		// ---------------------------------------------------

			ErrorCode Append( const void* const sourceData, const size_t lengthToWriteInBytes ) override sealed {
				::DWORD	writtenLengthInBytes;

				return FALSE != ::WriteFile( _fileHandle, sourceData, static_cast<::DWORD>(lengthToWriteInBytes), &writtenLengthInBytes, nullptr ) ? Errors::NONE : Errors::UNSPECIFIED;
			}

		// - DATA MEMBERS ------------------------------------

		private:
			const ::HANDLE	_fileHandle;
		};

	// ---

		ResultPair<SynchronousFileAppender>	result{ nullptr, Errors::BAD_FILE_NAME };
		wchar_t								filePath[DIR_LENGTH];
		const ::HANDLE						file( ::CreateFileW( UTF8FileNameToWidePath( filePath, PathFromKnownContentLocation( contentLocation ), fileName ),
																 FILE_APPEND_DATA,
																 FILE_SHARE_READ,
																 nullptr,
																 OPEN_ALWAYS,
																 AGGREGATE_SYNCHRONOUS_WRITE_ATTRIBUTES,
																 nullptr ) );

		result.object		= new( allocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION ) Win32SynchronousFileAppender( file );
		result.resultCode	= result.object ? Errors::NONE : Errors::OUT_OF_MEMORY;

		// If the writer creation failed, we're responsible for closing the file handle here (the writer will not be able to do this in the destructor)
		if( !result ) {
			::CloseHandle( file );
		}

		return result;
	}

// ---------------------------------------------------

	void Win32ContentProvider::CopyFreeFile( const KnownContentLocation contentLocation, const UTF8Char* const destinationFileName, const UTF8Char* const sourceFileName, const FileOverwriteBehavior overwriteBehavior ) {
		wchar_t	destinationFilePath[DIR_LENGTH];
		wchar_t	sourceFilePath[DIR_LENGTH];

		if( UTF8FileNameToWidePath( destinationFilePath, PathFromKnownContentLocation( contentLocation ), destinationFileName ) && UTF8FileNameToWidePath( sourceFilePath, PathFromKnownContentLocation( contentLocation ), sourceFileName ) ) {
			::CopyFileW( sourceFilePath, destinationFilePath, FileOverwriteBehavior::OVERWRITE_IF_FILE_EXISTS == overwriteBehavior ? FALSE : TRUE );
		}
	}

// ---------------------------------------------------

	void Win32ContentProvider::DeleteFreeFile( KnownContentLocation contentLocation, const UTF8Char* const fileName ) {
		wchar_t	filePath[DIR_LENGTH];

		if( UTF8FileNameToWidePath( filePath, PathFromKnownContentLocation( contentLocation ), fileName ) ) {
			::DeleteFileW( filePath );
		}
	}

}	// namespace Foundation
}	// namespace Eldritch2

#if( ET_COMPILER_IS_MSVC )
#	ifdef POP_ZeroMemory
		COMPILERMESSAGEGENERIC( "Reinstating ZeroMemory macro." )
#		pragma pop_macro( "ZeroMemory" )
#		undef POP_ZeroMemory
#	endif
#	ifdef POP_StrCat
		COMPILERMESSAGEGENERIC( "Reinstating StrCat macro." )
#		pragma pop_macro( "StrCat" )
#		undef POP_StrCat
#	endif
#endif