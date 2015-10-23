/*==================================================================*\
  ContentProvider.inl
  ------------------------------------------------------------------
  Purpose:
  Windows implementation for the content provider interface. This
  object translates high-level calls to read/serialize content from
  the hard disk into low-level API commands for paging file contents
  into the running process' address space.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace FileSystem {
namespace Win32 {

	ETInlineHint const wchar_t* ContentProvider::PathFromKnownContentLocation( const KnownContentLocation location ) const {
		ETRuntimeAssert( location < KnownContentLocation::COUNT );

	// ---

		return _knownContentPaths[static_cast<size_t>(location)];
	}

}	// namespace Win32
}	// namespace Foundation
}	// namespace Eldritch2