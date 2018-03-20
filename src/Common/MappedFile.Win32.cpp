/*==================================================================*\
  MappedFile.Win32.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/MappedFile.hpp>
#include <Common/ScopeGuard.hpp>
#include <Common/ErrorCode.hpp>
#include <Common/Memory.hpp>
#include <Common/Math.hpp>
//------------------------------------------------------------------//
#include <eastl/utility.h>
#include <Windows.h>
//------------------------------------------------------------------//

#if defined( _WIN32_WINNT_WIN8 ) && ( _WIN32_WINNT >= _WIN32_WINNT_WIN8 )
#	define WIN8_MEMORY_MAPPED_FILE_AVAILABLE 1
#else
#	define WIN8_MEMORY_MAPPED_FILE_AVAILABLE 0
	COMPILERMESSAGEGENERIC( "Windows 8 PrefetchVirtualMemory-optimized file mapping object unavailable, use the Windows 8 SDK or newer to enable" )
#endif

namespace Eldritch2 {
namespace {

	enum : size_t {
		PrefetchStride = 4096u
	};

// ---

	static ETInlineHint ETPureFunctionHint DWORD FilePermissionsFromAccessMode( MappedFile::AccessMode mode ) {
		return mode < MappedFile::AccessMode::Write ? GENERIC_READ : ( GENERIC_READ | GENERIC_WRITE );
	}

// ---------------------------------------------------

	static ETInlineHint ETPureFunctionHint DWORD ProtectModeFromAccessMode( MappedFile::AccessMode mode ) {
		return mode < MappedFile::AccessMode::Write ? PAGE_READONLY : PAGE_READWRITE;
	}

// ---------------------------------------------------

	static ETInlineHint ETPureFunctionHint DWORD MapModeFromAccessMode( MappedFile::AccessMode mode ) {
	//	Officially, FILE_MAP_WRITE implies FILE_MAP_READ but this is still a little more clear.
		return mode < MappedFile::AccessMode::Write ? FILE_MAP_READ : (FILE_MAP_READ | FILE_MAP_WRITE);
	}

// ---------------------------------------------------

	static ETInlineHint ETPureFunctionHint Range<char*> MapFile( const PlatformChar* const path, DWORD creationDisposition, MappedFile::AccessMode accessMode, uint64 offsetInBytes, size_t mappingSizeInBytes ) {
	//	Begin by creating a handle to the file.
		const HANDLE file( CreateFileW( path, FilePermissionsFromAccessMode( accessMode ), FILE_SHARE_READ, nullptr, creationDisposition, FILE_FLAG_POSIX_SEMANTICS, nullptr ) );
		if (file == INVALID_HANDLE_VALUE) {
			return Range<char*>();
		}
		ET_AT_SCOPE_EXIT( CloseHandle( file ) );

		if (mappingSizeInBytes == 0u) {
			BY_HANDLE_FILE_INFORMATION	fileInfo;

			if (GetFileInformationByHandle( file, &fileInfo ) == 0) {
				return Range<char*>();
			}

			mappingSizeInBytes = (static_cast<uint64>(fileInfo.nFileSizeHigh) << 32u) | fileInfo.nFileSizeLow;
		}

	//	Next, map a range of the file according to the desired size.
		ULARGE_INTEGER	sizeHelper;
		sizeHelper.QuadPart	= mappingSizeInBytes;

		const HANDLE mapping( CreateFileMappingW( file, nullptr, ProtectModeFromAccessMode( accessMode ), sizeHelper.HighPart, sizeHelper.LowPart, nullptr ) );
		if (!mapping ) {
			return Range<char*>();
		}
		ET_AT_SCOPE_EXIT( CloseHandle( mapping ) );

		ULARGE_INTEGER	offsetHelper;
		offsetHelper.QuadPart	= offsetInBytes;

		char* const view( static_cast<char*>(MapViewOfFile( mapping, MapModeFromAccessMode( accessMode ), offsetHelper.HighPart, offsetHelper.LowPart, mappingSizeInBytes )) );
		if (!view) {
			return Range<char*>();
		}

		return { view, view + mappingSizeInBytes };
	}

}	// anonymous namespace

	MappedFile::MappedFile( MappedFile&& other ) : _access( other._access ), _region( eastl::exchange( other._region, Range<char*>() ) ) {}

// ---------------------------------------------------

	MappedFile::MappedFile() : _access(), _region() {}

// ---------------------------------------------------

	MappedFile::~MappedFile() {
		if (_region.IsEmpty()) {
			return;
		}

		UnmapViewOfFile( _region.Begin() );
	}

// ---------------------------------------------------

	Eldritch2::ErrorCode MappedFile::ClearOrCreate( AccessMode accessMode, const PlatformChar* path, size_t fileSizeInBytes ) {
	/*	The client must explicitly specify a size when creating a file; we cannot infer a value based on the size of the file since we may not be opening
	 *	something that already exists. */
		if (fileSizeInBytes == EndOfFile || fileSizeInBytes == 0u) {
			return Error::InvalidParameter;
		}

		Range<char*> region( MapFile( path, TRUNCATE_EXISTING, accessMode, 0u, fileSizeInBytes ) );
		ET_FAIL_UNLESS( region.IsEmpty() ? Error::InvalidParameter : Error::None );

		Swap( _region, region );

		if (!region.IsEmpty()) {
			UnmapViewOfFile( region.Begin() );
		}

		return Error::None;
	}

