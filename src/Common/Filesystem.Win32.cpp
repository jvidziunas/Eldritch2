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

	static ETInlineHint void EnsureDirectoryExists(const Path<>& path) {
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

	static ETPureFunctionHint Path<> GetUserDocumentsDirectory() {
		Path<>     directory(MallocAllocator("User Documents Directory Path Allocator"));
		const auto base(GetKnownFolderPath(FOLDERID_Documents));

		directory.Append(base.Begin(), base.End()).Append(L"/My Games/" WPROJECT_NAME L"/");

		//	Ensure the directory exists.
		EnsureDirectoryExists(directory);

		return eastl::move(directory);
	}

	// ---------------------------------------------------

	static ETPureFunctionHint Path<> GetLocalAppDataDirectory() {
		Path<>     directory(MallocAllocator("Local App Data Directory Path Allocator"));
		const auto base(GetKnownFolderPath(FOLDERID_LocalAppData));

		directory.Append(base.Begin(), base.End()).Append(L"/" WPROJECT_NAME L"/");

		EnsureDirectoryExists(directory);

		return eastl::move(directory);
	}

	// ---------------------------------------------------

	static ETPureFunctionHint Path<> GetSharedAppDataDirectory() {
		Path<>     directory(MallocAllocator("Local App Data Directory Path Allocator"));
		const auto base(GetKnownFolderPath(FOLDERID_RoamingAppData));

		directory.Append(base.Begin(), base.End()).Append(L"/" WPROJECT_NAME L"/");

		EnsureDirectoryExists(directory);

		return eastl::move(directory);
	}

	// ---------------------------------------------------

	static ETPureFunctionHint Path<> GetWorkingDirectory() {
		PlatformChar path[MAX_PATH];
		auto         pathLength(GetCurrentDirectoryW(DWORD(_countof(path)), path));

		if (pathLength == 0) {
			return Path<>(MallocAllocator("Working Directory Path Allocator"));
		}

		//	Ensure the path ends with a directory separator.
		if (path[pathLength - 1] != L'\\') {
			AppendString(path, L"\\");
			++pathLength;
		}

		return Path<>(path, path + pathLength, MallocAllocator("Working Directory Path Allocator"));
	}

	// ---------------------------------------------------

	static ETPureFunctionHint Path<> GetPackageDirectory(const Path<>& workingDirectory) {
		Path<> path(workingDirectory, MallocAllocator("Package Directory Path Allocator"));
		path.Append(L"Content/");

		return eastl::move(path);
	}

	// ---------------------------------------------------

	static ETPureFunctionHint Path<> GetDownloadedPackagesDirectoryPath(const Path<>& documentsDirectory) {
		Path<> path(documentsDirectory, MallocAllocator("Downloaded Packages Directory Path Allocator"));
		path.Append(L"DownloadedPackages/");

		//	Ensure the directory exists.
		EnsureDirectoryExists(path);

		return eastl::move(path);
	}

	// ---------------------------------------------------

	static ETPureFunctionHint Path<> GetLogDirectory(const Path<>& documentsDirectory) {
		Path<> path(documentsDirectory, MallocAllocator("Log Directory Path Allocator"));

		path.Append(L"Logs/");

		//	Ensure the directory exists.
		EnsureDirectoryExists(path);

		return eastl::move(path);
	}

} // anonymous namespace

void Copy(KnownDirectory destination, StringView<PlatformChar> destinationPath, KnownDirectory source, StringView<PlatformChar> sourcePath, CopyMode mode) {
	Path<> sourceAbsolutePath, destinationAbsolutePath;

	sourceAbsolutePath.Assign(destination, destinationPath);
	destinationAbsolutePath.Assign(source, sourcePath);

	CopyFileW(sourceAbsolutePath, destinationAbsolutePath, mode == CopyMode::SkipIfExists ? TRUE : FALSE);
}

// ---------------------------------------------------

void Move(KnownDirectory destination, StringView<PlatformChar> destinationPath, KnownDirectory source, StringView<PlatformChar> sourcePath) {
	Path<> sourceAbsolutePath, destinationAbsolutePath;

	sourceAbsolutePath.Assign(source, sourcePath);
	destinationAbsolutePath.Assign(destination, destinationPath);

	MoveFileExW(sourceAbsolutePath, destinationAbsolutePath, MOVEFILE_REPLACE_EXISTING);
}

// ---------------------------------------------------

void Delete(KnownDirectory directory, StringView<PlatformChar> path) {
	Path<> absolutePath;

	absolutePath.Assign(directory, path);

	DeleteFileW(absolutePath);
}

// ---------------------------------------------------

ErrorCode ForEachFile(StringView<PlatformChar> specifier, Function<void(StringView<PlatformChar> /*path*/)> handler) {
	WIN32_FIND_DATAW result;
	PlatformChar*    terminated(ETStackAlloc(PlatformChar, specifier.GetLength() + 1u));

	CopyAndTerminate(specifier.Begin(), specifier.End(), terminated, L'\0');

	const auto find(FindFirstFileExW(terminated, FindExInfoBasic, &result, FindExSearchNameMatch, nullptr, FIND_FIRST_EX_CASE_SENSITIVE | FIND_FIRST_EX_LARGE_FETCH));
	if (ET_UNLIKELY(INVALID_HANDLE_VALUE == find)) {
		return Error::Unspecified;
	}

	ET_AT_SCOPE_EXIT(FindClose(find));
	do {
		handler(StringView<PlatformChar>(result.cFileName, FindTerminator(result.cFileName)));
	} while (FindNextFileW(find, &result) != 0);

	return GetLastError() == ERROR_NO_MORE_FILES ? Error::None : Error::Unspecified;
}

} // namespace Eldritch2
