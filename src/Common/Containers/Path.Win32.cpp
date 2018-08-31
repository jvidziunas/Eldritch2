/*==================================================================*\
  Path.Win32.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/Path.hpp>
#include <Build.hpp>
//------------------------------------------------------------------//
#include <EASTL/utility.h>
#include <Windows.h>
#include <ShLwApi.h>
#include <ShlObj.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

namespace {

	ETInlineHint Path GetKnownFolderPath(const MallocAllocator& allocator, REFKNOWNFOLDERID folder) {
		wchar_t* path(nullptr);
		if (FAILED(SHGetKnownFolderPath(folder, 0, nullptr, &path))) {
			return Path(allocator);
		}
		ET_AT_SCOPE_EXIT(CoTaskMemFree(path));

		return Path(allocator, AbstractStringView<Path::CharacterType>(path, StringLength(path)));
	}

	// ---------------------------------------------------

	ETInlineHint Path GetUserDocumentsDirectory() {
		Path path(GetKnownFolderPath(MallocAllocator("User Documents Directory Path Allocator"), FOLDERID_Documents));
		path.Append(L"\\" WPROJECT_NAME L"\\");

		return eastl::move(path);
	}

	// ---------------------------------------------------

	ETInlineHint Path GetLocalAppDataDirectory() {
		Path path(GetKnownFolderPath(MallocAllocator("Local App Data Directory Path Allocator"), FOLDERID_LocalAppData));
		path.Append(L"\\" WPROJECT_NAME L"\\");

		return eastl::move(path);
	}

	// ---------------------------------------------------

	ETInlineHint Path GetSharedAppDataDirectory() {
		Path path(GetKnownFolderPath(MallocAllocator("Roaming App Data Directory Path Allocator"), FOLDERID_RoamingAppData));
		path.Append(L"\\" WPROJECT_NAME L"\\");

		return eastl::move(path);
	}

	// ---------------------------------------------------

	ETInlineHint Path GetPackageDirectory() {
		Path path(MallocAllocator("Package Directory Path Allocator"));
		path.Append(L"Content\\");

		return eastl::move(path);
	}

	// ---------------------------------------------------

	ETInlineHint Path GetDownloadedPackagesDirectory() {
		Path path(GetKnownFolderPath(MallocAllocator("Downloaded Packages Directory Path Allocator"), FOLDERID_Documents));
		path.Append(L"\\" WPROJECT_NAME L"\\DownloadedPackages\\");

		return eastl::move(path);
	}

	// ---------------------------------------------------

	ETInlineHint Path GetImageDirectory() {
		Path  path(MallocAllocator(MallocAllocator("Working Directory Path Allocator")));
		DWORD pathLength(GetModuleFileNameW(/*hModule =*/nullptr, /*lpFilename =*/nullptr, /*nSize =*/0u));

		if (pathLength != 0) {
			path.Reserve(pathLength);
			path.ForceSize(pathLength);
			GetModuleFileNameW(/*hModule =*/nullptr, path.GetData(), pathLength);
		}

		return eastl::move(path);
	}

	// ---------------------------------------------------

	ETInlineHint Path GetLogDirectory() {
		Path path(GetKnownFolderPath(MallocAllocator("Log Directory Path Allocator"), FOLDERID_Documents));
		path.Append(L"\\" WPROJECT_NAME L"\\Logs\\");

		return eastl::move(path);
	}

} // anonymous namespace

AbstractStringView<PlatformChar> GetPath(KnownDirectory directory) {
	static const Path UserDocuments(GetUserDocumentsDirectory());
	static const Path AppDataLocal(GetLocalAppDataDirectory());
	static const Path AppDataShared(GetSharedAppDataDirectory());
	static const Path Packages(GetPackageDirectory());
	static const Path DownloadedPackages(GetDownloadedPackagesDirectory());
	static const Path ImageDirectory(GetImageDirectory());
	static const Path Logs(GetLogDirectory());

	switch (directory) {
	case Eldritch2::KnownDirectory::UserDocuments: return UserDocuments; break;
	case Eldritch2::KnownDirectory::AppDataLocal: return AppDataLocal; break;
	case Eldritch2::KnownDirectory::AppDataShared: return AppDataShared; break;
	case Eldritch2::KnownDirectory::Packages: return Packages; break;
	case Eldritch2::KnownDirectory::DownloadedPackages: return DownloadedPackages; break;
	case Eldritch2::KnownDirectory::ImageDirectory: return ImageDirectory; break;
	case Eldritch2::KnownDirectory::Logs: return Logs; break;
	default: return L"";
	}
}

} // namespace Eldritch2
