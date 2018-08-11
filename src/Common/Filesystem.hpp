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
#include <Common/Containers/AbstractString.hpp>
#include <Common/Mpl/IntTypes.hpp>
#include <Common/Function.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
enum class KnownDirectory : uint32;
class ErrorCode;
} // namespace Eldritch2

namespace Eldritch2 {

enum class CopyMode : uint32 {
	SkipIfExists,
	OverwriteIfExists
};

ErrorCode ForEachFile(StringView<PlatformChar> specifier, Function<void(StringView<PlatformChar> /*path*/)> handler);

void Copy(KnownDirectory destinationDirectory, StringView<PlatformChar> destinationPath, KnownDirectory sourceDirectory, StringView<PlatformChar> sourcePath, CopyMode mode);
void Copy(KnownDirectory directory, StringView<PlatformChar> destinationPath, StringView<PlatformChar> sourcePath, CopyMode mode);

void Move(KnownDirectory destinationDirectory, StringView<PlatformChar> destinationPath, KnownDirectory sourceDirectory, StringView<PlatformChar> sourcePath);
void Move(KnownDirectory directory, StringView<PlatformChar> destinationPath, StringView<PlatformChar> sourcePath);

void Delete(KnownDirectory directory, StringView<PlatformChar> path);

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/FileSystem.inl>
//------------------------------------------------------------------//
