/*==================================================================*\
  Platform.Win32.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/CharTypes.hpp>
#include <Common/Platform.hpp>
#include <Common/Memory.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
#include <Process.h>
#include <malloc.h>
#include <psapi.h>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
#if PSAPI_VERSION == 1
ET_LINK_LIBRARY("Psapi.lib")
#endif
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace {

	enum : uint64 { TicksPerMicrosecond = 1000000u };

	// ---------------------------------------------------

	struct ProcessMemoryCountersEx : public PROCESS_MEMORY_COUNTERS_EX {
	public:
		//!	Constructs this @ref ProcessMemoryCountersEx instance.
		ETInlineHint ProcessMemoryCountersEx() {
			GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(this), static_cast<DWORD>(sizeof(*this)));
		}

		~ProcessMemoryCountersEx() = default;
	};

	// ---------------------------------------------------

	struct SystemInfo : public SYSTEM_INFO {
	public:
		//!	Constructs this @ref SystemInfo instance.
		ETInlineHint SystemInfo() {
			GetSystemInfo(this);
		}

		~SystemInfo() = default;
	};

	// ---------------------------------------------------

	struct MemoryStatusEx : public MEMORYSTATUSEX {
	public:
		//!	Constructs this @ref MemoryStatusEx instance.
		ETInlineHint MemoryStatusEx() {
			dwLength = static_cast<DWORD>(sizeof(*this));

			GlobalMemoryStatusEx(this);
		}

		~MemoryStatusEx() = default;
	};

	// ---------------------------------------------------

	ETPureFunctionHint uint64 GetCpuTicksPerMicrosecond() {
		LARGE_INTEGER counter;

		QueryPerformanceFrequency(&counter);

		//	QueryPerformanceFrequency() returns the number of ticks in a second;
		//	we want a value in microseconds.
		return (static_cast<uint64>(counter.QuadPart) / TicksPerMicrosecond);
	}

	// ---------------------------------------------------

	ETPureFunctionHint bool VerifyWindowsLibrariesNewerThanVersion(DWORD majorVersion, DWORD minorVersion) {
		OSVERSIONINFOEX versionInfo;
		ULONGLONG       conditionMask;

		versionInfo.dwOSVersionInfoSize = static_cast<DWORD>(sizeof(versionInfo));
		versionInfo.dwMajorVersion      = majorVersion;
		versionInfo.dwMinorVersion      = minorVersion;

		conditionMask = VerSetConditionMask(0, VER_MAJORVERSION, VER_GREATER_EQUAL);
		conditionMask = VerSetConditionMask(conditionMask, VER_MINORVERSION, VER_GREATER_EQUAL);

		return VerifyVersionInfoW(&versionInfo, (VER_MAJORVERSION | VER_MINORVERSION), conditionMask) != FALSE;
	}

	// ---------------------------------------------------

	ETPureFunctionHint ETInlineHint DWORD CountSetBits(ULONG_PTR mask) {
		enum : DWORD { LSHIFT = CHAR_BIT * sizeof(ULONG_PTR) - 1u };

		ULONG_PTR currentBit((ULONG_PTR)1u << LSHIFT);
		DWORD     count(0u);

		for (DWORD i(0); i <= LSHIFT; ++i) {
			count += ((mask & currentBit) ? 1u : 0u);
			currentBit >>= 1u;
		}

		return count;
	}

	// ---------------------------------------------------

	static const auto CpuTicksPerMicrosecond(GetCpuTicksPerMicrosecond());

} // anonymous namespace

size_t GetCurrentWorkingSetInBytes() {
	return ProcessMemoryCountersEx().WorkingSetSize;
}

// ---------------------------------------------------

size_t GetPeakWorkingSetInBytes() {
	return ProcessMemoryCountersEx().PeakWorkingSetSize;
}

// ---------------------------------------------------

size_t GetMaximumWorkingSetInBytes() {
	return static_cast<size_t>(MemoryStatusEx().ullTotalVirtual);
}

// ---------------------------------------------------

size_t GetVirtualMemoryAllocationGranularityInBytes() {
	return static_cast<size_t>(SystemInfo().dwAllocationGranularity);
}

// ---------------------------------------------------

ETPureFunctionHint CoreInfo GetCoreInfo() {
	CoreInfo info{};
	DWORD    length(0);

	if (GetLogicalProcessorInformationEx(RelationAll, nullptr, &length) == FALSE && GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
		char* buffer(static_cast<char*>(_alloca(length)));

		if (GetLogicalProcessorInformationEx(RelationAll, reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(buffer), &length)) {
			const char* const end(buffer + length);

			while (buffer < end) {
				const SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX& processor(*reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(buffer));

				if (processor.Relationship == RelationProcessorCore) {
					info.physicalCores++;

					for (size_t g(0); g < processor.Processor.GroupCount; ++g) {
						info.logicalCores += CountSetBits(processor.Processor.GroupMask[g].Mask);
					}
				}

				buffer += processor.Size;
			}
		}
	}

	return info;
}

// ---------------------------------------------------

ETPureFunctionHint size_t GetL0CacheLineSizeInBytes() {
	return 64u;
}

// ---------------------------------------------------

CpuTimestamp ReadCpuTimestamp() {
	LARGE_INTEGER counter;

	QueryPerformanceCounter(&counter);

	return static_cast<CpuTimestamp>(counter.QuadPart) / CpuTicksPerMicrosecond;
}

// ---------------------------------------------------

ETPureFunctionHint bool IsWindows10OrNewer() {
	return false;
}

// ---------------------------------------------------

ETPureFunctionHint bool IsWindows8OrNewer() {
	//	Windows 8 is version 6.2 internally.
	return VerifyWindowsLibrariesNewerThanVersion(6u, 2u);
}

// ---------------------------------------------------

ETPureFunctionHint bool IsWindows7OrNewer() {
	//	Windows 7 is version 6.1 internally.
	return VerifyWindowsLibrariesNewerThanVersion(6u, 1u);
}

// ---------------------------------------------------

ETPureFunctionHint bool IsPosix() {
	//	Windows is not POSIX.
	return false;
}

// ---------------------------------------------------

ETPureFunctionHint bool IsMacOsX() {
	//	Windows is not OSX.
	return false;
}

} // namespace Eldritch2
