/*==================================================================*\
  FileSystem.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/AbstractStringSpan.hpp>
#include <Common/Mpl/IntTypes.hpp>
#include <Common/Function.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

enum class KnownDirectory : unsigned int;
enum class Result : int;

} // namespace Eldritch2

namespace Eldritch2 {

enum class FileCopyMode : bool {
	SkipIfExists,
	OverwriteIfExists
};

Result ForEachFile(AbstractStringSpan<PlatformChar> specifier, Function<void(AbstractStringSpan<PlatformChar> /*path*/)> handler);

Result Copy(KnownDirectory destinationDirectory, AbstractStringSpan<PlatformChar> destinationPath, KnownDirectory sourceDirectory, AbstractStringSpan<PlatformChar> sourcePath, FileCopyMode);
Result Copy(KnownDirectory directory, AbstractStringSpan<PlatformChar> destinationPath, AbstractStringSpan<PlatformChar> sourcePath, FileCopyMode);

Result Move(KnownDirectory destinationDirectory, AbstractStringSpan<PlatformChar> destinationPath, KnownDirectory sourceDirectory, AbstractStringSpan<PlatformChar> sourcePath);
Result Move(KnownDirectory directory, AbstractStringSpan<PlatformChar> destinationPath, AbstractStringSpan<PlatformChar> sourcePath);

Result EnsureDirectoryExists(KnownDirectory root, AbstractStringSpan<PlatformChar> path = SL(""));

Result Delete(KnownDirectory directory, AbstractStringSpan<PlatformChar> path);

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/FileSystem.inl>
//------------------------------------------------------------------//
