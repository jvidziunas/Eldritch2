/*==================================================================*\
  MappedFile.Win32.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/MappedFile.hpp>
#include <Common/ScopeGuard.hpp>
#include <Common/ErrorCode.hpp>
#include <Common/Memory.hpp>
#include <Common/Math.hpp>
//------------------------------------------------------------------//
#include <eastl/utility.h>
#include <Windows.h>
//------------------------------------------------------------------//

#if _WIN32_WINNT >= _WIN32_WINNT_WIN8
#	define PREFETCHVIRTUALMEMORY_AVAILABLE 1
#else
#	define PREFETCHVIRTUALMEMORY_AVAILABLE 0
#endif

namespace Eldritch2 {

namespace {

	enum : size_t {
		PrefetchStride = 4096u
	};

	// ---

	ETConstexpr ETInlineHint ETPureFunctionHint DWORD PermissionsFromAccess(MappedFile::AccessType access) ETNoexceptHint {
		return access < MappedFile::AccessType::Write ? GENERIC_READ : (GENERIC_READ | GENERIC_WRITE);
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETPureFunctionHint DWORD ProtectModeFromAccess(MappedFile::AccessType access) ETNoexceptHint {
		return access < MappedFile::AccessType::Write ? PAGE_READONLY : PAGE_READWRITE;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETPureFunctionHint DWORD MapModeFromAccess(MappedFile::AccessType access) ETNoexceptHint {
		//	Officially, FILE_MAP_WRITE implies FILE_MAP_READ but this is still a little more clear.
		return access < MappedFile::AccessType::Write ? FILE_MAP_READ : (FILE_MAP_READ | FILE_MAP_WRITE);
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint ErrorCode MapFile(Range<char*>& outMapping, const PlatformChar* const path, DWORD disposition, MappedFile::AccessType access, uint64 byteOffset, size_t byteSize) ETNoexceptHint {
		const HANDLE file(CreateFileW(path, PermissionsFromAccess(access), FILE_SHARE_READ, /*lpSecurityAttributes =*/nullptr, disposition, FILE_FLAG_POSIX_SEMANTICS, /*hTemplateFile =*/nullptr));
		ET_ABORT_UNLESS(file != INVALID_HANDLE_VALUE ? Error::None : Error::Unspecified);
		ET_AT_SCOPE_EXIT(CloseHandle(file));

		if (byteSize == MappedFile::LengthOfFile) {
			BY_HANDLE_FILE_INFORMATION fileInfo;
			ET_ABORT_UNLESS((GetFileInformationByHandle(file, ETAddressOf(fileInfo)) != 0) ? Error::None : Error::Unspecified);
			byteSize = (uint64(fileInfo.nFileSizeHigh) << 32u) | fileInfo.nFileSizeLow;
		}

		//	Next, map a range of the file according to the desired size.
		ULARGE_INTEGER sizeHelper;
		sizeHelper.QuadPart = byteSize;

		const HANDLE mapping(CreateFileMappingW(file, /*lpFileMappingAttributes =*/nullptr, ProtectModeFromAccess(access), sizeHelper.HighPart, sizeHelper.LowPart, /*lpName =*/nullptr));
		ET_ABORT_UNLESS(mapping ? Error::None : Error::Unspecified);
		ET_AT_SCOPE_EXIT(CloseHandle(mapping));

		ULARGE_INTEGER offsetHelper;
		offsetHelper.QuadPart = byteOffset;
		// Static analyzer has problems seeing the null check for `mapping`.
		ET_SUPPRESS_MSVC_WARNINGS(6387)
		const auto view(static_cast<char*>(MapViewOfFile(mapping, MapModeFromAccess(access), offsetHelper.HighPart, offsetHelper.LowPart, byteSize)));
		ET_ABORT_UNLESS(view ? Error::None : Error::Unspecified);

		outMapping = { view, view + byteSize };
		return Error::None;
	}

} // anonymous namespace

MappedFile::MappedFile() ETNoexceptHint : _access(), _mapping(nullptr, nullptr) {}

// ---------------------------------------------------

