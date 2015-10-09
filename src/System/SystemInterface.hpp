/*==================================================================*\
  SystemInterface.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Noncopyable.hpp>
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	namespace Scheduler {
		class	TaskScheduler;
	}

	namespace Utility {
		template <typename ResultObject>
		struct	ResultPair;
	}

	class	Allocator;
}

namespace Eldritch2 {
namespace Foundation {

	namespace Foundation	= ::Eldritch2::Foundation;
	namespace Scheduler		= ::Eldritch2::Scheduler;
	namespace Utility		= ::Eldritch2::Utility;

// ---------------------------------------------------

	class ETPureAbstractHint SystemInterface : private Utility::Noncopyable {
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

		virtual size_t	GetCurrentWorkingSetInBytes() const abstract;

		virtual	size_t	GetPeakWorkingSetInBytes() const abstract;

		virtual size_t	GetMaximumWorkingSetInBytes() const abstract;

		virtual size_t	GetVirtualMemoryAllocationGranularityInBytes() const abstract;

	// - PROCESSOR STATISTICS ----------------------------

		virtual ::Eldritch2::uint64	GetThreadCount() const abstract;

		virtual ::Eldritch2::uint64	GetCoreCount() const abstract;

		virtual ::Eldritch2::uint64	GetL0CacheLineSizeInBytes() const abstract;

	// - TIMING DATA -------------------------------------

		virtual ::Eldritch2::uint64	ReadTimestampCounter() const abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		// Constructs this SystemInterface instance.
		SystemInterface();

		// Destroys this SystemInterface instance.
		~SystemInterface();
	};

}	// namespace Foundation
}	// namespace Eldritch2