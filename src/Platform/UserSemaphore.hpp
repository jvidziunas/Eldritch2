/*==================================================================*\
  UserSemaphore.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Platform.hpp>
#include <Utility/MPL/Compiler.hpp>
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//
#if ET_PLATFORM_LINUX || ET_PLATFORM_MAC
#	include <semaphore.h>
#endif
//------------------------------------------------------------------//

#if ET_PLATFORM_WINDOWS
using HANDLE = void*;
#endif

namespace Eldritch2 {
namespace Platform {

	class UserSemaphore {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref UserSemaphore instance.
		UserSemaphore( Eldritch2::uint32 maximumCount, Eldritch2::uint32 initialCount = 0u );
	//!	Constructs this @ref UserSemaphore instance.
		UserSemaphore( const UserSemaphore& ) = delete;

		~UserSemaphore();

	// ---------------------------------------------------

	public:
		Eldritch2::uint32	IncreaseCount( Eldritch2::uint32 count = 1u );

		void				Acquire();

		bool				TryAcquire();

	// ---------------------------------------------------

		UserSemaphore&	operator=( const UserSemaphore& ) = delete;

	// - DATA MEMBERS ------------------------------------
	private:
#	if ET_PLATFORM_WINDOWS
		HANDLE	_semaphore;
#	elif ET_PLATFORM_LINUX || ET_PLATFORM_MAC
		sem_t	_semaphore;
#	endif
	};

}	// namespace Platform
}	// namespace Eldritch2