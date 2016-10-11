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
#include <Utility/MPL/IntTypes.hpp>
#include <Utility/MPL/Platform.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Platform {

	class PlatformInterface {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref PlatformInterface instance.
		PlatformInterface();

		~PlatformInterface();

	// - MEMORY STATISTICS -------------------------------

		size_t	GetVirtualMemoryAllocationGranularityInBytes() const;

		size_t	GetCurrentWorkingSetInBytes() const;

		size_t	GetMaximumWorkingSetInBytes() const;

		size_t	GetPeakWorkingSetInBytes() const;

	// - PROCESSOR STATISTICS ----------------------------

		size_t	GetL0CacheLineSizeInBytes() const;

		size_t	GetThreadCount() const;

		size_t	GetCoreCount() const;

	// - TIMING DATA -------------------------------------

		Eldritch2::uint64	ReadTimestampCounter() const;

	// - OPERATING SYSTEM CAPABILITY QUERY ---------------

		bool	IsWindows10OrNewer() const;

		bool	IsWindows8OrNewer() const;

		bool	IsWindows7OrNewer() const;

	// - DATA MEMBERS ------------------------------------

	private:
#	if( ET_PLATFORM_WINDOWS )
		//!	Timestamp value in ticks per microsecond.
		const Eldritch2::uint64	_timestampFrequency;
#	endif
	};

}	// namespace Platform
}	// namespace Eldritch2