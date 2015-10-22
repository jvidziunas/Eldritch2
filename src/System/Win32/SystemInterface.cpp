/*==================================================================*\
  SystemInterface.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/StandardLibrary.hpp>
#include <System/Win32/SystemInterface.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
#include <Process.h>
#include <psapi.h>
//------------------------------------------------------------------//
#include <atomic>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
#if( PSAPI_VERSION == 1 )
	ET_LINK_LIBRARY( "Psapi.lib" )
#endif
//------------------------------------------------------------------//

using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2::System;
using namespace ::Eldritch2;
using namespace ::std;

namespace {

	enum : uint64 {
		TICKS_PER_MICROSECOND = 1000000u
	};

// ---------------------------------------------------

	struct ProcessMemoryCountersEx : public ::PROCESS_MEMORY_COUNTERS_EX {
	public:
		ETInlineHint ProcessMemoryCountersEx() {
			::GetProcessMemoryInfo( ::GetCurrentProcess(), reinterpret_cast<::PROCESS_MEMORY_COUNTERS*>(this), static_cast<::DWORD>(sizeof(*this)) );
		}
	};

// ---------------------------------------------------

	struct SystemInfo : public ::SYSTEM_INFO {
	public:
		ETInlineHint SystemInfo() {
			::GetSystemInfo( this );
		}
	};

// ---------------------------------------------------

	static uint64 ReadPerformanceFrequency() {
		::LARGE_INTEGER	counter;
		::QueryPerformanceFrequency( &counter );

		// QueryPerformanceFrequency() returns the number of ticks in a second;
		// we want a value in microseconds.
		return (static_cast<uint64>(counter.QuadPart) / TICKS_PER_MICROSECOND);
	}

// ---------------------------------------------------

	static bool VerifyWindowsLibrariesNewerThanVersion( ::DWORD majorVersion, ::DWORD minorVersion ) {
		::OSVERSIONINFOEX	versionInfo;
		::ULONGLONG			conditionMask;

		versionInfo.dwOSVersionInfoSize = static_cast<::DWORD>(sizeof(versionInfo));
		versionInfo.dwMajorVersion = majorVersion;
		versionInfo.dwMinorVersion = minorVersion;
		
		conditionMask = ::VerSetConditionMask( 0, VER_MAJORVERSION, VER_GREATER_EQUAL );
		conditionMask = ::VerSetConditionMask( conditionMask, VER_MINORVERSION, VER_GREATER_EQUAL );

		return 0 != ::VerifyVersionInfo( &versionInfo, (VER_MAJORVERSION | VER_MINORVERSION), conditionMask );
	}

}	// anonymous namespace

namespace Eldritch2 {
namespace System {
namespace Win32 {

	SystemInterface::SystemInterface() : _timestampFrequency( ReadPerformanceFrequency() ) {
		if( !IsWindows7OrNewer() ) {
			::MessageBox( nullptr, SL("You are using an unsupported version of Windows. Certain portions of the program may not function correctly or at all. Please update to Windows 7 or newer in order to ensure correct behavior of this program."), SL(""), MB_OK );
		}
	}

// ---------------------------------------------------

	size_t SystemInterface::GetCurrentWorkingSetInBytes() const {
		const ProcessMemoryCountersEx	counters;

		return counters.WorkingSetSize;
	}

// ---------------------------------------------------

	size_t SystemInterface::GetPeakWorkingSetInBytes() const {
		const ProcessMemoryCountersEx	counters;

		return counters.PeakWorkingSetSize;
	}

// ---------------------------------------------------

	size_t SystemInterface::GetMaximumWorkingSetInBytes() const {
		struct MemoryStatusEx : public ::MEMORYSTATUSEX {
		public:
			ETInlineHint MemoryStatusEx() {
				dwLength = static_cast<::DWORD>(sizeof( *this ));
				::GlobalMemoryStatusEx( this );
			}
		};

	// ---

		const MemoryStatusEx	memoryStatus;

		return static_cast<size_t>(memoryStatus.ullTotalVirtual);
	}

// ---------------------------------------------------

	size_t SystemInterface::GetVirtualMemoryAllocationGranularityInBytes() const {
		const SystemInfo	systemInfo;

		return static_cast<size_t>(systemInfo.dwAllocationGranularity);
	}

// ---------------------------------------------------

	uint64 SystemInterface::GetThreadCount() const {
		const SystemInfo	systemInfo;
		
		return static_cast<uint64>(systemInfo.dwNumberOfProcessors);
	}

// ---------------------------------------------------

	uint64 SystemInterface::GetCoreCount() const {
		return GetThreadCount();
	}

// ---------------------------------------------------

	uint64 SystemInterface::GetL0CacheLineSizeInBytes() const { 
		return 64u;
	}

// ---------------------------------------------------

	uint64 SystemInterface::ReadTimestampCounter() const {
		::LARGE_INTEGER	counter;

		::QueryPerformanceCounter( &counter );

		return (static_cast<uint64>(counter.QuadPart) / _timestampFrequency);
	}

// ---------------------------------------------------

	bool SystemInterface::IsWindows10OrNewer() const {
		return false;
	}

// ---------------------------------------------------

	bool SystemInterface::IsWindows8OrNewer() const {
		// Windows 8 is version 6.2 internally
		return VerifyWindowsLibrariesNewerThanVersion( 6u, 2u );
	}

// ---------------------------------------------------

	bool SystemInterface::IsWindows7OrNewer() const {
		// Windows 7 is version 6.1 internally
		return VerifyWindowsLibrariesNewerThanVersion( 6u, 1u );
	}

}	// namespace Win32
}	// namespace Foundation
}	// namespace Eldritch2