/*==================================================================*\
  FileSystem.Win32.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/Path.hpp>
#include <Common/FileSystem.hpp>
#include <Common/ErrorCode.hpp>
#include <Common/Memory.hpp>
#include <Build.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

ErrorCode Copy(KnownDirectory destinationRoot, AbstractStringView<PlatformChar> destinationPath, KnownDirectory sourceRoot, AbstractStringView<PlatformChar> sourcePath, CopyMode mode) {
	const Path sourceAbsolutePath(MallocAllocator(), destinationRoot, destinationPath), destinationAbsolutePath(MallocAllocator(), sourceRoot, sourcePath);

	::CopyFileW(sourceAbsolutePath, destinationAbsolutePath, mode == CopyMode::SkipIfExists ? TRUE : FALSE);
	return Error::None;
}

// ---------------------------------------------------

ErrorCode Move(KnownDirectory destinationRoot, AbstractStringView<PlatformChar> destinationPath, KnownDirectory sourceRoot, AbstractStringView<PlatformChar> sourcePath) {
	const Path sourceAbsolutePath(MallocAllocator(), destinationRoot, destinationPath), destinationAbsolutePath(MallocAllocator(), sourceRoot, sourcePath);

	::MoveFileExW(sourceAbsolutePath, destinationAbsolutePath, MOVEFILE_REPLACE_EXISTING);
	return Error::None;
}

// ---------------------------------------------------

ErrorCode Delete(KnownDirectory root, AbstractStringView<PlatformChar> path) {
	const Path absolutePath(MallocAllocator(), root, path);

	::DeleteFileW(absolutePath);
	return Error::None;
}

// ---------------------------------------------------

ErrorCode EnsureDirectoryExists(KnownDirectory root, AbstractStringView<PlatformChar> path) {
	const Path absolutePath(MallocAllocator(), root, path);

	::CreateDirectoryW(absolutePath, nullptr);
	return Error::None;
}

// ---------------------------------------------------

ErrorCode ForEachFile(AbstractStringView<PlatformChar> specifier, Function<void(AbstractStringView<PlatformChar> /*path*/) ETNoexceptHint> handler) {
	::WIN32_FIND_DATAW result;
	PlatformChar*      terminated(ETStackAlloc(PlatformChar, specifier.GetLength() + 1u));

	CopyAndTerminate(specifier.Begin(), specifier.End(), terminated, L'\0');

	const auto find(::FindFirstFileExW(terminated, FindExInfoBasic, &result, FindExSearchNameMatch, nullptr, FIND_FIRST_EX_CASE_SENSITIVE | FIND_FIRST_EX_LARGE_FETCH));
	if (ET_UNLIKELY(INVALID_HANDLE_VALUE == find)) {
		return Error::Unspecified;
	}

	ET_AT_SCOPE_EXIT(::FindClose(find));
	do {
		handler({ result.cFileName, StringLength(result.cFileName) });
	} while (::FindNextFileW(find, &result) != 0);

	return ::GetLastError() == ERROR_NO_MORE_FILES ? Error::None : Error::Unspecified;
}

} // namespace Eldritch2
