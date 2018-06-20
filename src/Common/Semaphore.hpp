/*==================================================================*\
  Semaphore.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/Platform.hpp>
#include <Common/Mpl/IntTypes.hpp>
//------------------------------------------------------------------//
#if (ET_PLATFORM_LINUX || ET_PLATFORM_MAC)
#	include <semaphore.h>
#endif
//------------------------------------------------------------------//

#if ET_PLATFORM_WINDOWS
using HANDLE = void*;
#endif

namespace Eldritch2 {

	class Semaphore {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Semaphore instance.
		Semaphore(uint32 maximumCount, uint32 initialCount = 0u);
	//!	Constructs this @ref Semaphore instance.
		Semaphore(const Semaphore&) = delete;

		~Semaphore();

	// ---------------------------------------------------

	public:
		uint32	IncreaseCount(uint32 count = 1u);

		void	Acquire();

		bool	TryAcquire();

	// ---------------------------------------------------

	//!	Disable copy construction.
		Semaphore&	operator=(const Semaphore&) = delete;

	// - DATA MEMBERS ------------------------------------
	private:
#	if ET_PLATFORM_WINDOWS
		HANDLE	_semaphore;
#	elif (ET_PLATFORM_LINUX || ET_PLATFORM_MAC)
		sem_t	_semaphore;
#	endif
	};

}	// namespace Eldritch2