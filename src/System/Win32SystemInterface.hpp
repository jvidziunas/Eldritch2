/*==================================================================*\
  Win32SystemInterface.hpp
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
#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Foundation {

	namespace Foundation	= ::Eldritch2::Foundation;
	namespace Utility		= ::Eldritch2::Utility;

// ---------------------------------------------------

	class Win32SystemInterface : public Foundation::SystemInterface {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref Win32SystemInterface instance.
		Win32SystemInterface();

		//!	Destroys this @ref Win32SystemInterface instance.
		~Win32SystemInterface();

	// - MEMORY STATISTICS -------------------------------

		size_t	GetCurrentWorkingSetInBytes() const override sealed;

		size_t	GetPeakWorkingSetInBytes() const override sealed;

		size_t	GetMaximumWorkingSetInBytes() const override sealed;

		size_t	GetVirtualMemoryAllocationGranularityInBytes() const override sealed;

	// - PROCESSOR STATISTICS ----------------------------

		::Eldritch2::uint64	GetThreadCount() const override sealed;

		::Eldritch2::uint64	GetCoreCount() const override sealed;

		::Eldritch2::uint64	GetL0CacheLineSizeInBytes() const override sealed;

	// - TIMING DATA -------------------------------------

		::Eldritch2::uint64	ReadTimestampCounter() const override sealed;

	// - OPERATING SYSTEM CAPABILITY QUERY ---------------

		bool	IsWindows7OrNewer() const;

		bool	IsWindows8OrNewer() const;

		bool	IsWindows10OrNewer() const;

	// - DATA MEMBERS ------------------------------------

	private:
		// Timestamp value in ticks per microsecond.
		const ::Eldritch2::uint64	_timestampFrequency;
	};

}	// namespace Foundation
}	// namespace Eldritch2