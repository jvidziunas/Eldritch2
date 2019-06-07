/*==================================================================*\
  FileSystem.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/Path.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

ETInlineHint ETForceInlineHint Result Copy(KnownDirectory directory, AbstractStringSpan<PlatformChar> destinationPath, AbstractStringSpan<PlatformChar> sourcePath, FileCopyMode mode) {
	return Copy(directory, destinationPath, directory, sourcePath, mode);
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint Result Move(KnownDirectory directory, AbstractStringSpan<PlatformChar> destinationPath, AbstractStringSpan<PlatformChar> sourcePath) {
	return Move(directory, destinationPath, directory, sourcePath);
}

} // namespace Eldritch2
