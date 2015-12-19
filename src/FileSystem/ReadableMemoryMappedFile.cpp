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

	void ReadableMemoryMappedFile::PrefetchRangeForRead( const size_t offsetInBytes, const size_t rangeSizeInBytes ) const {
		const auto	begin( static_cast<const char*>(GetAddressForFileByteOffset( offsetInBytes )) );

		PrefetchRangeForRead( { begin, begin + rangeSizeInBytes } );
	}

}	// namespace FileSystem
}	// namespace Eldritch2