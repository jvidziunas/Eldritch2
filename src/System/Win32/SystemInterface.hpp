/*==================================================================*\
  SystemInterface.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <System/SystemInterface.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace System {
namespace Win32 {

	class SystemInterface : public System::SystemInterface {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref SystemInterface instance.
		SystemInterface();

		//!	Destroys this @ref SystemInterface instance.
		~SystemInterface() = default;

	// - MEMORY STATISTICS -------------------------------

		size_t	GetVirtualMemoryAllocationGranularityInBytes() const override sealed;

		size_t	GetCurrentWorkingSetInBytes() const override sealed;

		size_t	GetMaximumWorkingSetInBytes() const override sealed;

		size_t	GetPeakWorkingSetInBytes() const override sealed;

	// - PROCESSOR STATISTICS ----------------------------

		::Eldritch2::uint64	GetL0CacheLineSizeInBytes() const override sealed;

		::Eldritch2::uint64	GetThreadCount() const override sealed;

		::Eldritch2::uint64	GetCoreCount() const override sealed;

	// - TIMING DATA -------------------------------------

		::Eldritch2::uint64	ReadTimestampCounter() const override sealed;

	// - OPERATING SYSTEM CAPABILITY QUERY ---------------

		bool	IsWindows10OrNewer() const;

		bool	IsWindows8OrNewer() const;

		bool	IsWindows7OrNewer() const;

	// - DATA MEMBERS ------------------------------------

	private:
		// Timestamp value in ticks per microsecond.
		const ::Eldritch2::uint64	_timestampFrequency;
	};

}	// namespace Win32
}	// namespace Foundation
}	// namespace Eldritch2