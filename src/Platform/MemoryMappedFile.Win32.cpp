/*==================================================================*\
  MemoryMappedFile.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/StandardLibrary.hpp>
#include <Platform/MemoryMappedFile.hpp>
#include <Utility/Containers/Range.hpp>
#include <Utility/Result.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//

#if defined( _WIN32_WINNT_WIN8 ) && ( _WIN32_WINNT >= _WIN32_WINNT_WIN8 )
#	define WIN8_MEMORY_MAPPED_FILE_AVAILABLE 1
#else
#	define WIN8_MEMORY_MAPPED_FILE_AVAILABLE 0
	COMPILERMESSAGEGENERIC( "Windows 8 PrefetchVirtualMemory-optimized file mapping object unavailable, use the Windows 8 SDK or newer to enable" )
#endif

namespace Eldritch2 {
namespace Platform {
namespace {

	enum : size_t {
		PrefetchStride = 4096u
	};

// ---

	static ETInlineHint ETPureFunctionHint DWORD FilePermissionsFromAccessMode( MemoryMappedFile::AccessMode mode ) {
		return mode < MemoryMappedFile::AccessMode::Write ? GENERIC_READ : ( GENERIC_READ | GENERIC_WRITE );
	}

// ---------------------------------------------------

	static ETInlineHint ETPureFunctionHint DWORD ProtectModeFromAccessMode( MemoryMappedFile::AccessMode mode ) {
		return mode < MemoryMappedFile::AccessMode::Write ? PAGE_READONLY : PAGE_READWRITE;
	}

// ---------------------------------------------------

	static ETInlineHint ETPureFunctionHint DWORD MapModeFromAccessMode( MemoryMappedFile::AccessMode mode ) {
	//	Officially, FILE_MAP_WRITE implies FILE_MAP_READ but this is still a little more clear.
		return mode < MemoryMappedFile::AccessMode::Write ? FILE_MAP_READ : (FILE_MAP_READ | FILE_MAP_WRITE);
	}

// ---------------------------------------------------

	static ETInlineHint ETPureFunctionHint Result<MemoryMappedFile> InstantiateMemoryMappedFile( const wchar_t* const path, DWORD creationDisposition, MemoryMappedFile::AccessMode accessMode, uint64 offsetInBytes, size_t mappingSizeSizeInBytes ) {
		ULARGE_INTEGER	mappingSize;
		ULARGE_INTEGER	mappingOffset;

		mappingOffset.QuadPart	= offsetInBytes;
		mappingSize.QuadPart	= mappingSizeSizeInBytes;

	//	Begin by creating a handle to the file.
		const auto fileHandle( CreateFileW( path, FilePermissionsFromAccessMode( accessMode ), FILE_SHARE_READ, nullptr, creationDisposition, FILE_FLAG_POSIX_SEMANTICS, nullptr ) );
		if( INVALID_HANDLE_VALUE == fileHandle ) {
			return Error::Unspecified;
		}

	//	Next, map a range of the file according to the desired size.
		const auto mappingHandle( CreateFileMappingW( fileHandle, nullptr, ProtectModeFromAccessMode( accessMode ), mappingSize.HighPart, mappingSize.LowPart, nullptr ) );
		if( NULL == mappingHandle ) {
			CloseHandle( fileHandle );
			return Error::Unspecified;
		}

		const auto mapping( static_cast<char*>( MapViewOfFile( mappingHandle, MapModeFromAccessMode( accessMode ), mappingOffset.HighPart, mappingOffset.LowPart, mappingSizeSizeInBytes ) ) );
		if( nullptr == mapping ) {
			CloseHandle( mappingHandle );
			CloseHandle( fileHandle );
			return Error::Unspecified;
		}

		return { accessMode, Range<char*>( mapping, mapping + mappingSizeSizeInBytes ) };
	}

}	// anonymous namespace

	MemoryMappedFile::MemoryMappedFile( AccessMode allowedAccess, Range<char*> region ) : _allowedAccess( allowedAccess ), _region( region ) {}

// ---------------------------------------------------

	MemoryMappedFile::MemoryMappedFile( MemoryMappedFile&& other ) : _allowedAccess( other._allowedAccess ), _region( other._region ) {
		other._region = Range<char*>::CreateEmptySet();
	}

// ---------------------------------------------------

	MemoryMappedFile::~MemoryMappedFile() {
		if( _region.IsEmpty() ) {
			return;
		}

		UnmapViewOfFile( _region.Begin() );
	}

// ---------------------------------------------------

	Result<MemoryMappedFile> MemoryMappedFile::ClearOrCreate( const SystemChar* const path, AccessMode accessMode, size_t fileSizeInBytes ) {
		return InstantiateMemoryMappedFile( path, TRUNCATE_EXISTING, accessMode, 0u, fileSizeInBytes );
	}

// ---------------------------------------------------

	Result<MemoryMappedFile> MemoryMappedFile::Open( const SystemChar* const path, AccessMode accessMode, uint64 offsetIntoFileInBytes, size_t mappedSizeInBytes ) {
		return InstantiateMemoryMappedFile( path, OPEN_EXISTING, accessMode, offsetIntoFileInBytes, mappedSizeInBytes );
	}

// ---------------------------------------------------

	bool MemoryMappedFile::HasAccessLevel( AccessMode access ) const {
		return access <= _allowedAccess;
	}

// ---------------------------------------------------

	size_t MemoryMappedFile::GetMappedRegionSizeInBytes() const {
		return _region.GetSize();
	}

// ---------------------------------------------------

	void MemoryMappedFile::PrefetchRange( size_t offsetInBytes, size_t rangeSizeInBytes ) const {
		PrefetchRange( TryGetStructureArrayAtOffset<const char>( offsetInBytes, rangeSizeInBytes ) );
	}

// ---------------------------------------------------

	void MemoryMappedFile::EvictRange( Range<const char*> /*memoryRange*/ ) const {
	//	Unlike POSIX, Windows lacks any mechanism to evict pages from the cache, so this is a no-op.
	}

// ---------------------------------------------------

	void MemoryMappedFile::EvictRange( size_t offsetInBytes, size_t rangeSizeInBytes ) const {
		EvictRange( TryGetStructureArrayAtOffset<const char>( offsetInBytes, rangeSizeInBytes ) );
	}

// ---------------------------------------------------

	void MemoryMappedFile::PrefetchRange( Range<const char*> memoryRange ) const {
#	if( WIN8_MEMORY_MAPPED_FILE_AVAILABLE )
		WIN32_MEMORY_RANGE_ENTRY	ranges[] = { { const_cast<char*>( memoryRange.first ), memoryRange.GetSize() } };

		PrefetchVirtualMemory( GetCurrentProcess(), _countof( ranges ), ranges, 0 );
#	else
		for( auto temp( memoryRange.Begin() ); temp < memoryRange.End(); temp += PrefetchStride ) {
		//	TODO: Is it possible to use prefetch instructions to avoid polluting the processor caches?
			register int readTarget = *reinterpret_cast<const int*>(temp);
		}
#	endif
	}

// ---------------------------------------------------

	void* MemoryMappedFile::GetAddressForFileByteOffset( size_t rawFileOffsetInBytes ) const {
		const auto	result( _region.Begin() + rawFileOffsetInBytes );

		return result < _region.End() ? result : nullptr;
	}

}	// namespace Platform
}	// namespace Eldritch2