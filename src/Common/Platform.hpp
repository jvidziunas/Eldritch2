/*==================================================================*\
  PlatformInterface.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/IntTypes.hpp>
#include <Common/Mpl/Platform.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

enum class MicrosecondTime : uint64 { Uninitialized = 0ull };
enum class CpuTimestamp : uint64 { Uninitialized = 0ull };
enum class FileTime : int64 { Unavailable = -1ll };

enum : uint32 { MicrosecondsPerSecond = 1000000u };

struct CoreInfo {
	uint32 physicalCores;
	uint32 logicalCores;
};

// ---

enum class WindowsApiVersion {
	Unavailable,
	Windows7,
	Windows8,
	Windows10
};

// ---

enum class MacApiVersion {
	Unavailable,
	OSX
};

// ---

enum class PosixApiVersion {
	Unavailable,
};

// - MEMORY STATISTICS -------------------------------

size_t GetVirtualMemoryAllocationByteGranularity() ETNoexceptHint;

size_t GetCurrentWorkingSetInBytes() ETNoexceptHint;

size_t GetMaximumWorkingSetInBytes() ETNoexceptHint;

size_t GetPeakWorkingSetInBytes() ETNoexceptHint;

// - PROCESSOR STATISTICS ----------------------------

ETPureFunctionHint CoreInfo GetCoreInfo() ETNoexceptHint;

ETPureFunctionHint size_t GetL0CacheLineSizeInBytes() ETNoexceptHint;

// - TIMING DATA -------------------------------------

CpuTimestamp ReadCpuTimestamp() ETNoexceptHint;

// - OPERATING SYSTEM CAPABILITY QUERY ---------------

ETPureFunctionHint bool IsWindows10OrNewer() ETNoexceptHint;

ETPureFunctionHint bool IsWindows8OrNewer() ETNoexceptHint;

ETPureFunctionHint bool IsWindows7OrNewer() ETNoexceptHint;

ETPureFunctionHint bool IsMacOsX() ETNoexceptHint;

ETPureFunctionHint bool IsPosix() ETNoexceptHint;

// - TIME UNIT CONVERSION ----------------------------

ETPureFunctionHint MicrosecondTime AsMicroseconds(CpuTimestamp timestamp) ETNoexceptHint;

ETConstexpr ETPureFunctionHint float32 AsMilliseconds(MicrosecondTime microseconds) ETNoexceptHint;
ETPureFunctionHint float32 AsMilliseconds(CpuTimestamp timestamp) ETNoexceptHint;

ETConstexpr ETPureFunctionHint MicrosecondTime MicrosecondsPerFrame(uint32 framerateInHz) ETNoexceptHint;

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Platform.inl>
//------------------------------------------------------------------//
