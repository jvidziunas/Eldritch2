/*==================================================================*\
  Platform.Win32.cpp
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

	struct ProcessMemoryCountersEx : public PROCESS_MEMORY_COUNTERS_EX {
		//!	Constructs this @ref ProcessMemoryCountersEx instance.
		ETForceInlineHint ProcessMemoryCountersEx() ETNoexceptHint {
			GetProcessMemoryInfo(GetCurrentProcess(), PPROCESS_MEMORY_COUNTERS(this), DWORD(sizeof(*this)));
		}
	};

	// ---

	struct MemoryStatusEx : public MEMORYSTATUSEX {
		//!	Constructs this @ref MemoryStatusEx instance.
		ETForceInlineHint MemoryStatusEx() ETNoexceptHint {
			dwLength = DWORD(sizeof(*this));
			GlobalMemoryStatusEx(this);
		}
	};

	// ---

	struct CurrentSystemInfo : public SYSTEM_INFO {
		//!	Constructs this @ref SystemInfo instance.
		ETForceInlineHint CurrentSystemInfo() ETNoexceptHint {
			GetSystemInfo(this);
		}
	};

	// ---

	static const struct CurrentSystemInfo SystemInfo;

	// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint bool VerifyWindowsLibrariesNewerThanVersion(DWORD majorVersion, DWORD minorVersion) ETNoexceptHint {
		OSVERSIONINFOEXW versionInfo;
		versionInfo.dwOSVersionInfoSize = DWORD(sizeof(versionInfo));
		versionInfo.dwMajorVersion      = majorVersion;
		versionInfo.dwMinorVersion      = minorVersion;

		ULONGLONG conditionMask(0u);
		conditionMask = VerSetConditionMask(conditionMask, VER_MAJORVERSION, VER_GREATER_EQUAL);
		conditionMask = VerSetConditionMask(conditionMask, VER_MINORVERSION, VER_GREATER_EQUAL);
		return VerifyVersionInfoW(ETAddressOf(versionInfo), VER_MAJORVERSION | VER_MINORVERSION, conditionMask) != FALSE;
	}

} // anonymous namespace

size_t GetCurrentWorkingSetInBytes() ETNoexceptHint {
	return ProcessMemoryCountersEx().WorkingSetSize;
}

// ---------------------------------------------------

size_t GetPeakWorkingSetInBytes() ETNoexceptHint {
	return ProcessMemoryCountersEx().PeakWorkingSetSize;
}

// ---------------------------------------------------

size_t GetMaximumWorkingSetInBytes() ETNoexceptHint {
	return size_t(MemoryStatusEx().ullTotalVirtual);
}

// ---------------------------------------------------

size_t GetVirtualMemoryAllocationByteGranularity() ETNoexceptHint {
	return size_t(SystemInfo.dwAllocationGranularity);
}

// ---------------------------------------------------

ETPureFunctionHint CoreInfo GetCoreInfo() ETNoexceptHint {
	CoreInfo cores{};
	DWORD    length(0);

	if (GetLogicalProcessorInformationEx(RelationAll, /*Buffer =*/nullptr, ETAddressOf(length)) == FALSE && GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
		char* buffer(ETStackAlloc(char, length));

		if (GetLogicalProcessorInformationEx(RelationAll, PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX(buffer), ETAddressOf(length))) {
			const char* const end(buffer + length);

			while (buffer < end) {
				const auto& processorInfo(*PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX(buffer));
				if (processorInfo.Relationship == RelationProcessorCore) {
					const auto& processor(processorInfo.Processor);
					cores.physicalCores++;
					cores.logicalCores = Reduce(processor.GroupMask, processor.GroupMask + processor.GroupCount, cores.logicalCores, [](const GROUP_AFFINITY& group, uint32 cores) ETNoexceptHint {
						return cores + CountSetBits(group.Mask);
					});
				}

				buffer += processorInfo.Size;
			}
		}
	}

	return cores;
}

// ---------------------------------------------------

ETPureFunctionHint size_t GetL0CacheLineSizeInBytes() ETNoexceptHint {
	return 64u;
}

// ---------------------------------------------------

CpuTimestamp ReadCpuTimestamp() ETNoexceptHint {
	LARGE_INTEGER counter;
	QueryPerformanceCounter(ETAddressOf(counter));
	return CpuTimestamp(counter.QuadPart);
}

// ---------------------------------------------------

ETPureFunctionHint bool IsWindows10OrNewer() ETNoexceptHint {
	return false;
}

// ---------------------------------------------------

ETPureFunctionHint bool IsWindows8OrNewer() ETNoexceptHint {
	//	Windows 8 is version 6.2 internally.
	return VerifyWindowsLibrariesNewerThanVersion(/*majorVersion=*/6u, /*minorVersion =*/2u);
}

// ---------------------------------------------------

ETPureFunctionHint bool IsWindows7OrNewer() ETNoexceptHint {
	//	Windows 7 is version 6.1 internally.
	return VerifyWindowsLibrariesNewerThanVersion(/*majorVersion=*/6u, /*minorVersion =*/1u);
}

// ---------------------------------------------------

ETPureFunctionHint bool IsPosix() ETNoexceptHint {
	//	Windows is not POSIX.
	return false;
}

// ---------------------------------------------------

ETPureFunctionHint bool IsMacOsX() ETNoexceptHint {
	//	Windows is not OSX.
	return false;
}

// ---------------------------------------------------

ETPureFunctionHint MicrosecondTime AsMicroseconds(CpuTimestamp timestamp) ETNoexceptHint {
	// Memoize ticks per microsecond.
	static const uint64 CpuTicksPerMicrosecond(([]() ETNoexceptHint -> uint64 {
		LARGE_INTEGER counter;
		QueryPerformanceFrequency(ETAddressOf(counter));
		return uint64(counter.QuadPart / /*microseconds/second =*/1000000u);
	})());

	return MicrosecondTime(uint64(timestamp) / CpuTicksPerMicrosecond);
}

// ---------------------------------------------------

ETPureFunctionHint float32 AsMilliseconds(CpuTimestamp timestamp) ETNoexceptHint {
	return float32(AsMicroseconds(timestamp)) / 1000.0f;
}

} // namespace Eldritch2