// ---------------------------------------------------

	Eldritch2::ErrorCode MappedFile::Open( AccessMode accessMode, const PlatformChar* path, uint64 offsetInBytes /*= StartOfFile*/, size_t mappedLengthInBytes /*= EndOfFile */ ) {
		Range<char*> region( MapFile( path, OPEN_EXISTING, accessMode, offsetInBytes, (EndOfFile == mappedLengthInBytes ? 0u : mappedLengthInBytes) ) );
		ET_FAIL_UNLESS( region.IsEmpty() ? Error::InvalidParameter : Error::None );

		Swap( _region, region );

		if (!region.IsEmpty()) {
			UnmapViewOfFile( region.Begin() );
		}

		return Error::None;
	}

// ---------------------------------------------------

	bool MappedFile::HasAccessLevel( AccessMode access ) const {
		return access <= _access;
	}

// ---------------------------------------------------

	size_t MappedFile::GetSizeInBytes() const {
		return _region.GetSize();
	}

// ---------------------------------------------------

	void MappedFile::Prefetch( size_t offsetInBytes, size_t rangeSizeInBytes ) const {
		Prefetch( GetRangeAtOffset<const char>( offsetInBytes, rangeSizeInBytes ) );
	}

// ---------------------------------------------------

	void MappedFile::Prefetch( Range<const char*> range ) const {
#	if WIN8_MEMORY_MAPPED_FILE_AVAILABLE
		WIN32_MEMORY_RANGE_ENTRY	ranges[] = { { const_cast<char*>( range.Begin() ), range.GetSize() } };

		PrefetchVirtualMemory( GetCurrentProcess(), _countof( ranges ), ranges, 0 );
#	else
		while (range) {
		//	TODO: Is it possible to use prefetch instructions to avoid polluting the processor caches?
			register int readTarget( *reinterpret_cast<const volatile char*>(range.Begin()) );
			static_cast<void>(readTarget);

			range.SetBegin( Min( range.Begin() + PrefetchStride, range.End() ) );
		}
#	endif
	}

// ---------------------------------------------------

	void MappedFile::Evict( size_t offsetInBytes, size_t rangeSizeInBytes ) const {
		Evict( GetRangeAtOffset<const char>( offsetInBytes, rangeSizeInBytes ) );
	}

// ---------------------------------------------------

	void MappedFile::Evict( Range<const char*> /*range*/ ) const {
	//	Unlike POSIX, Windows lacks any mechanism to evict pages from the cache, so this is a no-op.
	}

// ---------------------------------------------------

	void* MappedFile::GetAddressForByteOffset( size_t rawFileOffsetInBytes ) const {
		char* const	result( _region.Begin() + rawFileOffsetInBytes );

		return result <= _region.End() ? result : nullptr;
	}

// ---------------------------------------------------

	MappedFile& MappedFile::operator=( MappedFile&& other ) {
		if (_region) {
			UnmapViewOfFile( _region.Begin() );
		}

		_access	= eastl::move( other._access );
		_region	= eastl::exchange( other._region, Range<char*>() );

		return *this;
	}

}	// namespace Eldritch2