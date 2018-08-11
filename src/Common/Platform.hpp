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

using CpuTimestamp    = uint64;
using MicrosecondTime = uint64;

struct CoreInfo {
	uint32 physicalCores;
	uint32 logicalCores;
};

// - MEMORY STATISTICS -------------------------------

size_t GetVirtualMemoryAllocationGranularityInBytes();

size_t GetCurrentWorkingSetInBytes();

size_t GetMaximumWorkingSetInBytes();

size_t GetPeakWorkingSetInBytes();

// - PROCESSOR STATISTICS ----------------------------

ETPureFunctionHint CoreInfo GetCoreInfo();

ETPureFunctionHint size_t GetL0CacheLineSizeInBytes();

// - TIMING DATA -------------------------------------

CpuTimestamp ReadCpuTimestamp();

// - OPERATING SYSTEM CAPABILITY QUERY ---------------

ETPureFunctionHint bool IsWindows10OrNewer();

ETPureFunctionHint bool IsWindows8OrNewer();

ETPureFunctionHint bool IsWindows7OrNewer();

ETPureFunctionHint bool IsPosix();

ETPureFunctionHint bool IsMacOsX();

} // namespace Eldritch2
