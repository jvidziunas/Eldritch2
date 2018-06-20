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
#include <Common/UniquePointer.hpp>
#include <Common/FileSystem.hpp>
#include <Common/ErrorCode.hpp>
#include <Common/Memory.hpp>
#include <Build.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
#include <ShLwApi.h>
#include <ShlObj.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace {

	class TaskMemFreeDeleter {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref TaskMemFreeDeleter instance.
		ETInlineHint TaskMemFreeDeleter(size_t lengthInBytes) :
			_lengthInBytes(lengthInBytes) {}
		//!	Constructs this @ref TaskMemFreeDeleter instance.
		TaskMemFreeDeleter() = default;

		~TaskMemFreeDeleter() = default;

		// ---------------------------------------------------

		ETInlineHint ETPureFunctionHint void operator()(void* const data) {
			CoTaskMemFree(data);
		}

		// ---------------------------------------------------

		ETInlineHint size_t GetSize() const {
			return _lengthInBytes;
		}

		// - DATA MEMBERS ------------------------------------

	private:
		size_t _lengthInBytes;
	};

	// ---

	template <size_t bufferSize>
	static ETInlineHint int WideCharFromUtf8(wchar_t (&out)[bufferSize], const Utf8Char* const source, int sourceLength = -1) {
		return MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, source, sourceLength, out, _countof(out));
	}

	// ---------------------------------------------------

	template <size_t bufferSize>
	static ETInlineHint int Utf8FromWideChar(Utf8Char (&out)[bufferSize], const wchar_t* const source, int sourceLength = -1) {
		return WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, source, sourceLength, out, _countof(out), NULL, NULL);
	}

	// ---------------------------------------------------

	static ETInlineHint void EnsureDirectoryExists(const PlatformString<>& path) {
		CreateDirectoryW(path, nullptr);
	}

	// ---------------------------------------------------

	static ETPureFunctionHint UniquePointer<PlatformChar[], TaskMemFreeDeleter> GetKnownFolderPath(REFKNOWNFOLDERID folderId) {
		wchar_t* path(nullptr);

		if (FAILED(SHGetKnownFolderPath(folderId, 0, nullptr, &path))) {
			return nullptr;
		}

		return { path, TaskMemFreeDeleter(StringLength(path)) };
	}

	// ---------------------------------------------------

	static ETPureFunctionHint PlatformString<> GetUserDocumentsDirectory() {
		//	Retrieve the Documents library path for this machine.
		PlatformString<> path(MallocAllocator("User Documents Directory Path Allocator"));
		const auto       base(GetKnownFolderPath(FOLDERID_Documents));

		path.Append(base.Begin(), base.End()).Append(L"/My Games/" WPROJECT_NAME L"/");

		//	Ensure the directory exists.
		EnsureDirectoryExists(path);

		return eastl::move(path);
	}

	// ---------------------------------------------------

	static ETPureFunctionHint PlatformString<> GetLocalAppDataDirectory() {
		//	Retrieve the AppDataLocal library path for this machine.
		PlatformString<> path(MallocAllocator("Local App Data Directory Path Allocator"));
		const auto       base(GetKnownFolderPath(FOLDERID_LocalAppData));

		path.Append(base.Begin(), base.End()).Append(L"/" WPROJECT_NAME L"/");

		//	Ensure the directory exists.
		EnsureDirectoryExists(path);

		return eastl::move(path);
	}

	// ---------------------------------------------------

	static ETPureFunctionHint PlatformString<> GetSharedAppDataDirectory() {
		//	Retrieve the AppDataRoaming library path for this machine.
		PlatformString<> path(MallocAllocator("Local App Data Directory Path Allocator"));
		const auto       base(GetKnownFolderPath(FOLDERID_RoamingAppData));

		path.Append(base.Begin(), base.End()).Append(L"/" WPROJECT_NAME L"/");

		//	Ensure the directory exists.
		EnsureDirectoryExists(path);

		return eastl::move(path);
	}

	// ---------------------------------------------------

	static ETPureFunctionHint PlatformString<> GetWorkingDirectory() {
		PlatformChar path[MAX_PATH];
		auto         pathLength(GetCurrentDirectoryW(static_cast<DWORD>(_countof(path)), path));

		if (pathLength == 0) {
			return PlatformString<>(MallocAllocator("Working Directory Path Allocator"));
		}

		//	Ensure the path ends with a directory separator.
		if (path[pathLength - 1] != L'\\') {
			AppendString(path, L"\\");
			++pathLength;
		}

		return PlatformString<>(path, path + pathLength, MallocAllocator("Working Directory Path Allocator"));
	}

	// ---------------------------------------------------

	static ETPureFunctionHint PlatformString<> GetPackageDirectory(const PlatformString<>& workingDirectoryPath) {
		PlatformString<> path(workingDirectoryPath, MallocAllocator("Package Directory Path Allocator"));

		path.Append(L"Content/");

		return eastl::move(path);
	}

	// ---------------------------------------------------

	static ETPureFunctionHint PlatformString<> GetDownloadedPackagesDirectoryPath(const PlatformString<>& documentsPath) {
		PlatformString<> path(documentsPath, MallocAllocator("Downloaded Packages Directory Path Allocator"));

		path.Append(L"DownloadedPackages/");

		//	Ensure the directory exists.
		EnsureDirectoryExists(path);

		return eastl::move(path);
	}

	// ---------------------------------------------------

	static ETPureFunctionHint PlatformString<> GetLogDirectory(const PlatformString<>& documentsPath) {
		PlatformString<> path(documentsPath, MallocAllocator("Log Directory Path Allocator"));

		path.Append(L"Logs/");

		//	Ensure the directory exists.
		EnsureDirectoryExists(path);

		return eastl::move(path);
	}

} // anonymous namespace

