/*==================================================================*\
  Path.Win32.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/Path.hpp>
#include <Build.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
#include <ShLwApi.h>
#include <ShlObj.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

namespace {

	ETForceInlineHint Path GetKnownFolderPath(const MallocAllocator& allocator, REFKNOWNFOLDERID folder) {
		using PathSpan = AbstractStringSpan<Path::CharacterType>;

		wchar_t* path(nullptr);
		if (FAILED(SHGetKnownFolderPath(folder, 0, nullptr, &path))) {
			return Path(allocator);
		}
		ET_AT_SCOPE_EXIT(CoTaskMemFree(path));

		return Path(allocator, KnownDirectory::Relative, PathSpan(path, PathSpan::SizeType(StringLength(path))));
	}

	// ---------------------------------------------------

	ETForceInlineHint Path GetUserDocumentsDirectory() {
		Path path(GetKnownFolderPath(Path::AllocatorType("User Documents Directory Path Allocator"), FOLDERID_Documents));
		path.Append(SL("\\") SL(PROJECT_NAME) SL("\\"));

		return Move(path);
	}

	// ---------------------------------------------------

	ETForceInlineHint Path GetLocalAppDataDirectory() {
		Path path(GetKnownFolderPath(Path::AllocatorType("Local App Data Directory Path Allocator"), FOLDERID_LocalAppData));
		path.Append(SL("\\") SL(PROJECT_NAME) SL("\\"));

		return Move(path);
	}

	// ---------------------------------------------------

	ETForceInlineHint Path GetSharedAppDataDirectory() {
		Path path(GetKnownFolderPath(Path::AllocatorType("Roaming App Data Directory Path Allocator"), FOLDERID_RoamingAppData));
		path.Append(SL("\\") SL(PROJECT_NAME) SL("\\"));

		return Move(path);
	}

	// ---------------------------------------------------

	ETForceInlineHint Path GetPackageDirectory() {
		Path path(Path::AllocatorType("Package Directory Path Allocator"));
		path.Append(SL("Content\\"));

		return Move(path);
	}

	// ---------------------------------------------------

	ETForceInlineHint Path GetDownloadedPackagesDirectory() {
		Path path(GetKnownFolderPath(Path::AllocatorType("Downloaded Packages Directory Path Allocator"), FOLDERID_Documents));
		path.Append(SL("\\") SL(PROJECT_NAME) SL("\\DownloadedPackages\\"));

		return Move(path);
	}

	// ---------------------------------------------------

	ETForceInlineHint Path GetImageDirectory() {
		Path  path(Path::AllocatorType("Working Directory Path Allocator"));
		DWORD pathLength(GetModuleFileNameW(/*hModule =*/nullptr, /*lpFilename =*/nullptr, /*nSize =*/0u));

		if (pathLength != 0) {
			path.Reserve(pathLength);
			path.ForceSize(pathLength);
			GetModuleFileNameW(/*hModule =*/nullptr, path.GetData(), pathLength);
		}

		return Move(path);
	}

	// ---------------------------------------------------

	ETForceInlineHint Path GetLogDirectory() {
		Path path(GetKnownFolderPath(Path::AllocatorType("Log Directory Path Allocator"), FOLDERID_Documents));
		path.Append(SL("\\") SL(PROJECT_NAME) SL("\\Logs\\"));

		return Move(path);
	}

} // anonymous namespace

AbstractStringSpan<PlatformChar> GetPath(KnownDirectory directory) {
	static const Path UserDocuments(GetUserDocumentsDirectory());
	static const Path AppDataLocal(GetLocalAppDataDirectory());
	static const Path AppDataShared(GetSharedAppDataDirectory());
	static const Path Packages(GetPackageDirectory());
	static const Path DownloadedPackages(GetDownloadedPackagesDirectory());
	static const Path ImageDirectory(GetImageDirectory());
	static const Path Logs(GetLogDirectory());

	switch (directory) {
	case KnownDirectory::UserDocuments: return UserDocuments; break;
	case KnownDirectory::AppDataLocal: return AppDataLocal; break;
	case KnownDirectory::AppDataShared: return AppDataShared; break;
	case KnownDirectory::Packages: return Packages; break;
	case KnownDirectory::DownloadedPackages: return DownloadedPackages; break;
	case KnownDirectory::ImageDirectory: return ImageDirectory; break;
	case KnownDirectory::Logs: return Logs; break;
	default: return AbstractStringSpan<PlatformChar>();
	}
}

} // namespace Eldritch2