MappedFile::MappedFile(MappedFile&& file) ETNoexceptHint : MappedFile() {
	Swap(*this, file);
}

// ---------------------------------------------------

MappedFile::~MappedFile() {
	if (_mapping.IsEmpty()) {
		return;
	}

	UnmapViewOfFile(_mapping.Begin());
}

// ---------------------------------------------------

ErrorCode MappedFile::ClearOrCreate(AccessType accessMode, const PlatformChar* path, size_t byteLength) ETNoexceptHint {
	/*	The client must explicitly specify a size when creating a file; we cannot infer a value based on the size of the file since we may not be opening
	 *	something that already exists. Likewise, we cannot create a file with 0 size. */
	if (byteLength == LengthOfFile || byteLength == 0u) {
		return Error::InvalidParameter;
	}

	Range<char*> mapping(nullptr, nullptr);
	ET_ABORT_UNLESS(MapFile(mapping, path, TRUNCATE_EXISTING, accessMode, 0u, byteLength));
	ET_AT_SCOPE_EXIT(if (mapping) UnmapViewOfFile(mapping.Begin()));

	Swap(_mapping, mapping);

	return Error::None;
}

// ---------------------------------------------------

ErrorCode MappedFile::Open(AccessType access, const PlatformChar* path, uint64 byteOffset, size_t byteLength) ETNoexceptHint {
	Range<char*> mapping(nullptr, nullptr);
	ET_ABORT_UNLESS(MapFile(mapping, path, OPEN_EXISTING, access, byteOffset, byteLength));
	ET_AT_SCOPE_EXIT(if (mapping) UnmapViewOfFile(mapping.Begin()));

	Swap(_mapping, mapping);

	return Error::None;
}

// ---------------------------------------------------

bool MappedFile::HasAccess(AccessType access) const ETNoexceptHint {
	return access <= _access;
}

// ---------------------------------------------------

size_t MappedFile::GetSizeInBytes() const ETNoexceptHint {
	return _mapping.GetSize();
}

// ---------------------------------------------------

void MappedFile::Prefetch(size_t byteOffset, size_t byteLength) const ETNoexceptHint {
	Prefetch(GetRange<const char>(byteOffset, byteLength));
}

// ---------------------------------------------------

void MappedFile::Prefetch(Range<const char*> range) const ETNoexceptHint {
#if PREFETCHVIRTUALMEMORY_AVAILABLE
	WIN32_MEMORY_RANGE_ENTRY ranges[] = { { const_cast<char*>(range.Begin()), range.GetSize() } };
	PrefetchVirtualMemory(GetCurrentProcess(), ETCountOf(ranges), ranges, 0);
#else
	COMPILERMESSAGEGENERIC("Windows 8 PrefetchVirtualMemory-optimized file mapping object unavailable, use the Windows 8 SDK or newer to enable")
	while (range) {
		//	TODO: Is it possible to use prefetch instructions to avoid polluting the processor caches?
		register int readTarget(*reinterpret_cast<const volatile char*>(range.Begin()));
		static_cast<void>(readTarget);

		range.SetBegin(Min(range.Begin() + PrefetchStride, range.End()));
	}
#endif // if PREFETCHVIRTUALMEMORY_AVAILABLE
}

// ---------------------------------------------------

void MappedFile::Evict(size_t byteOffset, size_t byteLength) const ETNoexceptHint {
	Evict(GetRange<const char>(byteOffset, byteLength));
}

// ---------------------------------------------------

void MappedFile::Evict(Range<const char*> /*range*/) const ETNoexceptHint {
	//	Unlike POSIX, Windows lacks any mechanism to evict pages from the cache, so this is a no-op.
}

// ---------------------------------------------------

void* MappedFile::Get(size_t byteOffset) const ETNoexceptHint {
	char* const result(_mapping.Begin() + byteOffset);
	return result <= _mapping.End() ? result : nullptr;
}

// ---------------------------------------------------

void Swap(MappedFile& lhs, MappedFile& rhs) ETNoexceptHint {
	Swap(lhs._access, rhs._access);
	Swap(lhs._mapping, rhs._mapping);
}

} // namespace Eldritch2