FileSystem::FileSystem() :
	_workingDirectory(GetWorkingDirectory()),
	_userDocumentsDirectory(GetUserDocumentsDirectory()),
	_localAppDataDirectory(GetLocalAppDataDirectory()),
	_sharedAppDataDirectory(GetSharedAppDataDirectory()),
	_packageDirectory(GetPackageDirectory(_workingDirectory)),
	_downloadedPackageDirectory(GetDownloadedPackagesDirectoryPath(_userDocumentsDirectory)),
	_logDirectory(GetLogDirectory(_userDocumentsDirectory)) {
}

// ---------------------------------------------------

void FileSystem::Copy(
	KnownDirectory        destination,
	const Utf8Char* const destinationPath,
	KnownDirectory        source,
	const Utf8Char* const sourcePath,
	OverwriteBehavior     overwriteBehavior) {
	const auto sourceAbsolutePath(GetAbsolutePath(source, sourcePath));
	const auto destinationAbsolutePath(GetAbsolutePath(destination, destinationPath));

	CopyFileW(sourceAbsolutePath, destinationAbsolutePath, OverwriteBehavior::SkipIfExists == overwriteBehavior ? TRUE : FALSE);
}

// ---------------------------------------------------

void FileSystem::Move(
	KnownDirectory        destination,
	const Utf8Char* const destinationPath,
	KnownDirectory        source,
	const Utf8Char* const sourcePath) {
	const auto sourceAbsolutePath(GetAbsolutePath(source, sourcePath));
	const auto destinationAbsolutePath(GetAbsolutePath(destination, destinationPath));

	MoveFileExW(sourceAbsolutePath, destinationAbsolutePath, MOVEFILE_REPLACE_EXISTING);
}

// ---------------------------------------------------

void FileSystem::Delete(KnownDirectory directory, const Utf8Char* const path) {
	const auto absolutePath(GetAbsolutePath(directory, path));

	DeleteFileW(absolutePath);
}

// ---------------------------------------------------

ErrorCode FileSystem::EnumerateMatchingFiles(KnownDirectory directory, const Utf8Char* const path, Function<void(const Utf8Char* /*packagePath*/)> handler) const {
	WIN32_FIND_DATAW findData;
	Utf8Char         utf8Path[MAX_PATH];
	const auto       absolutePathSpecifier(GetAbsolutePath(directory, path));
	const auto       findHandle(
        FindFirstFileExW(
            absolutePathSpecifier,
            FindExInfoBasic,
            &findData,
            FindExSearchNameMatch,
            nullptr,
            FIND_FIRST_EX_CASE_SENSITIVE | FIND_FIRST_EX_LARGE_FETCH));

	if (INVALID_HANDLE_VALUE == findHandle) {
		return Error::Unspecified;
	}

	do {
		if (Utf8FromWideChar(utf8Path, findData.cFileName) == 0) {
			break;
		}

		handler(utf8Path);
	} while (FindNextFileW(findHandle, &findData) != 0);

	const auto findError(GetLastError());

	FindClose(findHandle);

	return findError == ERROR_NO_MORE_FILES ? Error::None : Error::Unspecified;
}

// ---------------------------------------------------

ETPureFunctionHint PlatformString<> FileSystem::GetAbsolutePath(KnownDirectory directory, const Utf8Char* path) const {
	PlatformString<> absolutePath(GetPath(directory), MallocAllocator("Path Allocator"));
	PlatformChar     widePath[MAX_PATH];

	if (WideCharFromUtf8(widePath, path) != 0) {
		absolutePath.Append(widePath);
	}

	return eastl::move(absolutePath);
}

} // namespace Eldritch2
