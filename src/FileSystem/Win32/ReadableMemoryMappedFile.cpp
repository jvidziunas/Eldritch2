/*==================================================================*\
  ReadableMemoryMappedFile.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <FileSystem/Win32/ReadableMemoryMappedFile.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace FileSystem {
namespace Win32 {

	ReadableMemoryMappedFile::ReadableMemoryMappedFile( const Range<const char*> mappedRegion ) : _region( mappedRegion ) {}

// ---------------------------------------------------

	ReadableMemoryMappedFile::~ReadableMemoryMappedFile() {
		::UnmapViewOfFile( _region.first );
	}

// ---------------------------------------------------

	size_t ReadableMemoryMappedFile::GetAccessibleRegionSizeInBytes() const {
		return _region.Size();
	}

// ---------------------------------------------------

	void ReadableMemoryMappedFile::PrefetchRangeForRead( const Range<const char*> memoryRange ) const {
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

	const void* ReadableMemoryMappedFile::GetAddressForFileByteOffset( const size_t rawFileOffsetInBytes ) const {
		return _region.first + rawFileOffsetInBytes;
	}

}	// namespace Win32
}	// namespace FileSystem
}	// namespace Eldritch2