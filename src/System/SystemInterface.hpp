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
#include <Utility/MPL/Noncopyable.hpp>
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace System {

	class ETPureAbstractHint SystemInterface : private Utility::Noncopyable {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum class SystemMemoryState {
			LOW_USAGE,
			NORMAL_USAGE,
			HIGH_USAGE,
			CRITICAL_USAGE,

		// ---

			COUNT
		};

	// - MEMORY STATISTICS -------------------------------

		virtual size_t	GetVirtualMemoryAllocationGranularityInBytes() const abstract;

		virtual size_t	GetMaximumWorkingSetInBytes() const abstract;

		virtual size_t	GetCurrentWorkingSetInBytes() const abstract;

		virtual	size_t	GetPeakWorkingSetInBytes() const abstract;

	// - PROCESSOR STATISTICS ----------------------------

		virtual ::Eldritch2::uint64	GetL0CacheLineSizeInBytes() const abstract;

		virtual ::Eldritch2::uint64	GetThreadCount() const abstract;

		virtual ::Eldritch2::uint64	GetCoreCount() const abstract;

	// - TIMING DATA -------------------------------------

		virtual ::Eldritch2::uint64	ReadTimestampCounter() const abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//!	Constructs this @ref SystemInterface instance.
		SystemInterface() = default;

		//!	Destroys this @ref SystemInterface instance.
		~SystemInterface() = default;
	};

}	// namespace System
}	// namespace Eldritch2