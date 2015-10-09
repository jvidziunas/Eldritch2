/*==================================================================*\
  Win32ContentProvider.inl
  ------------------------------------------------------------------
  Purpose:
  Windows implementation for the content provider interface. This
  object translates high-level calls to read/serialize content from
  the hard disk into low-level API commands for paging file contents
  into the running process' address space.

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace FileSystem {

	namespace FileSystem	= ::Eldritch2::FileSystem;

// ---------------------------------------------------

	ETInlineHint const wchar_t* Win32ContentProvider::PathFromKnownContentLocation( ContentProvider::KnownContentLocation location ) const {
		ETRuntimeAssert( location < KnownContentLocation::COUNT );

		return _knownContentPaths[static_cast<size_t>(location)];
	}

}	// namespace Foundation
}	// namespace Eldritch2