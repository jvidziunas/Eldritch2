/*==================================================================*\
  FileView.Win32.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Windows.h>
//------------------------------------------------------------------//

#if defined(_WIN32_WINNT_WIN8) && (_WIN32_WINNT >= _WIN32_WINNT_WIN8)
#	define PREFETCHVIRTUALMEMORY_AVAILABLE 1
#else // !defined(_WIN32_WINNT_WIN8) || (_WIN32_WINNT < _WIN32_WINNT_WIN8)
#	define PREFETCHVIRTUALMEMORY_AVAILABLE 0
#endif // defined(_WIN32_WINNT_WIN8) && (_WIN32_WINNT >= _WIN32_WINNT_WIN8)

namespace Eldritch2 {

namespace {

	ETConstexpr ETForceInlineHint ETPureFunctionHint DWORD PermissionsByAccess(FileView::FileAccess access) ETNoexceptHint {
		return access < FileView::FileAccess::Write ? GENERIC_READ : (GENERIC_READ | GENERIC_WRITE);
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint ETPureFunctionHint DWORD ProtectionFromAccess(FileView::FileAccess access) ETNoexceptHint {
		return access < FileView::FileAccess::Write ? PAGE_READONLY : PAGE_READWRITE;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint ETPureFunctionHint DWORD ModeFromAccess(FileView::FileAccess access) ETNoexceptHint {
		//	Officially, FILE_MAP_WRITE implies FILE_MAP_READ but this is still a little more clear.
		return access < FileView::FileAccess::Write ? FILE_MAP_READ : (FILE_MAP_READ | FILE_MAP_WRITE);
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint ETPureFunctionHint uint64 PackUInt64(DWORD low, DWORD high) ETNoexceptHint {
		return ULARGE_INTEGER{ /*LowPart =*/low, /*HighPart =*/high }.QuadPart;
	}

	// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint Tuple<DWORD, DWORD> UnpackInt64(int64 value) ETNoexceptHint {
		ULARGE_INTEGER temp;
		temp.QuadPart = value;

		return MakeTuple(temp.HighPart, temp.LowPart);
	}

	// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint Result MapFile(Span<byte*>& outView, HANDLE file, FileView::FileAccess access, uint64 byteOffset, size_t byteSize) ETNoexceptHint {
		// Fill in the mapping size from the base file length.
		if (byteSize == FileView::EndOfFile) {
			BY_HANDLE_FILE_INFORMATION fileInfo;
			ET_ABORT_UNLESS((GetFileInformationByHandle(file, ETAddressOf(fileInfo)) != 0) ? Result::Success : Result::Unspecified);
			byteSize = size_t(PackUInt64(fileInfo.nFileSizeLow, fileInfo.nFileSizeHigh));
		}

		//	Map a range of the file according to the desired size.
		DWORD sizeHigh, sizeLow;
		Tie(sizeLow, sizeHigh) = UnpackInt64(byteSize);
		const HANDLE mapping(CreateFileMappingW(file, /*lpFileMappingAttributes =*/nullptr, ProtectionFromAccess(access), sizeHigh, sizeLow, /*lpName =*/nullptr));
		ET_ABORT_UNLESS(mapping ? Result::Success : Result::Unspecified);
		ET_AT_SCOPE_EXIT(CloseHandle(mapping));

		DWORD offsetHigh, offsetLow;
		Tie(offsetHigh, offsetLow) = UnpackInt64(byteOffset);
		// Static analyzer has problems seeing the null check for `mapping`.
		ET_SUPPRESS_MSVC_WARNINGS(6387)
		const auto view(static_cast<byte*>(MapViewOfFile(mapping, ModeFromAccess(access), offsetHigh, offsetLow, byteSize)));
		ET_ABORT_UNLESS(view ? Result::Success : Result::Unspecified);

		outView = Span<byte*>(view, view + byteSize);
		return Result::Success;
	}

} // anonymous namespace

FileView::FileView() ETNoexceptHint : _access(Read), _file(INVALID_HANDLE_VALUE) {}

// ---------------------------------------------------

FileView::FileView(FileView&& view) ETNoexceptHint : FileView() {
	Swap(*this, view);
}

// ---------------------------------------------------

FileView::~FileView() {
	if (_view) {
		UnmapViewOfFile(_view.Begin());
	}
}

// ---------------------------------------------------

