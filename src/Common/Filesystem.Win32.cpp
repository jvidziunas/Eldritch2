/*==================================================================*\
  FileSystem.Win32.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Build.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

Result Copy(KnownDirectory destinationRoot, AbstractStringSpan<PlatformChar> destinationPath, KnownDirectory sourceRoot, AbstractStringSpan<PlatformChar> sourcePath, FileCopyMode mode) {
	const Path sourceAbsolutePath(MallocAllocator(), destinationRoot, destinationPath), destinationAbsolutePath(MallocAllocator(), sourceRoot, sourcePath);

	::CopyFileW(sourceAbsolutePath, destinationAbsolutePath, mode == FileCopyMode::SkipIfExists ? TRUE : FALSE);
	return Result::Success;
}

// ---------------------------------------------------

Result Move(KnownDirectory destinationRoot, AbstractStringSpan<PlatformChar> destinationPath, KnownDirectory sourceRoot, AbstractStringSpan<PlatformChar> sourcePath) {
	const Path sourceAbsolutePath(MallocAllocator(), destinationRoot, destinationPath), destinationAbsolutePath(MallocAllocator(), sourceRoot, sourcePath);

	::MoveFileExW(sourceAbsolutePath, destinationAbsolutePath, MOVEFILE_REPLACE_EXISTING);
	return Result::Success;
}

// ---------------------------------------------------

Result Delete(KnownDirectory root, AbstractStringSpan<PlatformChar> path) {
	const Path absolutePath(MallocAllocator(), root, path);

	::DeleteFileW(absolutePath);
	return Result::Success;
}

// ---------------------------------------------------

Result EnsureDirectoryExists(KnownDirectory root, AbstractStringSpan<PlatformChar> path) {
	const Path absolutePath(MallocAllocator(), root, path);

	::CreateDirectoryW(absolutePath, /*lpSecurityAttributes =*/nullptr);
	return Result::Success;
}

// ---------------------------------------------------

Result ForEachFile(AbstractStringSpan<PlatformChar> specifier, Function<void(AbstractStringSpan<PlatformChar> /*path*/)> handler) {
	PlatformChar* terminatedSpec(ETStackAlloc(PlatformChar, specifier.GetLength() + 1u));
	CopyAndTerminate(specifier.Begin(), specifier.End(), terminatedSpec, SL('\0'));

	WIN32_FIND_DATAW result;
	const auto       find(FindFirstFileExW(terminatedSpec, FindExInfoBasic, ETAddressOf(result), FindExSearchNameMatch, nullptr, FIND_FIRST_EX_CASE_SENSITIVE | FIND_FIRST_EX_LARGE_FETCH));
	ET_ABORT_UNLESS(find != INVALID_HANDLE_VALUE ? Result::Success : Result::Unspecified);
	ET_AT_SCOPE_EXIT(FindClose(find));

	do {
		handler(PlatformStringSpan(result.cFileName, PlatformStringSpan::SizeType(StringLength(result.cFileName))));
	} while (FindNextFileW(find, ETAddressOf(result)) != 0);

	return GetLastError() == ERROR_NO_MORE_FILES ? Result::Success : Result::Unspecified;
}

} // namespace Eldritch2
