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
#include <Common/Containers/AbstractStringView.hpp>
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

ErrorCode ForEachFile(AbstractStringView<PlatformChar> specifier, Function<void(AbstractStringView<PlatformChar> /*path*/) ETNoexceptHint> handler);

ErrorCode Copy(KnownDirectory destinationDirectory, AbstractStringView<PlatformChar> destinationPath, KnownDirectory sourceDirectory, AbstractStringView<PlatformChar> sourcePath, CopyMode mode);
ErrorCode Copy(KnownDirectory directory, AbstractStringView<PlatformChar> destinationPath, AbstractStringView<PlatformChar> sourcePath, CopyMode mode);

ErrorCode Move(KnownDirectory destinationDirectory, AbstractStringView<PlatformChar> destinationPath, KnownDirectory sourceDirectory, AbstractStringView<PlatformChar> sourcePath);
ErrorCode Move(KnownDirectory directory, AbstractStringView<PlatformChar> destinationPath, AbstractStringView<PlatformChar> sourcePath);

ErrorCode EnsureDirectoryExists(KnownDirectory root, AbstractStringView<PlatformChar> path = L"");

ErrorCode Delete(KnownDirectory directory, AbstractStringView<PlatformChar> path);

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/FileSystem.inl>
//------------------------------------------------------------------//