Result FileView::ClearOrCreate(FileAccess access, const PlatformChar* path, size_t byteLength) ETNoexceptHint {
	/*	The client must explicitly specify a size when creating a file; we cannot infer a value based on the size of the file as we may not be opening
	 *	a resource that previously exists. Likewise, we cannot create a file with 0 size. */
	if (byteLength == EndOfFile || byteLength == 0u) {
		return Result::InvalidParameter;
	}

	HANDLE file(CreateFileW(path, PermissionsByAccess(access), FILE_SHARE_READ, /*lpSecurityAttributes =*/nullptr, TRUNCATE_EXISTING, FILE_FLAG_POSIX_SEMANTICS, /*hTemplateFile =*/nullptr));
	ET_ABORT_UNLESS(file != INVALID_HANDLE_VALUE ? Result::Success : Result::Unspecified);
	ET_AT_SCOPE_EXIT(CloseHandle(file));

	Span<byte*> view;
	ET_ABORT_UNLESS(MapFile(view, file, access, 0u, byteLength));
	ET_AT_SCOPE_EXIT(if (view) UnmapViewOfFile(view.Begin()));

	Swap(_access, access);
	Swap(_view, view);
	Swap(_file, file);

	return Result::Success;
}

// ---------------------------------------------------

Result FileView::Open(FileAccess access, const PlatformChar* path, uint64 byteOffset, size_t byteLength) ETNoexceptHint {
	HANDLE file(CreateFileW(path, PermissionsByAccess(access), FILE_SHARE_READ, /*lpSecurityAttributes =*/nullptr, OPEN_EXISTING, FILE_FLAG_POSIX_SEMANTICS, /*hTemplateFile =*/nullptr));
	ET_ABORT_UNLESS(file != INVALID_HANDLE_VALUE ? Result::Success : Result::Unspecified);
	ET_AT_SCOPE_EXIT(CloseHandle(file));

	Span<byte*> view;
	ET_ABORT_UNLESS(MapFile(view, file, access, byteOffset, byteLength));
	ET_AT_SCOPE_EXIT(if (view) UnmapViewOfFile(view.Begin()));

	Swap(_access, access);
	Swap(_view, view);
	Swap(_file, file);

	return Result::Success;
}

// ---------------------------------------------------

bool FileView::HasAccess(FileAccess access) const ETNoexceptHint {
	return access <= _access;
}

// ---------------------------------------------------

FileTime FileView::GetLastWriteTime() const ETNoexceptHint {
	FILETIME lastWriteTime{ 0u, 0u };
	GetFileTime(_file, /*lpCreationTime =*/nullptr, /*lpLastAccessTime =*/nullptr, ETAddressOf(lastWriteTime));
	return FileTime(PackUInt64(lastWriteTime.dwLowDateTime, lastWriteTime.dwHighDateTime));
}

// ---------------------------------------------------

size_t FileView::GetByteSize() const ETNoexceptHint {
	return _view.GetSize();
}

// ---------------------------------------------------

void FileView::Prefetch(size_t byteOffset, size_t byteLength) const ETNoexceptHint {
	Prefetch(Slice<const byte>(byteOffset, byteLength));
}

// ---------------------------------------------------

void FileView::Prefetch(Span<const byte*> range) const ETNoexceptHint {
	enum : size_t { PrefetchStride = 4096u };
#if PREFETCHVIRTUALMEMORY_AVAILABLE
	WIN32_MEMORY_RANGE_ENTRY ranges[] = { { const_cast<char*>(range.Begin()), range.GetSize() } };
	PrefetchVirtualMemory(GetCurrentProcess(), ETCountOf(ranges), ranges, 0);
#else
	COMPILERMESSAGEGENERIC("Windows 8 PrefetchVirtualMemory-optimized file mapping object unavailable, use the Windows 8 SDK or newer to enable")
	while (range) {
		//	TODO: Is it possible to use prefetch instructions to avoid polluting the processor caches?
		int readTarget(*reinterpret_cast<const volatile char*>(range.Begin()));
		static_cast<void>(readTarget);

		range.SetBegin(Minimum(range.Begin() + PrefetchStride, range.End()));
	}
#endif // if PREFETCHVIRTUALMEMORY_AVAILABLE
}

// ---------------------------------------------------

void FileView::Evict(size_t byteOffset, size_t byteLength) const ETNoexceptHint {
	Evict(Slice<const byte>(byteOffset, byteLength));
}

// ---------------------------------------------------

void FileView::Evict(Span<const byte*> /*range*/) const ETNoexceptHint {
	//	Unlike POSIX, Windows lacks any mechanism to evict pages from the cache, so this is a no-op.
}

// ---------------------------------------------------

void* FileView::Get(size_t byteOffset) const ETNoexceptHint {
	const auto result(_view.Begin() + byteOffset);
	return result <= _view.End() ? result : nullptr;
}

// ---------------------------------------------------

void Swap(FileView& lhs, FileView& rhs) ETNoexceptHint {
	Swap(lhs._access, rhs._access);
	Swap(lhs._view, rhs._view);
	Swap(lhs._file, rhs._file);
}

} // namespace Eldritch2
