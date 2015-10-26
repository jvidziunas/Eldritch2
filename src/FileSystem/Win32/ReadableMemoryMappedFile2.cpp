/*==================================================================*\
  ReadableMemoryMappedFile2.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <FileSystem/Win32/ReadableMemoryMappedFile2.hpp>
//------------------------------------------------------------------//

#if defined( _WIN32_WINNT_WIN8 ) && ( _WIN32_WINNT >= _WIN32_WINNT_WIN8 )
#	define WIN8_MEMORY_MAPPED_FILE_AVAILABLE 1
#else
#	if( ET_COMPILER_IS_MSVC )
#	define WIN8_MEMORY_MAPPED_FILE_AVAILABLE 0
	COMPILERMESSAGEGENERIC( "Windows 8 PrefetchVirtualMemory-optimized file mapping object unavailable, use the Windows 8 SDK or newer to enable" )
#	endif
#endif

using namespace Eldritch2;

namespace Eldritch2 {
namespace FileSystem {
namespace Win32 {

	ReadableMemoryMappedFile2::ReadableMemoryMappedFile2( const Range<const char*> mappedRegion ) : ReadableMemoryMappedFile( mappedRegion ) {}

// ---------------------------------------------------

	void ReadableMemoryMappedFile2::PrefetchRangeForRead( const Range<const char*> memoryRange ) const {
#if( WIN8_MEMORY_MAPPED_FILE_AVAILABLE )
		::WIN32_MEMORY_RANGE_ENTRY	ranges[] = { { const_cast<char*>(memoryRange.first), memoryRange.Size() } };

		::PrefetchVirtualMemory( ::GetCurrentProcess(), _countof( ranges ), ranges, 0 );
#else
		ReadableMemoryMappedFile::PrefetchRangeForRead( memoryRange );
#endif
	}

}	// namespace Win32
}	// namespace FileSystem
}	// namespace Eldritch2

