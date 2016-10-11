/*==================================================================*\
  PlatformInterface.Win32.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/StandardLibrary.hpp>
#include <Platform/PlatformInterface.hpp>
#include <Utility/MPL/CharTypes.hpp>
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

namespace Eldritch2 {
namespace Platform {
namespace {

	enum : uint64 {
		TicksPerMicrosecond = 1000000u
	};

// ---------------------------------------------------

	struct ProcessMemoryCountersEx : public PROCESS_MEMORY_COUNTERS_EX {
	public:
	//!	Constructs this @ref ProcessMemoryCountersEx instance.
		ETInlineHint ProcessMemoryCountersEx() {
			GetProcessMemoryInfo( GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(this), static_cast<DWORD>(sizeof(*this)) );
		}

		~ProcessMemoryCountersEx() = default;
	};

// ---------------------------------------------------

	struct SystemInfo : public SYSTEM_INFO {
	public:
	//!	Constructs this @ref SystemInfo instance.
		ETInlineHint SystemInfo() {
			GetSystemInfo( this );
		}

		~SystemInfo() = default;
	};

// ---------------------------------------------------

	struct MemoryStatusEx : public MEMORYSTATUSEX {
	public:
	//!	Constructs this @ref MemoryStatusEx instance.
		ETInlineHint MemoryStatusEx() {
			dwLength = static_cast<DWORD>( sizeof( *this ) );

			GlobalMemoryStatusEx( this );
		}

		~MemoryStatusEx() = default;
	};

// ---------------------------------------------------

	static ETPureFunctionHint uint64 ReadPerformanceFrequency() {
		LARGE_INTEGER	counter;

		QueryPerformanceFrequency( &counter );

	//	QueryPerformanceFrequency() returns the number of ticks in a second;
	//	we want a value in microseconds.
		return (static_cast<uint64>(counter.QuadPart) / TicksPerMicrosecond);
	}

// ---------------------------------------------------

	static ETPureFunctionHint bool VerifyWindowsLibrariesNewerThanVersion( DWORD majorVersion, DWORD minorVersion ) {
		OSVERSIONINFOEX	versionInfo;
		ULONGLONG		conditionMask;

		versionInfo.dwOSVersionInfoSize = static_cast<DWORD>(sizeof(versionInfo));
		versionInfo.dwMajorVersion = majorVersion;
		versionInfo.dwMinorVersion = minorVersion;
		
		conditionMask = VerSetConditionMask( 0, VER_MAJORVERSION, VER_GREATER_EQUAL );
		conditionMask = VerSetConditionMask( conditionMask, VER_MINORVERSION, VER_GREATER_EQUAL );

		return 0 != VerifyVersionInfoW( &versionInfo, (VER_MAJORVERSION | VER_MINORVERSION), conditionMask );
	}

}	// anonymous namespace

	PlatformInterface::PlatformInterface() : _timestampFrequency( ReadPerformanceFrequency() ) {
		if( !IsWindows7OrNewer() ) {
			MessageBoxW( nullptr, L"You are using an unsupported version of Windows. Certain portions of the program may not function correctly or at all. Please update to Windows 7 or newer in order to ensure correct behavior of this program.", L"", MB_OK );
		}
	}

// ---------------------------------------------------

	PlatformInterface::~PlatformInterface() {}

// ---------------------------------------------------

	size_t PlatformInterface::GetCurrentWorkingSetInBytes() const {
		return ProcessMemoryCountersEx().WorkingSetSize;
	}

// ---------------------------------------------------

	size_t PlatformInterface::GetPeakWorkingSetInBytes() const {
		return ProcessMemoryCountersEx().PeakWorkingSetSize;
	}

// ---------------------------------------------------

	size_t PlatformInterface::GetMaximumWorkingSetInBytes() const {
		return static_cast<size_t>( MemoryStatusEx().ullTotalVirtual );
	}

// ---------------------------------------------------

	size_t PlatformInterface::GetVirtualMemoryAllocationGranularityInBytes() const {
		return static_cast<size_t>( SystemInfo().dwAllocationGranularity );
	}

// ---------------------------------------------------

	size_t PlatformInterface::GetThreadCount() const {
		return static_cast<size_t>( SystemInfo().dwNumberOfProcessors );
	}

// ---------------------------------------------------

	size_t PlatformInterface::GetCoreCount() const {
		return GetThreadCount();
	}

// ---------------------------------------------------

	size_t PlatformInterface::GetL0CacheLineSizeInBytes() const { 
		return 64u;
	}

// ---------------------------------------------------

	uint64 PlatformInterface::ReadTimestampCounter() const {
		LARGE_INTEGER	counter;

		QueryPerformanceCounter( &counter );

		return (static_cast<uint64>(counter.QuadPart) / _timestampFrequency);
	}

// ---------------------------------------------------

	bool PlatformInterface::IsWindows10OrNewer() const {
		return false;
	}

// ---------------------------------------------------

	bool PlatformInterface::IsWindows8OrNewer() const {
	//	Windows 8 is version 6.2 internally
		return VerifyWindowsLibrariesNewerThanVersion( 6u, 2u );
	}

// ---------------------------------------------------

	bool PlatformInterface::IsWindows7OrNewer() const {
	//	Windows 7 is version 6.1 internally
		return VerifyWindowsLibrariesNewerThanVersion( 6u, 1u );
	}

}	// namespace Foundation
}	// namespace Eldritch2