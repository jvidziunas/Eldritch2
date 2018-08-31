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

ETInlineHint ETForceInlineHint ErrorCode Copy(KnownDirectory directory, AbstractStringView<PlatformChar> destinationFileName, AbstractStringView<PlatformChar> sourceFileName, CopyMode mode) {
	return Copy(directory, destinationFileName, directory, sourceFileName, mode);
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ErrorCode Move(KnownDirectory directory, AbstractStringView<PlatformChar> destinationFileName, AbstractStringView<PlatformChar> sourceFileName) {
	return Move(directory, destinationFileName, directory, sourceFileName);
}

} // namespace Eldritch2
