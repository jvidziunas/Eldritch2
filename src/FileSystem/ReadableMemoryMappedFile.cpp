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
#include <FileSystem/ReadableMemoryMappedFile.hpp>
#include <Utility/Containers/Range.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace FileSystem {

	void ReadableMemoryMappedFile::PrefetchRange( const size_t offsetInBytes, const size_t rangeSizeInBytes ) const {
		PrefetchRange( TryGetStructureArrayAtOffset<char>( offsetInBytes, rangeSizeInBytes ) );
	}

// ---------------------------------------------------

	void ReadableMemoryMappedFile::EvictRange( const ::Eldritch2::Range<const char*> /*memoryRange*/ ) const {}

// ---------------------------------------------------

	void ReadableMemoryMappedFile::EvictRange( const size_t offsetInBytes, const size_t rangeSizeInBytes ) const {
		EvictRange( TryGetStructureArrayAtOffset<char>( offsetInBytes, rangeSizeInBytes ) );
	}

}	// namespace FileSystem
}	// namespace Eldritch2